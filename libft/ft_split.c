/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_split.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yzioual <yzioual@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/31 09:24:53 by yzioual           #+#    #+#             */
/*   Updated: 2023/11/06 11:35:54 by yzioual          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static int	ft_words_count(const char *s, char delim)
{
	int	i;
	int	in_word;
	int	count;

	if (s == NULL)
		return (-1);
	count = 0;
	i = 0;
	in_word = 0;
	while (s[i])
	{
		if (s[i] <= 32 || s[i] == delim)
			in_word = 0;
		else if (!in_word)
		{
			in_word = 1;
			count++;
		}
		i++;
	}
	return (count);
}

char	**ft_split(const char *s, char delim)
{
	int		i;
	int		k;
	int		word_len;
	int		words_count;
	int		temp;
	int		start;
	char	**result;

	if (s == NULL)
		return (NULL);
	words_count = ft_words_count(s, delim);
	result = malloc(sizeof(char *) * (words_count + 1));
	if (result == NULL)
		return (NULL);
	k = -1;
	i = 0;
	temp = 0;
	while (++k < words_count)
	{
		while (s[i] && (s[i] <= 32 || s[i] == delim))
			i++;
		start = i;
		while (s[i] && s[i] > 32 && s[i] != delim)
			i++;
		word_len = i - start;
		result[k] = malloc(sizeof(char) * (word_len + 1));
		if (result[k] == NULL)
			return (NULL);
		temp = -1;
		while (++temp < word_len)
			result[k][temp] = s[start + temp];
		result[k][temp] = '\0';	
	}
	result[words_count] = NULL;
	return (result);
}
