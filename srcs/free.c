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

void	merge_free(t_block *list)
{
	t_block *temp;

	if (list->pre && list->pre->is_free == FREE)
	{
		temp = list->next;
		list->pre->size = list->pre->size + METABLOCK_SIZE + list->size;
		list->pre->next = temp;
		*(list->pre->addr - 1) = (*(list->pre->addr - 1)\
		& NOT_END) | (*(list->addr - 1) & END);
		if (temp)
			temp->pre = list->pre;
		list = list->pre;
	}
	if (list->next && list->next->is_free == FREE)
	{
		temp = list->next->next;
		list->size = list->size + METABLOCK_SIZE + list->next->size;
		*(list->addr - 1) = (*(list->addr - 1) & NOT_END)\
			| (*(list->next->addr - 1) & END);
		list->next = temp;
		if (temp)
			temp->pre = list;
	}
}

void	free_zone(t_block *block)
{
	t_block	*cp;
	t_zone	*zone;

	cp = block->pre ? block->pre : block;
	if (!cp->is_free && (*(cp->addr - 1) & BEGIN_AND_END) == BEGIN_AND_END)
	{
		zone = (t_zone *)(cp->addr - METABLOCK_SIZE - METAZONE_SIZE);
		if (!((zone->type == TINY || zone->type == SMALL) && \
		!zone->pre && !zone->next))
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

int		find_addree(void *addr)
{
	int i = 0;
	t_block *list;

	ft_printf("Find adddr start %p\n", addr);
	while (i < 3)
	{
		ft_printf("I -- >> %d\n", i);
		ft_printf("gmap[%d]\n", g_map[i]);
		ft_printf("gmap[%d]->zone %p\n", g_map[i].zone);
		if((g_map[i].zone) == NULL)
		{
			i++;
			continue;
		}
		list = (g_map[i].zone)->block_list;
		ft_printf("inside while\n");
		while (list)
		{
			ft_printf("Second while >> \n");
			if (list->addr == addr)
				return 1;
			ft_printf("------list  list->next\n");			
			list = list->next;
			// ft_printf("list %p, list->next\", list);
		}
		i++;
	}
	ft_printf("Find adddr end %p\n", addr);	
	return (0);
}
void	free(void *ptr)
{
	t_block	*block;

	// return ;
	ft_printf("free: %p\n", ptr);
	pthread_mutex_lock(&g_map_mutex);
	//check if ptr was allocated
	// if (ptr)
	if (find_addree(ptr))
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
