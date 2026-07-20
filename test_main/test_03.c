#include "../inc/malloc.h"
#include <unistd.h>
#include <stdio.h>

int	main(void)
{
	void	*tiny1;
	void	*tiny2;
	void	*small1;
	void	*large1;

	tiny1 = malloc(1);
	tiny2 = malloc(2);
	small1 = malloc(666);
	large1 = malloc(2222);

	show_alloc_mem();


	return (0);
}
