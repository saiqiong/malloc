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

#include <unistd.h>
void	*realloc(void *ptr, size_t size)
{
	size_t	allocated_size;
	char	*addr;

	if ( !ptr || !find_address(ptr))
        return malloc(size);
		// return (debug_infor(NULL, 0, "Realloc"));
	allocated_size = ((t_block *)((char *)ptr - METABLOCK_SIZE))->size;
	if (allocated_size >= (size_t)(ALIGHN(size)))
		return (ptr);
	addr = malloc(size);
	ft_memcpy(addr, ptr, allocated_size);
	free(ptr);
return (addr);
}

void	*calloc(size_t count, size_t size)
{
	void *res;

	res = malloc(count * size);
	if (res)
		ft_bzero(res, ALIGHN(count * size));
	return (res);
}
