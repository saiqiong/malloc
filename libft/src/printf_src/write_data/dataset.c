/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   dataset.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: saxiao <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/01/23 08:56:12 by saxiao            #+#    #+#             */
/*   Updated: 2018/02/01 09:59:37 by saxiao           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <ft_printf.h>

static void	p_data_flush(t_pdata *print)
{
	if (print->fd > -1)
		write(print->fd, print->data, print->data_len);
	print->data[0] = '\0';
	print->data_len = 0;
}

void		dataset(t_pdata *print, t_uchar c, size_t n)
{
	size_t	i;

	i = 0;
	if (n <= 0 || (print->ret_on_full && print->len >= print->data_size))
		return ;
	while (i < n)
	{
		if (print->ret_on_full && print->len >= print->data_size)
			break ;
		if (print->data_len == print->data_size)
			p_data_flush(print);
		print->data[print->data_len] = c;
		(print->len)++;
		(print->data_len)++;
		i++;
	}
}
