#include "malloc.h"

size_t	align_16(size_t size)
{
	return (((size + 15) / 16) * 16);
}
