/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line_utils.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: micongiu <micongiu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/14 12:39:13 by micongiu          #+#    #+#             */
/*   Updated: 2024/12/27 11:45:15 by micongiu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line.h"

char	*ft_strjoin(char *s1, char *s2)
{
	size_t	i;
	size_t	j;
	char	*s3;

	i = 0;
	j = 0;
	if (s1 == NULL || s2 == NULL)
		return (NULL);
	s3 = malloc(sizeof(char) * (ft_strlen(s1, '\0') + ft_strlen(s2, '\0') + 1));
	if (s3 == NULL)
		return (NULL);
	while (s1[i] != '\0')
	{
		s3[i] = s1[i];
		i++;
	}
	while (s2[j] != '\0')
	{
		s3[i] = s2[j];
		i++;
		j++;
	}
	s3[i] = '\0';
	free(s1);
	return (s3);
}

char	*ft_strdup(char *src)
{
	char	*copy;
	size_t	i;

	if (!src)
		return (NULL);
	if (src[0] == '\0')
		return (NULL);
	i = 0;
	copy = (char *)malloc(sizeof(char) * (ft_strlen(src, '\0') + 1));
	if (!copy)
		return (NULL);
	while (src[i] != '\0')
	{
		copy[i] = src[i];
		i++;
	}
	copy[i] = '\0';
	return (copy);
}

size_t	ft_strlen(const char *str, char c)
{
	size_t	a;

	a = 0;
	if (str == NULL)
		return (a);
	while (str[a] && str[a] != c)
		a++;
	return (a);
}

char	*ft_strchr(const char *str, int c)
{
	unsigned int	a;

	a = 0;
	if (!str)
		return (NULL);
	while (str[a])
	{
		if (str[a] == (char)c)
			return ((char *)&str[a]);
		a++;
	}
	if (str[a] == (char) c)
		return ((char *)&str[a]);
	return (NULL);
}

char	*ft_substr(char *s, unsigned int start, size_t len)
{
	char	*sub_s;
	size_t	i;

	if (s == NULL)
		return (NULL);
	if (start >= ft_strlen(s, '\0'))
	{
		sub_s = malloc (1);
		*sub_s = '\0';
		return (sub_s);
	}
	if (ft_strlen(s, '\0') < len)
		len = ft_strlen(s, '\0');
	sub_s = malloc (len + 1);
	i = 0;
	if (sub_s == NULL)
		return (NULL);
	while (s[start] != '\0' && i < len)
	{
		sub_s[i++] = s[start];
		start++;
	}
	sub_s[len] = '\0';
	free(s);
	return (sub_s);
}
