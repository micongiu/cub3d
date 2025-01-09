/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strlcat.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: micongiu <micongiu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/01 13:55:09 by micongiu          #+#    #+#             */
/*   Updated: 2024/05/13 17:05:18 by micongiu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

size_t	ft_strlcat(char *dest, const char *src, size_t size)
{
	size_t	s_dest;
	size_t	b;
	size_t	s_src;

	if (!src && !dest)
		return (0);
	if (size == 0)
		return (ft_strlen_lib(src));
	s_dest = ft_strlen_lib(dest);
	b = 0;
	s_src = ft_strlen_lib(src);
	if (size < s_dest)
		return (s_src + size);
	while (src[b] != '\0' && s_dest + 1 < size)
	{
		dest[s_dest] = src[b];
		s_dest++;
		b++;
	}
	dest[s_dest] = '\0';
	return (ft_strlen_lib(&src[b]) + s_dest);
}
