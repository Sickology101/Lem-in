/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marius <marius@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/12/06 15:55:54 by mangheli          #+#    #+#             */
/*   Updated: 2022/05/24 10:38:28 by marius           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static	char	*read_file(char *savebuf, const int fd, int *error)
{
	int		ret;
	char	*buf;

	buf = malloc(sizeof(char) * (BUFF_SIZE + 1));
	ret = 1;
	while (ret > 0)
	{
		ret = read(fd, buf, BUFF_SIZE);
		buf[ret] = '\0';
		if (savebuf == NULL && buf != NULL)
			savebuf = ft_strjoin(buf, "\0");
		else
			savebuf = ft_strupdate(savebuf, buf);
		if (ft_strchr(buf, '\n'))
			break ;
	}
	ft_strdel(&buf);
	if (ret == -1)
	{
		*error = -1;
		ft_strdel(&savebuf);
	}
	return (savebuf);
}

static	char	*get_line(char *savebuf, char **line, int *error)
{
	int		index;
	char	*temp;
	int		size;

	index = 0;
	size = ft_strlen(savebuf);
	while (savebuf[index] != '\n' && savebuf[index] != '\0')
		index++;
	if (index > 0 && savebuf != NULL)
	{
		ft_strdel(line);
		*line = ft_strsub(savebuf, 0, index);
	}
	if (index + 1 <= size)
	{
		temp = ft_strsub(savebuf, index + 1, size -1);
		ft_strdel(&savebuf);
		savebuf = ft_strupdate(temp, "\0");
		*error = 1;
	}
	if (index + 1 >= size)
		ft_strdel(&savebuf);
	if (index != 0)
		*error = 1;
	return (savebuf);
}

int	get_next_line(const int fd, char **line)
{
	int			error;
	static char	*savebuf[MAX_FD];

	if (line == NULL || fd < 0 || BUFF_SIZE <= 0 || fd > MAX_FD)
		return (-1);
	*line = ft_strdup("");
	error = 0;
	if (savebuf[fd] == NULL || ft_strchr(savebuf[fd], '\n') == NULL)
		savebuf[fd] = read_file(savebuf[fd], fd, &error);
	if (error == -1 || savebuf[fd] == NULL)
		return (error);
	savebuf[fd] = get_line(savebuf[fd], line, &error);
	return (error);
}
