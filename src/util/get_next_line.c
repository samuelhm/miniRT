#include <stdlib.h>
#include <string.h>
#include <unistd.h>

char	*str_join(char const *s1, char const *s2);

static char	*extract_line(char *buf, size_t *pos)
{
	char	*line;
	size_t	start;
	size_t	len;

	start = *pos;
	while (buf[*pos] && buf[*pos] != '\n')
		(*pos)++;
	if (buf[*pos] == '\n')
		(*pos)++;
	len = *pos - start;
	line = calloc(len + 1, sizeof(char));
	if (!line)
		return (NULL);
	memcpy(line, buf + start, len);
	return (line);
}

static char	*append_chunk(char *line, char *chunk, int nread)
{
	char	*tmp;

	if (nread <= 0)
		return (line);
	chunk[nread] = '\0';
	tmp = str_join(line, chunk);
	free(line);
	return (tmp);
}

static int	contains_newline(char *line)
{
	while (*line)
	{
		if (*line == '\n')
			return (1);
		line++;
	}
	return (0);
}

static char	*read_lines(int fd, char *line, char *chunk, size_t *pos)
{
	int		nread;

	while (!contains_newline(line))
	{
		nread = read(fd, chunk, 128);
		if (nread <= 0)
			return (line);
		chunk[nread] = '\0';
		{
			char	*tmp = str_join(line, chunk);
			free(line);
			line = tmp;
		}
	}
	(void)pos;
	return (line);
}

char	*get_next_line(int fd)
{
	static char	buf[65536];
	static size_t	pos;
	static ssize_t	total;
	char		*line;

	if (fd < 0)
		return (NULL);
	if (pos >= (size_t)total)
	{
		pos = 0;
		total = read(fd, buf, sizeof(buf) - 1);
		if (total <= 0)
			return (NULL);
		buf[total] = '\0';
	}
	if (pos == 0)
	{
		line = calloc(1, sizeof(char));
		if (!line)
			return (NULL);
	}
	else
	{
		line = calloc(1, sizeof(char));
		if (!line)
			return (NULL);
	}
	pos = 0;
	total = read(fd, buf, sizeof(buf) - 1);
	if (total <= 0)
	{
		free(line);
		return (NULL);
	}
	buf[total] = '\0';
	line = read_lines(fd, line, buf, &pos);
	{
		char	*result = extract_line(buf, &pos);
		free(line);
		return (result);
	}
}
