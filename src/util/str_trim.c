#include "util.h"
#include <stdlib.h>
#include <string.h>

char	*str_trim(char const *s1, char const *set)
{
	size_t	end;
	size_t	start;

	if (!s1 || !set)
		return (NULL);
	start = 0;
	while (s1[start] && strchr(set, s1[start]))
		start++;
	end = strlen(s1);
	while (end > start && strchr(set, s1[end - 1]))
		end--;
	return (str_sub(s1, (unsigned int)start, end - start));
}
