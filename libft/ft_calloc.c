/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_calloc.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yzioual <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/30 13:10:23 by yzioual           #+#    #+#             */
/*   Updated: 2023/11/06 14:38:34 by yzioual          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_calloc(size_t num_elements, size_t element_size)
{
	size_t	total_size;
	void	*block;

	if (num_elements == 0 || element_size == 0)
		return (malloc(0));
	total_size = num_elements * element_size;
	if (element_size != 0 && total_size / element_size != num_elements)
		return (NULL);
	block = malloc(total_size);
	if (block == NULL)
		return (NULL);
	ft_memset(block, 0, total_size);
	return (block);
}
