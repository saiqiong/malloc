/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   malloc.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: saxiao <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/11/04 12:10:25 by saxiao            #+#    #+#             */
/*   Updated: 2019/11/08 17:29:19 by saxiao           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <sys/mman.h>
#include <unistd.h>
#include "../includes/malloc.h"

t_block	*find_block(t_block *block_list, size_t size, int type)
{
	while (block_list)
	{
		if (type == TINY || type == SMALL)
		{
			if (block_list->is_free == FREE \
			&& block_list->size > size + METABLOCK_SIZE)
				return (block_list);
		}
		else
		{
			if (block_list->is_free == FREE)
				return (block_list);
		}
		block_list = block_list->next;
	}
	return (NULL);
}

void	seperate_block(t_block *fit_block, size_t size)
{
	t_block *temp;
	t_block *rest;

	temp = fit_block->next;
	rest = (t_block *)(fit_block->addr + size);
	rest->addr = fit_block->addr + size + METABLOCK_SIZE;
	rest->size = fit_block->size - size - METABLOCK_SIZE;
	fit_block->size = size;
	*(rest->addr - 1) = *(fit_block->addr - 1);
	*(fit_block->addr - 1) = *(fit_block->addr - 1) & NOT_END;
	*(rest->addr - 1) = *(rest->addr - 1) & NOT_BEGIN;
	fit_block->is_free = USED;
	fit_block->next = rest;
	rest->pre = fit_block;
	rest->next = temp;
	if (temp)
		temp->pre = rest;
}

void	*allocate_large_size(t_block *block)
{
	block->is_free = USED;
	return (void *)block->addr;
}

void	*allocate_in_block(t_zone *zone, size_t size, int type)
{
	t_block *fit_block;

	if (!zone)
	{
		if (allocate_zone(zone, type, size))
			return (NULL);
		return (allocate_in_block((g_map + type)->zone, size, type));
	}
	fit_block = find_block(zone->block_list, size, type);
	if (fit_block)
	{
		if (type == LARGE)
			return (allocate_large_size(fit_block));
		seperate_block(fit_block, size);
		// fit_block->is_free = USED;
		return ((void *)fit_block->addr);
	}
	else
	{
		if (allocate_zone(zone, type, size))
			return (NULL);
		return (allocate_in_block(zone->next, size, type));
	}
}

#include <stdio.h>

void init_g_map()
{
	int i;

	i = 0;
	while(i != MAP_NUMBER)
	{
		g_map[i].zone = NULL;
		i++;
	}
}

void	*malloc(size_t size)
{
	int		malloc_type;
	void	*res;

	static int init = 0;
	if(init == 0)
	{
		init_g_map();
		init = 1;

	}

	ft_printf("-->> Size: %zu\n", size);
	if (size <= MAX_TINY_BLOCK)
		malloc_type = TINY;
	else if (size <= MAX_SMALL_BLOCK)
		malloc_type = SMALL;
	else
		malloc_type = LARGE;
	pthread_mutex_lock(&g_map_mutex);

	res = allocate_in_block(g_map[malloc_type].zone, ALIGHN(size), malloc_type);
	pthread_mutex_unlock(&g_map_mutex);
	ft_printf("-->> %p\n\n", res);
	show_alloc_mem_ex();

	// return (debug_infor(res, ((t_block *)((char *)res - METABLOCK_SIZE))->size, "Malloced"));
	return(res);
}
