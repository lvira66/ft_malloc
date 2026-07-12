/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_malloc.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lviravon <marvin@d42.fr>                   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/06 15:41:42 by lviravon          #+#    #+#             */
/*   Updated: 2026/07/06 15:41:42 by lviravon         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <sys/mman.h>
#include "malloc.h"

void *alloc_tiny()
{
	t_block *ptr;

	ptr = mmap(NULL, 16480, PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANON, -1, 0);
	if (ptr == MAP_FAILED)
		return (NULL);

	ptr->size = 16480;
	ptr->is_free = 0;
	ptr->next = NULL;

	return ((void *)(ptr - 1));
}
