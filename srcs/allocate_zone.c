/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   allocate_zone.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: saxiao <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/11/08 14:25:14 by saxiao            #+#    #+#             */
/*   Updated: 2019/11/08 15:19:41 by saxiao           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <sys/mman.h>
#include <unistd.h>
#include "../includes/malloc.h"

void	connect_block(t_zone *zone, t_zone *next)
{
	t_block *cp;

	cp = zone->block_list;
	while (cp->next)
		cp = cp->next;
	cp->next = next->block_list;
	next->block_list->pre = cp;
}

size_t	to_pagesize(size_t size)
{
	size_t res;

	res = size % getpagesize() ?\
		size + getpagesize() - (size % getpagesize()) : size;
	return (res);
}

size_t	calculate_zone_size(int type, size_t size)
{
	int		max_block;
	size_t	zone_size;

	if (type == LARGE)
		zone_size = to_pagesize(size + METABLOCK_SIZE + METAZONE_SIZE);
	else
	{
		if (type == TINY)
			max_block = MAX_TINY_BLOCK;
		else
			max_block = MAX_SMALL_BLOCK;
		zone_size = to_pagesize((max_block + METABLOCK_SIZE) * 100\
				+ METAZONE_SIZE);
	}
	return (zone_size);
}

void	init_new_zone(t_zone *new_zone, char *address, int zone_size, int type)
{
	new_zone->addr = address + METAZONE_SIZE;
	new_zone->next = NULL;
	new_zone->pre = NULL;
	new_zone->block_list = (t_block *)(address + METAZONE_SIZE);
	new_zone->block_list->addr = address + METABLOCK_SIZE + METAZONE_SIZE;
	*(new_zone->block_list->addr - 1) = BEGIN_AND_END;
	new_zone->block_list->next = NULL;
	new_zone->block_list->pre = NULL;
	new_zone->block_list->size = zone_size - METABLOCK_SIZE - METAZONE_SIZE;
	new_zone->block_list->is_free = FREE;
	new_zone->type = type;
}

int		allocate_zone(t_zone *zone, int type, size_t size)
{
	size_t	zone_size;
	char	*address;
	t_zone	*new_zone;

	zone_size = calculate_zone_size(type, size);
	address = mmap(0, zone_size, PROT_READ | PROT_WRITE, MAP_ANONYMOUS |
			MAP_PRIVATE, -1, 0);
	if (address == MAP_FAILED)
		return (1);
	new_zone = (t_zone *)address;
	init_new_zone(new_zone, address, zone_size, type);
	if (!zone)
		(g_map + type)->zone = new_zone;
	else
	{
		zone->next = new_zone;
		new_zone->pre = zone;
		connect_block(zone, new_zone);
	}
	return (0);
}
