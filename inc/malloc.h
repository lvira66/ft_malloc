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
# include <unistd.h>
# include <pthread.h>

# define TINY 16480
# define HEXA_BASE "0123456789ABCDEF"


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

extern malloc_zone g_zones;

size_t	align_16(size_t size);
void	*ft_memcpy(void *dest, const void *src, size_t size);
void	*malloc(size_t size);
void	free(void *ptr_user);
void	*realloc(void *ptr, size_t mem_size);
void	show_alloc_mem(void);
void	ft_putnbr_usigned(unsigned int nb);
void	ft_put_ad(unsigned long nbr, char *base, int Ox);
void	ft_putstr(char *s);
void	ft_putchar(char c);
void	show_alloc_mem(void);

#endif
