/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_bzero.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yzioual <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/30 11:29:33 by yzioual           #+#    #+#             */
/*   Updated: 2023/10/30 11:33:33 by yzioual          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	ft_bzero(void *str, size_t n)
{
	unsigned char		*holder;
	size_t				i;

	holder = (unsigned char *)str;
	i = 0;
	while (i < n)
	{
		*holder = 0;
		holder++;
		i++;
	}
}
