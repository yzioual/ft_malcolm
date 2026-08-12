/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strtrim.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yzioual <yzioual@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/31 09:08:10 by yzioual           #+#    #+#             */
/*   Updated: 2023/11/03 15:47:06 by yzioual          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static int	ft_char_in_set(char c, const char *set)
{
	while (*set)
	{
		if (c == *set)
			return (1);
		set++;
	}
	return (0);
}

char	*ft_strtrim(const char *s1, const char *set)
{
	size_t	start_trim;
	size_t	end_trim;
	size_t	s1_len;
	char	*result;

	if (!s1 || !set)
		return (NULL);
	start_trim = 0;
	while (s1[start_trim] && ft_char_in_set(s1[start_trim], set))
		start_trim++;
	if (s1[start_trim] == '\0')
		return (ft_strdup(""));
	s1_len = ft_strlen(s1);
	end_trim = s1_len - 1;
	while (end_trim > start_trim && ft_char_in_set(s1[end_trim], set))
		end_trim--;
	result = (char *)malloc(end_trim - start_trim + 2);
	if (!result)
		return (NULL);
	ft_strlcpy(result, &s1[start_trim], end_trim - start_trim + 2);
	return (result);
}
