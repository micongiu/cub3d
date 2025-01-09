/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_split.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: micongiu <micongiu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/01 13:53:54 by micongiu          #+#    #+#             */
/*   Updated: 2024/05/16 11:43:52 by micongiu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include <stdio.h>

int	count_words(const char *s, char c)
{
	int	i;
	int	count;

	i = 0;
	count = 0;
	while (s[i])
	{
		while (s[i] && s[i] == c)
			i ++;
		if (s[i])
			count ++;
		while (s[i] && s[i] != c)
			i ++;
	}
	return (count);
}

static void	free_matrix(char **matrix)
{
	size_t	i;

	i = 0;
	if (matrix == NULL)
		return ;
	while (matrix[i])
	{
		free(matrix[i]);
		i++;
	}
	free(matrix);
	matrix = NULL;
}

char	**ft_split(const char *s, char c)
{
	size_t	k;
	size_t	j;
	int		pos;
	char	**matrix;

	matrix = ft_calloc((count_words(s, c) + 1), sizeof(char *));
	if (!s || !matrix)
		return (free_matrix(matrix), NULL);
	k = -1;
	j = 0;
	pos = -1;
	while (++k <= ft_strlen_lib(s))
	{
		if (s[k] != c && pos < 0)
			pos = k;
		if ((s[k] == c || k == ft_strlen_lib(s)) && pos >= 0)
		{
			matrix[j++] = ft_substr_lib(s, pos, (k - pos));
			pos = -1;
		}
	}
	matrix[j] = NULL;
	return (matrix);
}
