/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_itoa.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shurtado <shurtado@student.42barcelona.fr> +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/09 16:07:17 by erigonza          #+#    #+#             */
/*   Updated: 2025/01/13 11:21:14 by shurtado         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/libft.h"

static int	count(int n);

static int	exponent(int n);

char	*ft_itoa(int n)
{
	char			*str;
	size_t			i;
	long int		exp;
	long int		copy;

	i = 0;
	exp = exponent(n);
	copy = (long int)n;
	str = calloc((count(n) + 1), sizeof(char));
	if (!str)
		return (NULL);
	if (n < 0)
	{
		copy = -copy;
		str[i++] = '-';
	}
	while (exp > 0)
	{
		str[i++] = (copy / exp) + 48;
		copy = copy % exp;
		exp /= 10;
	}
	str[i] = '\0';
	return (str);
}

static int	exponent(int n)
{
	int		exp;
	int		digits;

	if (n == 0)
		return (1);
	if (n == -2147483648)
		return (1000000000);
	digits = count(n);
	if (n < 0)
	{
		n = -n;
		digits--;
	}
	exp = 1;
	while (--digits)
	{
		exp *= 10;
	}
	return (exp);
}

static int	count(int n)
{
	int		digits;

	digits = 0;
	if (n == 0)
		return (1);
	else if (n == -2147483648)
		return (11);
	else if (n < 0)
	{
		digits++;
		n *= -1;
	}
	while (n > 0)
	{
		n /= 10;
		digits++;
	}
	return (digits);
}
