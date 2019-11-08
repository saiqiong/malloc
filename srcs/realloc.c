/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   realloc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: saxiao <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/11/08 15:24:25 by saxiao            #+#    #+#             */
/*   Updated: 2019/11/08 15:24:41 by saxiao           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/malloc.h"

// void	*realloc(void *ptr, size_t size)
// {
// 	size_t	allocated_size;
// 	char	*addr;
// 	char	temp[((t_block *)((char *)ptr - METABLOCK_SIZE))->size];

// 	if (!ptr)
// 		return (debug_infor(NULL, 0, "Realloc"));
// 	allocated_size = ((t_block *)((char *)ptr - METABLOCK_SIZE))->size;
// 	if (allocated_size >= (size_t)(ALIGHN(size)))
// 		return (ptr);
// 	ft_memmove(temp, ptr, allocated_size);
// 	free(ptr);
// 	addr = malloc(size);
// 	ft_memmove(addr, temp, allocated_size);
// 	return (addr);
// }

// void	*calloc(size_t count, size_t size)
// {
// 	void *res;

// 	res = malloc(count * size);
// 	if (res)
// 		ft_bzero(res, ALIGHN(count * size));
// 	return (res);
// }
