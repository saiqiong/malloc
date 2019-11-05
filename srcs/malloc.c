/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   malloc.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: saxiao <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/11/04 12:10:25 by saxiao            #+#    #+#             */
/*   Updated: 2019/11/04 16:52:44 by saxiao           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <sys/mman.h>
#include <unistd.h>
#include "../includes/malloc.h"
 void	allocate_zone(t_zone **zone, int type)
 {
	 int	max_block;
	 int	zone_size;
	 char	*address;
	 t_zone *new_zone;

	if (type == TINY)
	 	max_block = MAX_TINY_BLOCK;
	else
		max_block = MAX_SMALL_BLOCK;
	zone_size = ( max_block + METABLOCK_SIZE ) * 100 % getpagesize() ? (max_block + METABLOCK_SIZE ) * 100 + getpagesize(): (max_block + METABLOCK_SIZE ) * 100;
	address = (t_zone *)mmap(0, zone_size, PROT_READ | PROT_WRITE, MAP_ANONYMOUS | MAP_PRIVATE, -1, 0);
	if (address == MAP_FAILED){
		// TODO mmap failed
	}
	new_zone = address;
	new_zone->addr = address;
	new_zone->next = NULL;
	new_zone->pre = NULL;
	new_zone->block_list = address;
	new_zone->block_list->addr = address + METABLOCK_SIZE;
	new_zone->block_list->next = NULL;
	new_zone->block_list->pre = NULL;
	new_zone->block_list->size =zone_size - METABLOCK_SIZE;
	new_zone->block_list->is_free = FREE;

	if(!(*zone))
		*zone = new_zone;
	else {
		(*zone)->next = new_zone;
		new_zone->pre = *zone;
	}

	//TODO NOT YET FOR LARGE ZONE
 }

// void	*allocate()
t_block	*find_block(t_block *block_list, size_t size)
{
	while (block_list)
	{
		if (block_list->is_free == FREE && block_list->size > size + METABLOCK_SIZE)
			return block_list;
		block_list = block_list->next;
	}
	return NULL;
}

void	*allocate_in_block(t_zone *zone, size_t size, int malloc_type)
{
	t_block *fit_block;
	t_block *temp;
	t_block *rest;
	t_block *block_list;

	block_list = zone->block_list;
	fit_block = find_block(block_list, size);
	if (fit_block)
	{
		temp = block_list->next;
		rest = block_list->addr + size;
		rest->addr = block_list->addr + size + METABLOCK_SIZE;
		rest->size = block_list->size - size - METABLOCK_SIZE;
		block_list->size = size;
		block_list->is_free = USED;
		block_list->next = rest;
		rest->pre = block_list;
		rest->next = temp;
		temp->pre = rest;
		return block_list->addr;
	}
	else {
		allocate_zone(&zone, malloc_type);

	}

}

 void 	*allocate_memory(t_zone *zone, size_t size, int malloc_type)
 {
	 if (!zone)
	 	allocate_zone(&zone, malloc_type);
	


 }
 
 void    *ft_malloc(size_t size)
 {
	int	malloc_type;

	if (size <= MAX_TINY_BLOCK)
		malloc_type = TINY;
	else if (size <= MAX_SMALL_BLOCK)
		malloc_type = SMALL;
	else
		malloc_type = LARGE;
	return allocate_memory(map[malloc_type].zone, size, malloc_type);
 }
