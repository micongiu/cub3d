/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_calloc.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: micongiu <micongiu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/31 16:27:18 by micongiu          #+#    #+#             */
/*   Updated: 2024/05/13 17:48:52 by micongiu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_calloc(size_t n, size_t size)
{
	char	*p;

	if (!n && !size)
		return (NULL);
	p = malloc(size * n);
	if (p == NULL)
		return (NULL);
	ft_bzero(p, size * n);
	return (p);
}
