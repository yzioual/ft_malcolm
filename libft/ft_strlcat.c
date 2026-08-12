/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strlcat.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yzioual <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/30 12:41:10 by yzioual           #+#    #+#             */
/*   Updated: 2023/11/03 15:49:33 by yzioual          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

unsigned int	ft_strlcat(char *dest, char *src, unsigned int size)
{
	unsigned int	d_len;
	unsigned int	s_len;
	unsigned int	i;

	i = 0;
	d_len = ft_strlen(dest);
	s_len = ft_strlen(src);
	if (size == 0 || size <= d_len)
		return (size + s_len);
	while (src[i] != '\0' && i < size - d_len - 1)
	{
		dest[d_len + i] = src[i];
		i++;
	}
	dest[i + d_len] = '\0';
	return (d_len + s_len);
}
