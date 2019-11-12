/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_intlen.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: saxiao <suddin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/04/24 05:31:01 by saxiao            #+#    #+#             */
/*   Updated: 2017/05/01 23:07:37 by saxiao           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int		ft_intlen(long n)
{
	int i;

	i = 0;
	(n <= 0) ? i++ : i;
	n = (n < 0) ? (n * -1) : n;
	while (n > 0)
	{
		n /= 10;
		i++;
	}
	return (i);
}
