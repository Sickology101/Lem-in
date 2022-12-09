/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_room.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marius <marius@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/12/08 12:52:37 by marius            #+#    #+#             */
/*   Updated: 2022/12/09 09:25:11 by marius           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#include "lemin.h"

static int	room_check_coo(char *line)
{
	char	*temp;
	int		index;

	index = 0;
	temp = ft_strchr(line, ' ') + 1;
	while (temp[index] != ' ' && temp[index] != '\0')
		if (ft_isdigit(temp[index++]) != 1)
			return (-1);
	if (temp[index++] != ' ')
		return (-1);
	while (temp[index] != '\0' && index <= 20)
		if (ft_isdigit(temp[index++]) != 1)
			return (-1);
	return (0);
}

static char	*room_check_syntax(char *line)
{
	int		index;
	char	*temp;

	index = 0;
	temp = line;
	if (line[0] != '#')
	{
		while (index <= 3 && temp != NULL)
			if (((temp = ft_strchr(temp, ' ')) != NULL) && temp++)
				index++;
		if (index != 2 || line[0] == 'L' || room_check_coo(line) != 0)
		{
			ft_memdel((void*)&line);
			return (NULL);
		}
	}
	return (line);
}

static int	check_comment(t_farm *farm, char *line)
{
	if (ft_strcmp(line, "##start") == 0)
	{
		if (farm->start != NULL)
			return (-1);
		else
		{
			farm->flags |= 1;
			farm->flags &= ~2;
		}
	}
	else if (ft_strcmp(line, "##end") == 0)
	{
		if (farm->end != NULL)
			return (-1);
		else
		{
			farm->flags |= 2;
			farm->flags &= ~1;
		}
	}
	return (0);
}

static int	dash_comment(char *line, int mode)
{
	if (mode == 0)
	{
		if ((ft_strchr(line, '-') == NULL) || line[0] == '#')
			return (0);
		else
			return (-1);
	}
	else if (mode == 1)
	{
		if ((ft_strchr(line, '-') != NULL && line[0] != '#'))
			return (0);
		else
			return (-1);
	}
	return (0);
}

int			get_room(t_room *room, t_farm *farm)
{
	char	*line;
	int		ret;
	long	id;

	id = 0;
	line = NULL;
	ret = gnl_store(0, &line, farm, 2);
	while (ret > 0 && line && dash_comment(line, 0) != -1)
	{
		if (line && line[0] == '#')
		{
			if (check_comment(farm, line) == -1)
				return (error_free_line(line));
		}
		else if (line && ft_strchr(line, '-') == NULL)
			if ((!(line) || (line = room_check_syntax(line)) == NULL) 	\
			|| (room = new_room(farm, room, line, id++)) == NULL)
				return (error_free_line(line));
		ft_memdel((void*)&line);
		ret = gnl_store(0, &line, farm, 2);
		if (line && dash_comment(line, 1) != -1 && ((farm->line = line)))
			return (0);
	}
	ft_memdel((void*)&line);
	return ((ret > 0) ? 0 : -1);
}
