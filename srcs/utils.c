/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lviravon <marvin@d42.fr>                   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/15 12:37:26 by lviravon          #+#    #+#             */
/*   Updated: 2026/07/15 12:37:26 by lviravon         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/malloc.h"

size_t	align_16(size_t size)
{
	return (((size + 15) / 16) * 16);
}

static int ft_strlen(char *str)
{
	int i;

	i = 0;
	while (str[i])
		i++;
	return (i);
}

void ft_putchar(char c)
{
	write(1, &c, 1);
}

void	*ft_memcpy(void *dest, const void *src, size_t size)
{
	size_t	i;

	if (src == NULL && dest == NULL)
		return (NULL);
	i = 0;
	while (i != size)
	{
		((char *)dest)[i] = ((char *)src)[i];
		i++;
	}
	return (dest);
}

void	ft_putstr(char *s)
{
	int	i;

	i = 0;
	if (!s)
	{
		write(1, "(null)", 6);
		return ;
	}
	while (*s)
	{
		write(1, s++, 1);
		i++;
	}
}

void	ft_put_ad(unsigned long nbr, char *base, int Ox)
{
	if (nbr == 0)
	{
		write(1, "(nil)", 5);
		return ;
	}
	if (Ox == 0)
	{
		write(1, "0x", 2);
		Ox++;
	}
	if (nbr / ft_strlen(base) != 0)
	{
		ft_put_ad(nbr / 16, base, Ox);
	}
	ft_putchar(base[nbr % 16]);
}

void	ft_putnbr_usigned(unsigned int nb)
{
	if (nb / 10 != 0)
		ft_putnbr_usigned(nb / 10);
	ft_putchar(nb % 10 + '0');
}
