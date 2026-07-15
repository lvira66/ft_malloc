/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   malloc.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lviravon <marvin@d42.fr>                   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/08 16:08:34 by lviravon          #+#    #+#             */
/*   Updated: 2026/07/08 16:08:34 by lviravon         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MALLOC_H
# define MALLOC_H

# include <sys/mman.h>
# include <stdbool.h>
# include <stdio.h>
# include <math.h>

# define TINY 16480

typedef struct s_block
{
	size_t size;
	bool is_free;
	struct s_block *next;
} t_block;

typedef struct s_zone
{
	size_t size;
	t_block *block;
	struct s_zone *next;
} t_zone;

typedef struct s_malloc_zone
{
	t_zone *tiny;
	t_zone *small;
	t_zone *large;
} malloc_zone;

size_t	align_16(size_t size);

#endif
