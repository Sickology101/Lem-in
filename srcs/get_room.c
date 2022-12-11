/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_room.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: parkharo <parkharo@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/12/08 12:52:37 by marius            #+#    #+#             */
/*   Updated: 2022/12/11 14:34:38 by parkharo         ###   ########.fr       */
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

static int	rcs(char **line)
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

int	get_room(t_room *r, t_farm *f)
{
	char	*l;
	int		ret;
	long	id;

	id = 0;
	l = NULL;
	ret = gnl_store(0, &l, f, 2);
	while (ret > 0 && l && dash_comment(l, 0) != -1)
	{
		if (l && l[0] == '#')
		{
			if (check_comment(f, l) == -1)
				return (error_free_line(l));
		}
		else if (l && ft_strchr(l, '-') == NULL)
			if ((!(l) || (rcs(&l)) == 0) || (new_room(f, &r, l, id++)) == 0)
				return (error_free_line(l));
		ft_memdel((void *)&l);
		ret = gnl_store(0, &l, f, 2);
		if (l && dash_comment(l, 1) != -1)
			return (bad_if_return(&f, l));
	}
	if (ret != 0)
		ft_memdel((void *)&l);
	return (return_check(ret));
}
