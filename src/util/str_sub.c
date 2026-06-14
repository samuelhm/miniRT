#include "util.h"
#include <stdlib.h>
#include <string.h>

char	*str_sub(char const *s, unsigned int start, size_t len)
{
	char	*res;
	size_t	len_s;

	if (!s)
		return (NULL);
	len_s = strlen(s);
	if (start > len_s)
		return (strdup(""));
	len_s -= start;
	if (len_s < len)
		len = len_s;
	res = calloc((len + 1), sizeof(char));
	if (!res)
		return (NULL);
	memcpy(res, s + start, len);
	return (res);
}
