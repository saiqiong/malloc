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
#include <stdlib.h> // to be deleted
#include "../includes/malloc.h"

// connet the block_list in different zones
void	connect_block(t_zone *zone, t_zone *next)
{
	t_block *cp;

	cp = zone->block_list;
	while (cp->next)
		cp = cp->next;
	cp->next = next->block_list;
	next->block_list->pre = cp;
}

void	allocate_zone(t_zone *zone, int type)
 {
	 int	max_block;
	 int	zone_size;
	 char	*address;
	 t_zone *new_zone;

	if (type == TINY)
	 	max_block = MAX_TINY_BLOCK;
	else
		max_block = MAX_SMALL_BLOCK;
	printf("inside allocate zoon  222222\n");
	
	zone_size = ( max_block + METABLOCK_SIZE ) * 100 % getpagesize() ? (max_block + METABLOCK_SIZE ) * 100 + getpagesize(): (max_block + METABLOCK_SIZE ) * 100;
	address = mmap(0, zone_size, PROT_READ | PROT_WRITE, MAP_ANONYMOUS | MAP_PRIVATE, -1, 0);
	if (address == MAP_FAILED){
		// TODO mmap failed
		printf("map failed\n");
	}
	new_zone = (t_zone *)address;

	new_zone->addr = address + sizeof(t_zone);
	new_zone->next = NULL;
	new_zone->pre = NULL;
	new_zone->block_list = (t_block *)address + sizeof(t_zone);
	new_zone->block_list->addr = address + METABLOCK_SIZE +  sizeof(t_zone);
	new_zone->block_list->next = NULL;
	new_zone->block_list->pre = NULL;
	new_zone->block_list->size =zone_size - METABLOCK_SIZE - sizeof(t_zone);
	printf("inside allocate zoon  1111111 %p\n", address);
	printf("size of address %lu, zone_size is %d\n", sizeof(address), zone_size);
	new_zone->block_list->is_free = FREE;
	printf("inside allocate zoon  33333 \n");

	if(!zone)
	{
		// *zone = new_zone;
		(map + type)->zone = new_zone;
		printf("hey, in allocate zone99999999999\n");
	}
	else {
		zone->next = new_zone;
		new_zone->pre = zone;
		connect_block(zone, new_zone);
	}

	//TODO NOT YET FOR LARGE ZONE
 }

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

void	seperate_block(t_block *fit_block, size_t size)
{
	t_block *temp;
	t_block *rest;

	temp = fit_block->next;
	rest = (t_block *)(fit_block->addr + size);
	printf("in seperate_block   555555\n");
	rest->addr = fit_block->addr + size + METABLOCK_SIZE;
	rest->size = fit_block->size - size - METABLOCK_SIZE;
	fit_block->size = size;
	fit_block->is_free = USED;
	fit_block->next = rest;
	rest->pre = fit_block;
	rest->next = temp;
	if (temp)
		temp->pre = rest;
}

// void	*allocate_in_block(t_zone *zone, size_t size, int malloc_type)
// {
// 	t_block *fit_block;

// 	if (!zone){
// 		allocate_zone(&zone, malloc_type);
// 	printf("allocate in block lallalala 111111\n");
// 	}
// 	// return allocate_in_block(zone, size, malloc_type);
// 	fit_block = find_block(zone->block_list, size);
// 	printf("22222222\n");

// 	if (fit_block)
// 	{
// 		seperate_block(fit_block, size);
// 		printf("33333333\n");
// 		return fit_block->addr;
// 	}
// 	else {
// 		allocate_zone(&zone, malloc_type);
// 		return allocate_in_block(zone->next, size, malloc_type);
// 	}
// }

void	*allocate_in_block(t_zone *zone, size_t size, int malloc_type)
{
	t_block *fit_block;
	// t_zone	*zone;

	// zone = (map + malloc_type)->zone;

	if (!zone){
		allocate_zone(zone, malloc_type);
	return allocate_in_block((map + malloc_type)->zone, size, malloc_type);
	printf("allocate in block lallalala 111111\n");
	}
	fit_block = find_block(zone->block_list, size);
	printf("22222222\n");

	if (fit_block)
	{
		seperate_block(fit_block, size);
		printf("33333333\n");
		return fit_block->addr;
	}
	else {
		allocate_zone(zone, malloc_type);
		return allocate_in_block(zone->next, size, malloc_type);
	}
}
 
 void    *ft_malloc(size_t size)
 {
	int	malloc_type;

	test_number = 6;

	if (size <= MAX_TINY_BLOCK)
		malloc_type = TINY;
	else if (size <= MAX_SMALL_BLOCK)
		malloc_type = SMALL;
	else
		malloc_type = LARGE;
	printf("type is%d\n", malloc_type);
	return allocate_in_block(map[malloc_type].zone, size, malloc_type);
 }

void	show_alloc_mem()
{
	int		i;
	t_block *list;
	char	*free;

	i = -1;
	while ( ++i < MAP_NUMBER){
		printf("i here %d map[i] %p\n", i, (map[i]).zone);
		if (map[i].zone) {
			printf("TINY : %p\n", map[i].zone->addr);
			list = (map[i].zone)->block_list;
			while (list) {
				if (list->is_free == FREE)
					free = "free";
				else
					free = "used";
				printf("%p - %p : %d bytes %s\n", list->addr, list->addr + list->size - 1, (int)list->size, free);
				list = list->next;
			}
		}
	}
}

int main()
{
	char *test = ft_malloc(6);
	test[0] = 's';
	test[1] = 'a';
	test[2] = 'l';
	test[3] = 'u';
	test[4] = 't';
	test[5] = '\0';
	char *test2 = ft_malloc(10);
	char *test8 = ft_malloc(100);
	

	printf("test is %s test number is %d\n", test, test_number);
	show_alloc_mem();
}