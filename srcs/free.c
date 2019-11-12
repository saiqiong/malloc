/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: saxiao <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/11/08 15:23:30 by saxiao            #+#    #+#             */
/*   Updated: 2019/11/08 15:23:36 by saxiao           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <sys/mman.h>
#include "../includes/malloc.h"

t_block *merge_free(t_block *list)
{
	t_block *temp;

	if (list->pre && list->pre->is_free == FREE)
	{
		temp = list->next;
		list->pre->size = list->pre->size + METABLOCK_SIZE + list->size;
		list->pre->next = temp;
		list->pre->end = list->end;
		if (temp)
			temp->pre = list->pre;
		list = list->pre;
	}
	if (list->next && list->next->is_free == FREE)
	{
		temp = list->next->next;
		list->size = list->size + METABLOCK_SIZE + list->next->size;
		list->end = list->next->end;
		list->next = temp;
		if (temp)
			temp->pre = list;
	}
	return (list);
}

void free_zone(t_block *block)
{
	t_block *cp;
	t_zone *zone;

	cp = block->pre ? block->pre : block;
	if (cp->is_free == FREE && cp->begin && cp->end)
	{
		zone = (t_zone *)((char *)cp->addr - METABLOCK_SIZE - METAZONE_SIZE);
		if (zone && !((zone->type == TINY || zone->type == SMALL) && !zone->pre && !zone->next))
		{
			if (!zone->pre)
			{
				(g_map + zone->type)->zone = zone->next;
				if (zone->next)
					zone->next->pre = NULL;
			}
			else
			{
				zone->pre->next = zone->next;
				if (zone->next)
					zone->next->pre = zone->pre;
			}
			munmap(zone, cp->size + METAZONE_SIZE + METABLOCK_SIZE);
		}
	}
}

int find_address(void *addr)
{
	int i = 0;
	t_block *list;

	while (i < MAP_NUMBER)
	{
		if ((g_map[i].zone) == NULL)
		{
			i++;
			continue;
		}
		list = (g_map[i].zone)->block_list;
		while (list)
		{
			if (list->addr == (char *)addr)
			{
				return (1);
			}
			list = list->next;
		}
		i++;
	}
	return (0);
}

void free(void *ptr)
{
	t_block *block;

	pthread_mutex_lock(&g_map_mutex);
	if (ptr && find_address(ptr))
	{
		block = (t_block *)((char *)ptr - METABLOCK_SIZE);
		if (block && block->is_free == USED)
		{
			block->is_free = FREE;
			block = merge_free(block);
			free_zone(block);
		}
	}
	pthread_mutex_unlock(&g_map_mutex);
}
