/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strchr.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: saxiao <suddin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/04/16 23:07:08 by saxiao            #+#    #+#             */
/*   Updated: 2017/04/22 08:53:07 by saxiao           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <string.h>
#include "libft.h"

char	*ft_strchr(const char *s, int c)
{
	unsigned int	i;
	char			*ss;

	i = -1u;
	ss = (char *)s;
	while (ss[++i])
		if (ss[i] == (char)c)
			return (ss + i);
	if (ss[i] == (char)c)
		return (ss + i);
	return (NULL);
}
