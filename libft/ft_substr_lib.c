/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_substr_lib.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: micongiu <micongiu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/02 15:02:07 by micongiu          #+#    #+#             */
/*   Updated: 2024/05/13 17:05:41 by micongiu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_substr_lib(char const *s, unsigned int start, size_t len)
{
	char	*sub_s;
	size_t	i;

	if (s == NULL)
		return (NULL);
	if (start >= ft_strlen_lib(s))
	{
		sub_s = ft_calloc (1, sizeof(char *));
		*sub_s = '\0';
		return (sub_s);
	}
	if (ft_strlen_lib(s) < len)
		len = ft_strlen_lib(s);
	sub_s = ft_calloc((len + 1), sizeof(char *));
	i = 0;
	if (sub_s == NULL)
		return (NULL);
	while (s[start] != '\0' && i < len)
	{
		sub_s[i] = s[start];
		i++;
		start++;
	}
	sub_s[len] = '\0';
	return (sub_s);
}
