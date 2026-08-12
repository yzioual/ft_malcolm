/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_itoa.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yzioual <yzioual@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/31 09:29:21 by yzioual           #+#    #+#             */
/*   Updated: 2023/11/06 10:12:24 by yzioual          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static int	ft_count_len(int n)
{
	int	count;

	count = 0;
	if (n == 0)
		return (1);
	if (n < 0)
		count++;
	while (n)
	{
		n /= 10;
		count++;
	}
	return (count);
}

static char	*ft_strcpy(char *dest, const char *src)
{
	char	*original_dest;

	original_dest = dest;
	while (*src != '\0')
	{
		*dest = *src;
		dest++;
		src++;
	}
	*dest = '\0';
	return (original_dest);
}

static char	*ft_handle_special_cases(char *result, int n)
{
	if (n == -2147483648)
	{
		result = (char *)malloc(12 * sizeof(char));
		if (!result)
			return (NULL);
		ft_strcpy(result, "-2147483648");
		return (result);
	}
	else if (n == 0)
	{
		result = (char *)malloc(2 * sizeof(char));
		if (!result)
			return (NULL);
		result[0] = '0';
		result[1] = '\0';
		return (result);
	}
	return (NULL);
}

static void	ft_populate_digits(int i, int sign, int n, char *result)
{
	while (i >= 0)
	{
		result[i] = (n % 10) + '0';
		n /= 10;
		i--;
	}
	if (sign == -1)
		result[0] = '-';
}

char	*ft_itoa(int n)
{
	char	*result;
	int		count;
	int		i;
	int		sign;

	sign = 1;
	if (n == 0 || n == -2147483648)
		return (ft_handle_special_cases(NULL, n));
	count = ft_count_len(n);
	if (n < 0 && n != -2147483648)
	{
		sign = -1;
		n = -n;
	}
	result = (char *)malloc(count + 1);
	if (!result)
		return (NULL);
	result[count] = '\0';
	i = count - 1;
	ft_populate_digits(i, sign, n, result);
	return (result);
}
