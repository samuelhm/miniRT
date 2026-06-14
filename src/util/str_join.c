#include "util.h"
#include <stdlib.h>
#include <string.h>

char	*str_join(char const *s1, char const *s2)
{
	size_t	len1;
	size_t	len2;
	char	*res;

	len1 = strlen(s1);
	len2 = strlen(s2);
	res = calloc(len1 + len2 + 1, sizeof(char));
	if (!res)
		return (NULL);
	memcpy(res, s1, len1);
	memcpy(res + len1, s2, len2);
	return (res);
}
