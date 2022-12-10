/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_room.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: parkharo <parkharo@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/12/08 12:52:37 by marius            #+#    #+#             */
/*   Updated: 2022/12/10 17:19:58 by parkharo         ###   ########.fr       */
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

static int	room_check_syntax(char **line)
{
	int		index;
	char	*temp;

	index = 0;
	temp = *line;
	if (*line[0] != '#')
	{
		while (index <= 3 && temp != NULL)
		{
			temp = ft_strchr(temp, ' ');
			if ((temp != NULL) && temp++)
				index++;
		}
		if (index != 2 || *line[0] == 'L' || room_check_coo(*line) != 0)
		{
			ft_memdel((void *)&(*line));
			return (0);
		}
	}
	if (!(*line))
		return (0);
	return (1);
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

int	get_room(t_room *room, t_farm *farm)
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
			if ((!(line) || (room_check_syntax(&line)) == 0)
				|| (new_room(farm, &room, line, id++)) == 0)
				return (error_free_line(line));
		ft_memdel((void *)&line);
		ret = gnl_store(0, &line, farm, 2);
		if (line && dash_comment(line, 1) != -1)
			return (bad_if_return(&farm, line));
	}
	ft_memdel((void *)&line);
	return (return_check(ret));
}
