/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   malloc.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: saxiao <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/11/04 11:46:22 by saxiao            #+#    #+#             */
/*   Updated: 2019/11/04 16:52:55 by saxiao           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MALLOC_H
# define MALLOC_H
#include <stdlib.h>
#include <stddef.h>
#include "../libft/libft.h"

# define FREE 0
# define USED 1
# define MAP_NUMBER 3
# define TINY 0
# define SMALL 1
# define LARGE 2
# define MAX_TINY_BLOCK 64
# define MAX_SMALL_BLOCK 2048

typedef struct s_block
{
	char		*addr;
	struct s_block 	*next;
	struct s_block 	*pre;
	size_t		size;
	int		is_free;
	
}		t_block;

typedef struct s_zone
{
	char 	*addr;
	struct 	s_zone *next;
	t_block *block_list;
}		t_zone;

typedef struct s_map
{
	t_zone *zone;
}		t_map;

extern t_map map[MAP_NUMBER];

void	ft_free(void *ptr);
void	*ft_malloc(size_t size);
void	*ft_realloc(void *ptr, size_t size);

#endif
