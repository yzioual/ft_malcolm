/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strdup.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yzioual <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/30 13:15:22 by yzioual           #+#    #+#             */
/*   Updated: 2023/10/30 13:18:56 by yzioual          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strdup(const char *s)
{
	size_t		s_size;
	char		*block;
	int			i;
	const char	*temp;

	s_size = 0;
	temp = s;
	while (*s)
	{
		s_size++;
		s++;
	}
	block = (char *)malloc(s_size + 1);
	if (block == NULL)
		return (NULL);
	i = 0;
	while (temp[i] != '\0')
	{
		block[i] = temp[i];
		i++;
	}
	block[i] = '\0';
	return (block);
}
