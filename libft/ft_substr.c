/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_substr.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yzioual <yzioual@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/31 08:58:42 by yzioual           #+#    #+#             */
/*   Updated: 2023/11/06 10:25:28 by yzioual          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static char	*ft_empty_string(void)
{
	char	*result;

	result = (char *)malloc(sizeof(char));
	if (result == NULL)
		return (NULL);
	result[0] = '\0';
	return (result);
}

char	*ft_substr(const char *s, unsigned int start, size_t len)
{
	size_t	i;
	char	*block;
	size_t	s_len;
	size_t	rem;
	char	*result;

	if (!s)
		return (NULL);
	s_len = ft_strlen(s);
	if (start >= s_len)
		return (ft_empty_string());
	rem = s_len - start;
	if (len > rem)
		block = (char *)malloc(rem + 1);
	else
		block = (char *)malloc(len + 1);
	if (block == NULL)
		return (NULL);
	result = block;
	i = start;
	while (s[i] != '\0' && len-- != 0)
		*block++ = s[i++];
	*block = '\0';
	return (result);
}
