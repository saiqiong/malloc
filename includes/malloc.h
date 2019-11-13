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
#include <pthread.h>
#include <stddef.h>
#include "../libft/include/libft.h"
#include "../libft/include/ft_printf.h"

# define FREE 0
# define USED 1
# define MAP_NUMBER 3
# define TINY 0
# define SMALL 1
# define LARGE 2
# define MAX_TINY_BLOCK 256
# define MAX_SMALL_BLOCK 3088
# define ALIGN_BYTE 16
# define ALIGHN(size) size % ALIGN_BYTE ? (size_t)(size + ALIGN_BYTE - (size % ALIGN_BYTE)) : size

typedef struct s_block
{
	char		*addr;
	struct s_block 	*next;
	struct s_block 	*pre;
	size_t		size;
	int		begin;
	int		end;
	int		is_free;
}		t_block;

typedef struct s_zone
{
	char 	*addr;
	struct 	s_zone *next;
	struct	s_zone *pre;
	t_block *block_list;
	char	*place_holder;;
	int		type;
}		t_zone;

typedef struct s_map
{
	t_zone *zone;
}		t_map;

#define METABLOCK_SIZE sizeof(t_block)
#define METAZONE_SIZE sizeof(t_zone)

static pthread_mutex_t g_map_mutex = PTHREAD_MUTEX_INITIALIZER;
t_map g_map[MAP_NUMBER];


/* free.c */
void	free(void *ptr);
int		find_address(void *addr);

/*malloc.c */
void	*malloc(size_t size);

/* realloc.c */
void	*realloc(void *ptr, size_t size);
void	*reallocf(void *ptr, size_t size);
void	*calloc(size_t count, size_t size);

/* helper.c */
void	init_g_map(void);
void	show_alloc_mem(void);
void	show_alloc_mem_ex(void);

/* allocate_zone.c */
int		allocate_zone(t_zone *zone, int type, size_t size);
#endif
