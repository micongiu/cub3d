/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strtrim.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: micongiu <micongiu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/01 13:56:42 by micongiu          #+#    #+#             */
/*   Updated: 2024/05/13 17:07:07 by micongiu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int	check_char(char c, const char *set)
{
	size_t	i;

	i = 0;
	while (i < ft_strlen_lib(set))
	{
		if (set[i] == c)
			return (1);
		i++;
	}
	return (0);
}

char	*ft_strtrim(const char *s1, const char *set)
{
	size_t	i;
	size_t	j;
	size_t	len;
	char	*s2;

	if (s1 == NULL || set == NULL)
		return (NULL);
	i = 0;
	len = ft_strlen_lib(s1);
	while (check_char(s1[i], set) == 1)
		i++;
	while (len > i && check_char(s1[len - 1], set) == 1)
		len--;
	s2 = malloc(sizeof(char) * (len - i + 1));
	if (s2 == NULL)
		return (NULL);
	j = 0;
	while (i < len)
	{
		s2[j] = s1[i];
		j++;
		i++;
	}
	s2[j] = '\0';
	return (s2);
}
