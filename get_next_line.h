/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.h                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mabusham <mabusham@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/17 16:40:40 by mabusham          #+#    #+#             */
/*   Updated: 2026/02/17 16:40:40 by mabusham         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef GET_NEXT_LINE_H

# define GET_NEXT_LINE_H

# include <stdlib.h>

# include <unistd.h>

# include <stddef.h>

# include <fcntl.h>

# ifndef BUFFER_SIZE
#  define BUFFER_SIZE 42
# endif

char	*get_next_line(int fd);
char	*read_until(int fd, char **remainder, char *buf);
char	*ft_strncpy(char *dest, char *src, size_t n);
char	*ft_strjoin(char *s1, char *s2);
int		ft_strfind(char *str, char c);
char	*ft_strdup(char *s1);

char	*extract_from_remainder(char **remainder, int index, char stopper);
char	*handle_existing_remainder(char **remainder);
char	*append_to_line(char *line, char *buf, int len);
int		process_stopper(char **line, char *buf, char **remainder, int bytes);
size_t	ft_strlen(const char *str);

#endif
