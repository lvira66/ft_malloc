/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   show_alloc_mem.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lviravon <marvin@d42.fr>                   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/15 18:22:18 by lviravon          #+#    #+#             */
/*   Updated: 2026/07/15 18:22:18 by lviravon         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/malloc.h"

int aff_tiny(void)
{
	int tot = 0;
	ft_putstr("TINY : ");
	ft_put_ad((unsigned long)g_zones.tiny, HEXA_BASE, 0);
	ft_putchar('\n');

	t_zone *current;
	t_block *curr_block;
	current = g_zones.tiny;

	while (current != NULL)
	{
		curr_block = current->block;
		while (curr_block != NULL)
		{
			if (curr_block->is_free == 0)
			{
				ft_put_ad((unsigned long)(curr_block + 1), HEXA_BASE, 0);
				ft_putstr(" - ");
				// ft_put_ad((unsigned long)(curr_block + curr_block->size), HEXA_BASE, 0);
				ft_put_ad((unsigned long)((char *)(curr_block + 1) + curr_block->size), HEXA_BASE, 0);
				ft_putstr(" : ");
				ft_putnbr_usigned(curr_block->size);
				tot += curr_block->size;
				ft_putstr(" bytes\n");
			}
			curr_block = curr_block->next;
		}
		current = current->next;
	}
	return (tot);
}
int aff_small(void)
{
	int tot = 0;
	ft_putstr("SMALL : ");
	ft_put_ad((unsigned long)g_zones.small, HEXA_BASE, 0);
	ft_putchar('\n');

	t_zone *current;
	t_block *curr_block;
	current = g_zones.small;

	while (current != NULL)
	{
		curr_block = current->block;
		while (curr_block != NULL)
		{
			if (curr_block->is_free == 0)
			{
				ft_put_ad((unsigned long)(curr_block + 1), HEXA_BASE, 0);
				ft_putstr(" - ");
				// ft_put_ad((unsigned long)curr_block->next, HEXA_BASE, 0);
				// ft_put_ad((unsigned long)(curr_block + curr_block->size), HEXA_BASE, 0);
				ft_put_ad((unsigned long)((char *)(curr_block + 1) + curr_block->size), HEXA_BASE, 0);
				ft_putstr(" : ");
				ft_putnbr_usigned(curr_block->size);
				tot += curr_block->size;
				ft_putstr(" bytes\n");
			}
			curr_block = curr_block->next;
		}
		current = current->next;
	}
	return (tot);
}

int aff_large(void)
{
	int tot = 0;
	ft_putstr("LARGE : ");
	ft_put_ad((unsigned long)g_zones.large, HEXA_BASE, 0);
	ft_putchar('\n');

	t_zone *current;
	t_block *curr_block;
	current = g_zones.large;

	while (current != NULL)
	{
		curr_block = current->block;
		if (curr_block->is_free == 0)
		{
			ft_put_ad((unsigned long)(curr_block + 1), HEXA_BASE, 0);
			ft_putstr(" - ");
			ft_put_ad((unsigned long)((char *)(curr_block + 1) + curr_block->size), HEXA_BASE, 0);
			ft_putstr(" : ");
			ft_putnbr_usigned(curr_block->size);
			tot += curr_block->size;
			ft_putstr(" bytes\n");
		}
		curr_block = curr_block->next;
		current = current->next;
	}
	return (tot);
}

void show_alloc_mem(void)
{
	int tot = 0;

	if (g_zones.tiny)
		tot += aff_tiny();
	else
		ft_putstr("TINY block dosn't exist.\n");
	if (g_zones.small)
		tot += aff_small();
	else
		ft_putstr("SMALL block dosn't exist.\n");
	if (g_zones.large)
		tot += aff_large();
	else
		ft_putstr("LARGE block dosn't exist.\n");

	ft_putstr("Total : ");
	ft_putnbr_usigned(tot);
	ft_putstr(" bytes\n");
}
