/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mabusham <mabusham@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/17 16:39:54 by mabusham          #+#    #+#             */
/*   Updated: 2026/02/17 16:39:54 by mabusham         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line.h"

char	*append_to_line(char *line, char *buf, int len)
{
	char	*tmp;
	char	*substr;

	substr = malloc(len + 1);
	if (!substr)
		return (free(line), NULL);
	ft_strncpy(substr, buf, len);
	substr[len] = '\0';
	if (!line)
		return (substr);
	tmp = ft_strjoin(line, substr);
	free(line);
	free(substr);
	return (tmp);
}

int	process_stopper(char **line, char *buf, char **remainder, int bytes)
{
	int		index;

	index = ft_strfind(buf, '\n');
	if (index == -1)
		return (0);
	*line = append_to_line(*line, buf, index + 1);
	if (!*line)
		return (-1);
	if (bytes - index - 1 > 0)
	{
		free(*remainder);
		*remainder = ft_strdup(buf + index + 1);
		if (!*remainder)
			return (-1);
	}
	else
	{
		free(*remainder);
		*remainder = NULL;
	}
	return (1);
}

char	*handle_existing_remainder(char **remainder)
{
	int		index;
	char	*line;
	char	*tmp;

	if (!*remainder)
		return (NULL);
	index = ft_strfind(*remainder, '\n');
	if (index >= 0)
	{
		line = append_to_line(NULL, *remainder, index + 1);
		tmp = ft_strdup(*remainder + index + 1);
		free(*remainder);
		*remainder = tmp;
		return (line);
	}
	line = ft_strdup(*remainder);
	free(*remainder);
	*remainder = NULL;
	return (line);
}

char	*read_until(int fd, char **remainder, char *buf)
{
	char	*line;
	int		bytes;
	int		res;

	line = handle_existing_remainder(remainder);
	if (line && ft_strfind(line, '\n') >= 0)
		return (line);
	while (1)
	{
		bytes = read(fd, buf, BUFFER_SIZE);
		if (bytes < 0)
			return (free(line), NULL);
		if (bytes == 0)
			break ;
		buf[bytes] = '\0';
		res = process_stopper(&line, buf, remainder, bytes);
		if (res == -1)
			return (free(line), NULL);
		if (res == 1)
			break ;
		line = append_to_line(line, buf, bytes);
	}
	if (line && line[0] != '\0')
		return (line);
	return (free(line), NULL);
}

char	*get_next_line(int fd)
{
	static char	*remainders[4096];
	char		*line;
	char		*buf;

	if (fd < 0 || BUFFER_SIZE <= 0)
		return (NULL);
	buf = malloc(BUFFER_SIZE + 1);
	if (!buf)
	{
		free(remainders[fd]);
		remainders[fd] = NULL;
		return (NULL);
	}
	line = read_until(fd, &remainders[fd], buf);
	if (line == NULL)
	{
		free(remainders[fd]);
		remainders[fd] = NULL;
	}
	free(buf);
	return (line);
}
