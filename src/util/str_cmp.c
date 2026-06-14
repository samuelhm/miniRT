#include "util.h"

int	str_cmp(const char *s1, const char *s2)
{
	unsigned int	i;
	unsigned int	j;

	i = 0;
	j = 0;
	while (s1[i] && s1[i] == s2[i])
	{
		i++;
		if (!s2[i])
			return (-1);
	}
	while (s2[j] && s1[j] == s2[j])
	{
		j++;
		if (!s1[j])
			return (-1);
	}
	return ((int)(i - j));
}
