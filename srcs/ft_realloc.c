/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_realloc.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lviravon <marvin@d42.fr>                   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/15 14:54:53 by lviravon          #+#    #+#             */
/*   Updated: 2026/07/15 14:54:53 by lviravon         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/malloc.h"

static int ptr_exist_tiny(void *ptr)
{
	t_block *mem;

	mem = ((t_block *)ptr) - 1;

	t_zone *tiny;
	t_block *current;

	tiny = g_zones.tiny;
	while (tiny != NULL)
	{
		current = tiny->block;
		while (current != NULL)
		{
			if (current == mem)
				return (1);
			current = current->next;
		}
		tiny = tiny->next;
	}
	return (0);
}

static int ptr_exist_small(void *ptr)
{
	t_block *mem;

	mem = ((t_block *)ptr) - 1;

	t_zone *small;
	t_block *current;

	small = g_zones.small;
	while (small != NULL)
	{
		current = small->block;
		while (current != NULL)
		{
			if (current == mem)
				return (1);
			current = current->next;
		}
		small = small->next;
	}
	return (0);
}

static int ptr_exist_large(void *ptr)
{
	t_zone *current;
	t_block *mem;

	mem = ((t_block *)ptr) - 1;
	current = g_zones.large;
	while (current != NULL)
	{
		if (current->block == mem)
			return (1);
		current = current->next;
	}
	return (0);
}

static int is_exist(void *ptr)
{
	if (ptr_exist_tiny(ptr) == 1)
		return (1);
	if (ptr_exist_small(ptr) == 1)
		return (1);
	if (ptr_exist_large(ptr) == 1)
		return (1);
	return (0);
}

void *realloc(void *ptr, size_t mem_size)
{
	pthread_mutex_lock(&g_zones.mut);
	if (ptr == NULL)
	{
		pthread_mutex_unlock(&g_zones.mut);
		return (malloc(mem_size));
	}
	else if (mem_size == 0)
	{
		free(ptr);
		pthread_mutex_unlock(&g_zones.mut);
		return (NULL);
	}
	if (is_exist(ptr) == 0)
	{
		write(2, "realloc(): invalid pointer\n", 27);
		pthread_mutex_unlock(&g_zones.mut);
		return (NULL);
	}

	t_block *meta;
	void *new_alloc;
	size_t size_cpy;

	meta = ((t_block *)ptr) - 1;

	size_cpy = mem_size < meta->size ? mem_size : meta->size;

	new_alloc = malloc(mem_size);
	if (!new_alloc)
	{
		pthread_mutex_unlock(&g_zones.mut);
		return (NULL);
	}

	ft_memcpy(new_alloc, ptr, size_cpy);
	free(ptr);
	pthread_mutex_unlock(&g_zones.mut);
	return (new_alloc);
}
