/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.h                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: micongiu <micongiu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/14 12:41:31 by micongiu          #+#    #+#             */
/*   Updated: 2024/03/19 14:32:20 by micongiu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef GET_NEXT_LINE_H
# define GET_NEXT_LINE_H

# include <stdlib.h>
# include <unistd.h>
# include <fcntl.h>
# include <stdio.h>

# ifndef BUFFER_SIZE
#  define BUFFER_SIZE 1
# endif

char	*ft_strjoin(char *s1, char *s2);
char	*ft_strdup(char *src);
size_t	ft_strlen(const char *str, char c);
char	*ft_strchr(const char *str, int c);
char	*ft_substr(char *s, unsigned int start, size_t len);

char	*check_line(int i, int fd, char *add, char *line);
char	*add_line(char *line, int fd, int i, char *buffer);
char	*manage_add(char *line, int i, char *add);
char	*get_next_line(int fd);

#endif
