#include "../inc/malloc.h"
#include <string.h>

int	main()
{
	char *str;

    str = (char *)malloc(42); 
    if (str)
    {
        strcpy(str, "Hello 42 !");
        write(1, str, 10);
        write(1, "\n", 1);
    }
    
    free(str); 
	return (0);
}
