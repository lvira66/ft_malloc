/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_malloc.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lviravon <marvin@d42.fr>                   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/06 15:41:42 by lviravon          #+#    #+#             */
/*   Updated: 2026/07/13 20:30:30 by lviravon         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/malloc.h"

malloc_zone g_zones;

void *alloc_tiny(size_t size, size_t real_size)
{
	t_block *ptr;
	t_block *next;
	t_zone *current;

	ptr = NULL;
	next = NULL;

	if (g_zones.tiny == NULL)
	{
		g_zones.tiny = (t_zone *)mmap(NULL, 16480, PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANON, -1, 0);
		if (g_zones.tiny == MAP_FAILED)
			return (NULL);
		g_zones.tiny->size = 16480;
		g_zones.tiny->next = NULL;
		g_zones.tiny->block = (t_block *)((char *)g_zones.tiny + sizeof(t_zone));

		g_zones.tiny->block->size = 16480 - sizeof(t_zone) - sizeof(t_block);
		g_zones.tiny->block->is_free = 1;
		g_zones.tiny->block->next = NULL;
	}

	current = g_zones.tiny;

	while (current != NULL)
	{
		ptr = current->block;
		while (ptr != NULL)
		{
			if (ptr->is_free == 1 && ptr->size >= (size + sizeof(t_block) + 1))
			{
				size_t old_size = ptr->size;
				ptr->size = real_size;
				ptr->is_free = 0;


				next = (t_block *)((char *)ptr + sizeof(t_block) + size);
				next->size = old_size - size - sizeof(t_block);
				next->is_free = 1;
				next->next = ptr->next;

				ptr->next = next;
				return ((void *)(ptr + 1));
			}
			else if (ptr->is_free == 1 && ptr->size >= size)
			{
				ptr->is_free = 0;
				return ((void *)(ptr + 1));
			}
			ptr = ptr->next;
		}
		current = current->next;
	}

	t_zone *next_zone;

	next_zone = (t_zone *)mmap(NULL, 16480, PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANON, -1, 0);
	if (next_zone == MAP_FAILED)
		return (NULL);
	next_zone->size = 16480;
	next_zone->next = NULL;
	next_zone->block = (t_block *)((char *)next_zone + sizeof(t_zone));

	next_zone->block->size = 16480 - sizeof(t_zone) - sizeof(t_block);
	next_zone->block->is_free = 1;
	next_zone->block->next = NULL;
	next_zone->next = g_zones.tiny->next;

	g_zones.tiny->next = next_zone;
	return (alloc_tiny(size, real_size));
}

void *alloc_small(size_t size, size_t real_size)
{
	t_block *ptr;
	t_block *next;
	t_zone *current;

	ptr = NULL;
	next = NULL;

	if (g_zones.small == NULL)
	{
		g_zones.small = (t_zone *)mmap(NULL, 106496, PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANON, -1, 0);
		if (g_zones.small == MAP_FAILED)
			return (NULL);
		g_zones.small->size = 106496;
		g_zones.small->next = NULL;
		g_zones.small->block = (t_block *)((char *)g_zones.small + sizeof(t_zone));

		g_zones.small->block->size = 106496 - sizeof(t_zone) - sizeof(t_block);
		g_zones.small->block->is_free = 1;
		g_zones.small->block->next = NULL;
	}

	current = g_zones.small;

	while (current != NULL)
	{
		ptr = current->block;
		while (ptr != NULL)
		{
			if (ptr->is_free == 1 && ptr->size >= (size + sizeof(t_block) + 1))
			{
				size_t old_size = ptr->size;
				ptr->size = real_size;
				ptr->is_free = 0;


				next = (t_block *)((char *)ptr + sizeof(t_block) + size);
				next->size = old_size - size - sizeof(t_block);
				next->is_free = 1;
				next->next = ptr->next;

				ptr->next = next;

				return ((void *)(ptr + 1));
			}
			else if (ptr->is_free == 1 && ptr->size >= size)
			{
				ptr->is_free = 0;
				return ((void *)(ptr + 1));
			}
			ptr = ptr->next;
		}
		current = current->next;
	}

	t_zone *next_zone;

	next_zone = (t_zone *)mmap(NULL, 106496, PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANON, -1, 0);
	if (next_zone == MAP_FAILED)
		return (NULL);
	next_zone->size = 106496;
	next_zone->next = NULL;
	next_zone->block = (t_block *)((char *)next_zone + sizeof(t_zone));

	next_zone->block->size = 106496 - sizeof(t_zone) - sizeof(t_block);
	next_zone->block->is_free = 1;
	next_zone->block->next = NULL;
	next_zone->next = g_zones.small->next;

	g_zones.small->next = next_zone;
	return (alloc_small(size, real_size));
}

void *alloc_large(size_t size, size_t real_size)
{
	size_t tot_req;
	size_t final_size;
	t_block *ptr;
	t_zone *new_zone;
	t_zone *current;

	tot_req = (size + sizeof(t_zone) + sizeof(t_block));
	final_size = ((tot_req + 4095) / 4096) * 4096;

	new_zone = NULL;
	new_zone = (t_zone *)mmap(NULL, final_size, PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANON, -1, 0);
	if (new_zone == MAP_FAILED)
		return (NULL);

	new_zone->size = final_size;
	new_zone->next = NULL;
	new_zone->block = (t_block *)((char *)new_zone + sizeof(t_zone));

	new_zone->block->size = real_size; //final_size - sizeof(t_zone) - sizeof(t_block);
	new_zone->block->is_free = 0;
	new_zone->block->next = NULL;

	current = g_zones.large;
	if (g_zones.large == NULL)
	{
		g_zones.large = new_zone;
		ptr = g_zones.large->block;
		return ((void *)(ptr + 1));
	}
	else
	{
		while (current->next != NULL)
		{
			current = current->next;
		}
		current->next = new_zone;
	}

	ptr = new_zone->block;
	return ((void *)(ptr + 1));
}
void *malloc(size_t size)
{
	int adjusted_size;

	adjusted_size = align_16(size);

	if (adjusted_size == 0)
		return (NULL);
	else if (adjusted_size <= 128)
		return (alloc_tiny(adjusted_size, size));
	else if (adjusted_size > 128 && adjusted_size <= 1024)
		return (alloc_small(adjusted_size, size));
	else
		return (alloc_large(adjusted_size, size));
}
