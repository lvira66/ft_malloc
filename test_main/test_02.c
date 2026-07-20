#include "../inc/malloc.h"
#include <unistd.h>
#include <string.h>

void print(char *s)
{
	write(1, s, strlen(s));
}

int	main(void)
{
	void	*tiny1;
	void	*tiny2;
	void	*small1;
	void	*large1;

	tiny1 = malloc(1);
	tiny2 = malloc(2);
	small1 = malloc(666);
	large1 = malloc(2222); // 1 MB pour forcer une LARGE zone

	show_alloc_mem();

	print("\n=======================\n");
	
	free(tiny1);
	free(small1);

	show_alloc_mem();

	free(tiny1);
	tiny2 = realloc(tiny2, 256);

	show_alloc_mem();

	return (0);
}
