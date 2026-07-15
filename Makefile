ifeq ($(HOSTTYPE),)
HOSTTYPE := $(shell uname -m)_$(shell uname -s)
endif

NAME := libft_malloc_$(HOSTTYPE).so

SRCS := ft_malloc.c \
		ft_free.c \
		utils.c

CFLAGS := -Wall -Werror -Wextra -fPIC

CC := gcc

OBJS := $(SRCS:.c=.o)

all : $(NAME)

$(NAME) : $(OBJS)
	$(CC) -shared -o $(NAME) $(OBJS)
	ln -s $(NAME) libft_malloc.so


$(OBJS) : %.o : %.c	
	$(CC) $(CFLAGS) -c $< -o $@

clean : 
	rm -f $(OBJS)

fclean : clean 
	rm -f $(NAME) libft_malloc.so

re : fclean all

.PHONY : all re clean fclean
