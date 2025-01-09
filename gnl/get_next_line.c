/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: micongiu <micongiu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/14 12:39:03 by micongiu          #+#    #+#             */
/*   Updated: 2024/03/19 15:32:03 by micongiu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line.h"

char	*add_line(char *line, int fd, int i, char *add)
{
	char	buffer[BUFFER_SIZE];

	if (add[0] != '\0')
		line = ft_strdup(add);
	else
	{
		i = read(fd, buffer, BUFFER_SIZE);
		if (i == -1)
			return (NULL);
		buffer[i] = '\0';
		line = ft_strdup(buffer);
	}
	while (!ft_strchr(line, '\n') && i != 0)
	{
		i = read(fd, buffer, BUFFER_SIZE);
		if (i == -1)
			return (NULL);
		buffer[i] = '\0';
		line = ft_strjoin(line, buffer);
	}
	if (i == 0 && line == NULL)
		return (NULL);
	if (i == 0)
		return (line);
	return (line);
}

char	*manage_add(char *line, int i, char *add)
{
	int	a;

	a = 0;
	i = ft_strlen(line, '\n');
	if (line[i] != '\0')
		i++;
	while (line[i])
	{
		add[a] = line[i];
		a++;
		i++;
	}
	add[a] = '\0';
	if (ft_strchr(line, '\n') != NULL)
	{
		i = ft_strlen(line, '\n');
		line = ft_substr(line, 0, i + 1);
	}
	else if (ft_strchr(line, '\0') != NULL)
	{
		add[0] = '\0';
		return (line);
	}
	return (line);
}

char	*get_next_line(int fd)
{
	static char	add[BUFFER_SIZE + 1];
	char		*line;
	int			i;

	if (BUFFER_SIZE < 1 || fd < 0)
		return (NULL);
	i = 1;
	line = NULL;
	line = add_line(line, fd, i, add);
	if (line == NULL)
		return (NULL);
	line = manage_add(line, i, add);
	return (line);
}
