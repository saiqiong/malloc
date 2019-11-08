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
void connect_block(t_zone *zone, t_zone *next)
{
	t_block *cp;

	cp = zone->block_list;
	while (cp->next)
		cp = cp->next;
	cp->next = next->block_list;
	next->block_list->pre = cp;
}

size_t calculate_zone_size(int type, size_t size)
{
	int max_block;
	size_t zone_size;

	if (type == LARGE)
	{
		zone_size = (size + METABLOCK_SIZE + METAZONE_SIZE) % getpagesize() ? size + METABLOCK_SIZE + METAZONE_SIZE + getpagesize() - ((size + METABLOCK_SIZE + METAZONE_SIZE) % getpagesize()) : size + METABLOCK_SIZE + METAZONE_SIZE;
	}
	else
	{
		if (type == TINY)
			max_block = MAX_TINY_BLOCK;
		else
			max_block = MAX_SMALL_BLOCK;
		zone_size = ((max_block + METABLOCK_SIZE) * 100 + METAZONE_SIZE) % getpagesize() ? (max_block + METABLOCK_SIZE) * 100 + METAZONE_SIZE + getpagesize() - ((max_block + METABLOCK_SIZE) * 100 + METAZONE_SIZE) % getpagesize() : (max_block + METABLOCK_SIZE) * 100 + +METAZONE_SIZE;
	}
	ft_printf("zone size is %zu\n", zone_size);
	return zone_size;
}

void init_new_zone(t_zone *new_zone, char *address, int zone_size, int type)
{
	new_zone->addr = address + METAZONE_SIZE;
	new_zone->next = NULL;
	new_zone->pre = NULL;
	new_zone->block_list = (t_block *)(address + METAZONE_SIZE);
	new_zone->block_list->addr = address + METABLOCK_SIZE + METAZONE_SIZE;
	*(new_zone->block_list->addr - 1) = BEGIN_AND_END; // mark this block as the begining and end of a zone

	new_zone->block_list->next = NULL;
	new_zone->block_list->pre = NULL;
	new_zone->block_list->size = zone_size - METABLOCK_SIZE - METAZONE_SIZE;
	new_zone->block_list->is_free = FREE;
	new_zone->type = type;
}

int allocate_zone(t_zone *zone, int type, size_t size)
{
	size_t zone_size;
	char *address;
	t_zone *new_zone;

	zone_size = calculate_zone_size(type, size);
	address = mmap(0, zone_size, PROT_READ | PROT_WRITE, MAP_ANONYMOUS | MAP_PRIVATE, -1, 0);
	if (address == MAP_FAILED)
		return 1;
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
	return 0;
}

t_block *find_block(t_block *block_list, size_t size, int type)
{
	while (block_list)
	{
		if (type == TINY || type == SMALL)
		{
			if (block_list->is_free == FREE && block_list->size > size + METABLOCK_SIZE)
				return block_list;
		}
		else
		{
			if (block_list->is_free == FREE)
				return block_list;
		}
		block_list = block_list->next;
	}
	return NULL;
}

void seperate_block(t_block *fit_block, size_t size)
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
	printf("fit_block flag%d\n", *(fit_block->addr - 1));
	printf("rest flag%d\n", *(rest->addr - 1));
	if (temp)
		temp->pre = rest;
}

void *allocate_large_size(t_block *block)
{
	block->is_free = USED;
	return (void *)block->addr;
}

void *allocate_in_block(t_zone *zone, size_t size, int malloc_type)
{
	t_block *fit_block;

	if (!zone)
	{
		if (allocate_zone(zone, malloc_type, size))
			return NULL;
		return allocate_in_block((g_map + malloc_type)->zone, size, malloc_type);
	}
	fit_block = find_block(zone->block_list, size, malloc_type);
	if (fit_block)
	{
		if (malloc_type == LARGE)
			return allocate_large_size(fit_block);
		seperate_block(fit_block, size);
		return (void *)fit_block->addr;
	}
	else
	{
		if (allocate_zone(zone, malloc_type, size))
			return NULL;
		return allocate_in_block(zone->next, size, malloc_type);
	}
}

void *debug_infor(char *addr, size_t size, char *type)
{
	char *debug_env;
	debug_env = getenv("MALLOCDEBUG");
	if (debug_env && (!ft_strcmp(debug_env, "true") || !ft_strcmp(debug_env, "TRUE")))
	{
		if (!addr)
			ft_printf("Opps! %s failed\n", type);
		else
			ft_printf("%s from address: %p %zu bytes\n", type, addr, size);
	}
	return addr;
}

void *ft_malloc(size_t size)
{
	int malloc_type;
	void *res;

	if (size <= MAX_TINY_BLOCK)
		malloc_type = TINY;
	else if (size <= MAX_SMALL_BLOCK)
		malloc_type = SMALL;
	else
		malloc_type = LARGE;
	pthread_mutex_lock(&g_map_mutex);
	res = allocate_in_block(g_map[malloc_type].zone, ALIGHN(size), malloc_type);
	pthread_mutex_unlock(&g_map_mutex);
	return debug_infor(res, ((t_block *)((char *)res - METABLOCK_SIZE))->size, "Malloced");
}

void print_block(t_block *list, size_t *total)
{
	if (list->is_free == USED)
	{
		ft_printf("%p - %p : %zu bytes\n", list->addr, list->addr + list->size, list->size);
		*total = *total + list->size;
	}
}

void show_alloc_mem()
{
	int i;
	t_block *list;
	char *type;
	size_t total;

	i = -1;
	type = "TINY";
	total = 0;
	while (++i < MAP_NUMBER)
	{
		if (i)
			type = (i == 1 ? "SMALL" : "LARGE");
		if (g_map[i].zone)
		{
			ft_printf("%s : %p\n", type, g_map[i].zone->addr);
			list = (g_map[i].zone)->block_list;
			while (list)
			{
				print_block(list, &total);
				list = list->next;
			}
		}
	}
	ft_printf("Total : %zu bytes\n", total);
}

void show_alloc_mem_ex()
{
	int i;
	t_block *list;
	char *free;
	char *type;

	i = -1;
	type = "TINY";
	while (++i < MAP_NUMBER)
	{
		if (i)
			type = (i == 1 ? "SMALL" : "LARGE");
		if (g_map[i].zone)
		{
			ft_printf("%s : %p\n", type, g_map[i].zone->addr);
			list = (g_map[i].zone)->block_list;
			while (list)
			{
				free = (list->is_free == FREE ? "free" : "used");
				ft_printf("%p - %p : %zu bytes %s\n", list->addr, list->addr + list->size, list->size, free);
				list = list->next;
			}
		}
	}
}

void merge_free(t_block *block_list)
{
	t_block *temp;

	if (block_list->pre && block_list->pre->is_free == FREE)
	{
		ft_printf("in merge free 00000000000000\n");
		temp = block_list->next;
		block_list->pre->size = block_list->pre->size + METABLOCK_SIZE + block_list->size;
		block_list->pre->next = temp;
		*(block_list->pre->addr - 1) = (*(block_list->pre->addr - 1) & NOT_END) | (*(block_list->addr - 1) & END);
		if (temp)
			temp->pre = block_list->pre;
		block_list = block_list->pre;
	}
	if (block_list->next && block_list->next->is_free == FREE)
	{
		ft_printf("in merge free 00000000000000\n");

		temp = block_list->next->next;
		block_list->size = block_list->size + METABLOCK_SIZE + block_list->next->size;
		*(block_list->addr - 1) = (*(block_list->addr - 1) & NOT_END) | (*(block_list->next->addr - 1) & END);
		block_list->next = temp;
		if (temp)
			temp->pre = block_list;
	}
}

void free_zone(t_block *block)
{
	t_block *cp;
	t_zone *zone;

	cp = block->pre ? block->pre : block;
	if (cp->is_free == FREE && (*(cp->addr - 1) & BEGIN_AND_END) == BEGIN_AND_END)
	{
		zone = (t_zone *)(cp->addr - METABLOCK_SIZE - METAZONE_SIZE);
		if (!((zone->type == TINY || zone->type == SMALL) && !zone->pre && !zone->next))
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

void ft_free(void *ptr)
{
	t_block *block;

	pthread_mutex_lock(&g_map_mutex);
	if (ptr)
	{
		block = (t_block *)((char *)ptr - METABLOCK_SIZE);
		if (block && block->is_free == USED)
		{
			debug_infor(block->addr, block->size, "Freed");
			block->is_free = FREE;
			merge_free(block);
			free_zone(block);
		}
		else
			debug_infor(NULL, 0, "Free");
	}
	else
		debug_infor(NULL, 0, "Free");
	pthread_mutex_unlock(&g_map_mutex);
}

void *ft_calloc(size_t count, size_t size)
{
	void *res;

	res = ft_malloc(count * size);
	if (res)
	{
		ft_bzero(res, ALIGHN(count * size));
	}
	return res;
}

void *ft_realloc(void *ptr, size_t size)
{
	size_t allocated_size;
	char *addr;
	char temp[((t_block *)((char *)ptr - METABLOCK_SIZE))->size];

	if (!ptr)
		return debug_infor(NULL, 0, "Realloc");
	allocated_size = ((t_block *)((char *)ptr - METABLOCK_SIZE))->size;
	if (allocated_size >= (size_t)(ALIGHN(size)))
		return ptr;
	ft_memmove(temp, ptr, allocated_size);
	ft_free(ptr);
	addr = ft_malloc(size);
	ft_memmove(addr, temp, allocated_size);
	return addr;
}

int main()
{
	char *test = ft_malloc(30);
	// char *test1 = ft_malloc(5000);
	// char *test2 = ft_malloc(5000);
	ft_free(test);
	ft_free(test);


	//  ft_realloc(test, 20);
	//  ft_free(test);
	// // test[0] = 's';
	// // test[1] = 'a';
	// // test[2] = 'l';
	// // test[3] = 'u';
	// // test[4] = 't';
	// // test[5] = '\0';
	// char *test1 = ft_malloc(10);
	// char *test2 = ft_malloc(100);
	// show_alloc_mem();
	// ft_realloc(test, 50);
	show_alloc_mem();
	show_alloc_mem_ex();

	// char *test3 = ft_malloc(1000);
	// char *test4 = ft_malloc(3000);
	// char *test5 = ft_malloc(3000);
	// char *test6 = ft_malloc(2000);
	// char *test7 = ft_malloc(2000);
	// char *test8 = ft_malloc(200);
	// char *test9 = ft_malloc(60);
	// ft_free(test2);
	// ft_free(test);
	// ft_free(test2);
	// ft_free(test6);
	// ft_free(test11);
	// printf("block size %lu\n", METABLOCK_SIZE);
	// printf("size of pointer=%lu, size size_t=%lu, size int=%lu\n", sizeof(test3), sizeof(size_t), sizeof(int));
	// printf("zone size %lu\n", METAZONE_SIZE);
	// printf("page size =%d\n", getpagesize());
}
