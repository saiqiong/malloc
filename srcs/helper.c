/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   print_infor.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: saxiao <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/11/08 15:24:08 by saxiao            #+#    #+#             */
/*   Updated: 2019/11/08 15:24:11 by saxiao           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include "../includes/malloc.h"

void	init_g_map(void)
{
	int i;

	i = 0;
	while (i != MAP_NUMBER)
	{
		(g_map + i)->zone = NULL;
		i++;
	}
}

void	print_block(t_block *list, size_t *total)
{
	if (list->is_free == USED)
	{
		ft_printf("%p - %p : %zu bytes\n", list->addr,\
			list->addr + list->size, list->size);
		*total = *total + list->size;
	}
}

void	show_alloc_mem(void)
{
	int		i;
	t_block	*list;
	char	*type;
	size_t	total;

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

void	show_alloc_mem_ex(void)
{
	int		i;
	t_block	*list;
	char	*free;
	char	*type;

	i = -1;
	type = "TINY";
	while (++i < MAP_NUMBER)
	{
		if (i)
			type = (i == 1 ? "SMALL" : "LARGE");
		if ((g_map + i)->zone)
		{
			ft_printf("%s : %p\n", type, g_map[i].zone->addr);
			list = (g_map[i].zone)->block_list;
			while (list)
			{
				free = (list->is_free == FREE ? "free" : "used");
				ft_printf("%p - %p : %zu bytes %s\n", list->addr,\
					list->addr + list->size, list->size, free);
				list = list->next;
			}
		}
	}
}
