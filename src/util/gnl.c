#include "util.h"
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

static char	*gnl_free_join(char *buffer, char *line)
{
	char	*tmp;

	tmp = str_join(line, buffer);
	if (!tmp)
	{
		free(line);
		free(buffer);
		return (NULL);
	}
	free(line);
	free(buffer);
	return (tmp);
}

static int	gnl_has_nl(char *buffer)
{
	int	i;

	i = 0;
	while (buffer[i])
	{
		if (buffer[i] == '\n')
			return (0);
		i++;
	}
	return (1);
}

static char	*gnl_next_line(char *buffer, int i, int s_tmp)
{
	char	*tmp;
	int		len;

	if (!buffer)
		return (NULL);
	len = (int)strlen(buffer);
	while (buffer[i] && buffer[i] != '\n')
		i++;
	if (buffer[i] == '\n')
		i++;
	tmp = calloc((len - i + 1), 1);
	while (buffer[i])
		tmp[s_tmp++] = buffer[i++];
	free(buffer);
	return (tmp);
}

static char	*gnl_line(char *buffer, int i, int mall)
{
	char	*tmp;

	while (buffer[mall] && buffer[mall] != '\n')
		mall++;
	mall += (buffer[mall] == '\n');
	tmp = calloc((mall + 1), 1);
	while (buffer[i] && buffer[i] != '\n')
	{
		tmp[i] = buffer[i];
		i++;
	}
	if (buffer[i] == '\n')
		tmp[i] = '\n';
	return (tmp);
}

static char	*gnl_while_reading(char *buffer, char *line, int fd)
{
	char	*tmp;
	int		byte_count;

	byte_count = 1;
	while (byte_count > 0)
	{
		buffer = calloc((1 + 1), 1);
		if (!buffer)
		{
			free(line);
			return (NULL);
		}
		byte_count = (int)read(fd, buffer, 1);
		if (byte_count < 0)
		{
			free(line);
			free(buffer);
			return (NULL);
		}
		if (byte_count == 0)
		{
			gnl_free_join(buffer, line);
			return (line);
		}
		if (gnl_has_nl(buffer) == 0 || !line)
		{
			tmp = line;
			line = str_join(line, buffer);
			free(tmp);
			free(buffer);
			return (line);
		}
		else
			line = gnl_free_join(buffer, line);
	}
	return (line);
}

static char	*gnl_reading(int fd, char *buffer)
{
	char	*line;

	if (!buffer)
	{
		line = calloc(1, 1);
		if (!line)
			return (NULL);
	}
	else
		line = buffer;
	line = gnl_while_reading(buffer, line, fd);
	if (!line)
		return (NULL);
	return (line);
}

char	*get_next_line(int fd)
{
	static char	*buffer;
	char		*res;

	if (fd < 0)
		return (NULL);
	buffer = gnl_reading(fd, buffer);
	if (!buffer || buffer[0] == '\0')
	{
		free(buffer);
		buffer = NULL;
		return (NULL);
	}
	res = gnl_line(buffer, 0, 0);
	if (!res || res[0] == '\0')
	{
		if (buffer)
			free(buffer);
		buffer = NULL;
		return (res);
	}
	buffer = gnl_next_line(buffer, 0, 0);
	if (!buffer)
	{
		free(res);
		return (NULL);
	}
	return (res);
}
