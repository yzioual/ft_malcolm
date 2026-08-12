/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strchr.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yzioual <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/30 12:43:42 by yzioual           #+#    #+#             */
/*   Updated: 2023/11/03 15:16:10 by yzioual          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strchr(const char *str, int c)
{
	unsigned char		target;
	const char			*ptr;

	target = (unsigned char)c;
	ptr = str;
	while (*ptr != '\0')
	{
		if (*ptr == target)
			return ((char *)ptr);
		ptr++;
	}
	if (*ptr == target)
		return ((char *)ptr);
	return (NULL);
}
