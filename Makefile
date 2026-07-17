ifeq ($(HOSTTYPE),)
HOSTTYPE := $(shell uname -m)_$(shell uname -s)
endif

NAME := libft_malloc_$(HOSTTYPE).so

SRCS := srcs/ft_malloc.c \
		srcs/ft_free.c \
		srcs/ft_realloc.c\
		srcs/show_alloc_mem.c \
		srcs/utils.c

CFLAGS := -Wall -Werror -Wextra -fPIC

CC := gcc

OBJS := $(SRCS:srcs/%.c=objs/%.o)

all : $(NAME)

$(NAME) : $(OBJS)
	$(CC) -shared -o $(NAME) $(OBJS)
	ln -sf $(NAME) libft_malloc.so


$(OBJS) : objs/%.o : srcs/%.c
	@mkdir -p objs
	$(CC) $(CFLAGS) -c $< -o $@

clean : 
	rm -rf objs

fclean : clean 
	rm -f $(NAME) libft_malloc.so

re : fclean all

.PHONY : all re clean fclean
