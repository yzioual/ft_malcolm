/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strjoin.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yzioual <yzioual@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/31 09:04:46 by yzioual           #+#    #+#             */
/*   Updated: 2023/11/03 15:41:37 by yzioual          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strjoin(const char *s1, const char *s2)
{
	size_t	s1_size;
	size_t	s2_size;
	size_t	total_size;
	char	*block;
	char	*result;

	if (s1 == NULL || s2 == NULL)
		return (NULL);
	s1_size = ft_strlen(s1);
	s2_size = ft_strlen(s2);
	total_size = s1_size + s2_size;
	block = (char *)malloc(total_size + 1);
	if (block == NULL)
		return (NULL);
	result = block;
	while (*s1 != '\0')
		*block++ = *s1++;
	while (*s2 != '\0')
		*block++ = *s2++;
	*block = '\0';
	return (result);
}
