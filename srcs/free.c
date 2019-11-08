#include "../includes/malloc.h"

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