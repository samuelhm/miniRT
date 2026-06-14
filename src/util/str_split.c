#include "util.h"
#include <stdlib.h>

static unsigned int	count_words(char const *s, char c)
{
	unsigned int	count;
	int			in_word;

	count = 0;
	in_word = 0;
	while (*s)
	{
		if (*s != c && !in_word)
		{
			in_word = 1;
			count++;
		}
		else if (*s == c)
			in_word = 0;
		s++;
	}
	return (count);
}

char	**str_split(char const *s, char c)
{
	char			**split;
	unsigned int	i;
	unsigned int	j;
	unsigned int	start;

	split = calloc((count_words(s, c) + 1), sizeof(char *));
	if (!split)
		return (NULL);
	i = 0;
	j = 0;
	start = 0;
	while (s[i])
	{
		if (i > 0 && s[i] != c && s[i - 1] == c)
			start = i;
		if (s[i] != c && (s[i + 1] == '\0' || s[i + 1] == c))
		{
			split[j] = str_sub(s, start, i - start + 1);
			if (!split[j])
			{
				while (j > 0)
					free(split[--j]);
				free(split);
				return (NULL);
			}
			j++;
		}
		i++;
	}
	split[j] = NULL;
	return (split);
}
