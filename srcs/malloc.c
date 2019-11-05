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

	if (type == TINY)
	 	max_block = MAX_TINY_BLOCK;
	else
		max_block = MAX_SMALL_BLOCK;
	zone_size = ( max_block + METABLOCK_SIZE ) * 100 % getpagesize() ? (max_block + METABLOCK_SIZE ) * 100 : (max_block + METABLOCK_SIZE ) * 100 + getpagesize();
	address = (t_zone *)mmap(0, zone_size, PROT_READ | PROT_WRITE, MAP_ANONYMOUS | MAP_PRIVATE, -1, 0);
	if (address == MAP_FAILED){
		// TODO mmap failed
	}
	*zone = address;
	(*zone)->addr = address;
	(*zone)->next = NULL;
	(*zone)->pre = NULL;
	(*zone)->block_list = 
	//TODO NOT YET FOR LARGE ZONE
 }

// void	*allocate()
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
