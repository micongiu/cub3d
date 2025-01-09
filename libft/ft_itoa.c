/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_itoa.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: micongiu <micongiu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/31 16:33:34 by micongiu          #+#    #+#             */
/*   Updated: 2024/05/13 17:06:21 by micongiu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include <stdio.h>

void	num(char *str, int a, int len, int n)
{
	len--;
	a = 0;
	if (n < 0)
	{
		a = 1;
		n *= -1;
		str[0] = '-';
	}
	while (len >= a)
	{
		str[len--] = (n % 10) + 48;
		n = n / 10;
	}
}

char	*ft_itoa(int n)
{
	int		len;
	int		a;
	char	*str;

	if (n == -2147483648)
		return (ft_strdup_lib("-2147483648"));
	len = 1;
	a = 1;
	if (n < 0)
		len++;
	while (n / a / 10)
	{
		a *= 10;
		len++;
	}
	str = malloc(sizeof(char) * (len + 1));
	if (str == NULL)
		return (NULL);
	str[len] = '\0';
	num(str, a, len, n);
	return (str);
}
