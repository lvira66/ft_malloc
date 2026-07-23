/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_free.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lviravon <marvin@d42.fr>                   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/06 15:43:15 by lviravon          #+#    #+#             */
/*   Updated: 2026/07/06 15:43:15 by lviravon         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/malloc.h"

int find_tiny(t_block *mem)
{
	t_zone *tiny;
	t_block *current;
	t_block *prev;

	tiny = g_zones.tiny;
	while (tiny != NULL)
	{
		current = tiny->block;
		prev = NULL;
		while (current != NULL)
		{
			if (current == mem)
			{
				mem->is_free = 1;
				if (current->next != NULL && current->next->is_free == 1)
				{
					current->size += current->next->size + sizeof(t_block);
					current->next = current->next->next;
				}
				if (prev != NULL && prev->is_free == 1)
				{
					prev->size += current->size + sizeof(t_block);
					prev->next = current->next;
				}
				return (1);
			}
			prev = current;
			current = current->next;
		}
		tiny = tiny->next;
	}
	return (0);
}
int find_small(t_block *mem)
{
	t_zone *small;
	t_block *current;
	t_block *prev;

	small = g_zones.small;
	while (small != NULL)
	{
		current = small->block;
		prev = NULL;
		while (current != NULL)
		{
			if (current == mem)
			{
				mem->is_free = 1;
				if (current->next != NULL && current->next->is_free == 1)
				{
					current->size += current->next->size + sizeof(t_block);
					current->next = current->next->next;
				}
				if (prev != NULL && prev->is_free == 1)
				{
					prev->size += current->size + sizeof(t_block);
					prev->next = current->next;
				}
				return (1);
			}
			prev = current;
			current = current->next;
		}
		small = small->next;
	}
	return (0);
}

int find_large(t_block *mem)
{
	t_zone *current;
	t_zone *prev;

	current = g_zones.large;
	prev = NULL;
	while (current != NULL)
	{
		if (current->block == mem)
		{
			if (prev != NULL)
				prev->next = current->next;
			else
				g_zones.large = current->next;
			munmap(current, current->size);
			return (1);
		}
		prev = current;
		current = current->next;
	}
	return (0);
}

void free(void *ptr_user)
{
	pthread_mutex_lock(&g_zones.mut);
	if (ptr_user == NULL)
	{
		write(2, "free(): invalid pointer\n", 24);
		pthread_mutex_unlock(&g_zones.mut);
		return ;
	}

	t_block *mem;

	mem = ((t_block *)ptr_user) - 1;
	
	if (find_tiny(mem))
	{
		pthread_mutex_unlock(&g_zones.mut);
		return ;
	}
	if (find_small(mem))
	{
		pthread_mutex_unlock(&g_zones.mut);
		return ;
	}
	pthread_mutex_unlock(&g_zones.mut);
	find_large(mem);
}
