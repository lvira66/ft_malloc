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

#include "malloc.h"

void *realloc(void *ptr, size_t mem_size)
{
	if (ptr == NULL)
		return (malloc(mem_size));
	else if (mem_size == 0)
	{
		free(ptr);
		return (NULL);
	}

	t_block *meta;
	void *new_alloc;
	size_t size_cpy;

	meta = ((t_block *)ptr) - 1;

	size_cpy = mem_size < meta->size ? mem_size : meta->size;

	new_alloc = malloc(mem_size);
	if (!new_alloc)
		return (NULL);

	ft_memcpy(new_alloc, ptr, size_cpy);
	free(ptr);
	return (new_alloc);
}
