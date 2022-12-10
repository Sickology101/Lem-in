/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   gnl_store.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: parkharo <parkharo@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/12/08 12:52:48 by marius            #+#    #+#             */
/*   Updated: 2022/12/10 16:58:18 by parkharo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "lemin.h"

static int	is_com_start_end(char *line)
{
	if (ft_strcmp(line, "##start") == 0)
		return (-1);
	if (ft_strcmp(line, "##end") == 0)
		return (-1);
	return (0);
}

static int	store(t_farm *farm, char *line, int ret)
{
	t_input	*new;

	if (ret > 0)
	{
		farm->input->line = ft_strdup(line);
		new = ft_memalloc(sizeof(t_input));
		if (!(new))
			return (-1);
		farm->input->next = new;
		farm->input = new;
	}
	return (0);
}

static int	read_from_ants_links(t_farm *farm, char **line, int ret, int fd)
{
	while (ret > 0 && line[0] && line[0][0] == '#')
	{
		if (is_com_start_end(line[0]) == -1 || store(farm, line[0], ret) == -1)
			ret = -1;
		ft_memdel((void *)line);
		if (ret != -1)
			ret = get_next_line(fd, &line[0]);
	}
	return (ret);
}

int	gnl_store(int fd, char **line, t_farm *farm, int origin)
{
	int	ret;

	ret = get_next_line(fd, line);
	if (ret > 0 && line[0] && line[0][0] == '#' && origin == 1)
	{
		ret = read_from_ants_links(farm, &line[0], ret, fd);
		if (ret == -1)
		{
			ft_memdel((void *)line);
			return (-1);
		}
	}
	if (!(store(farm, line[0], ret) == 0))
		return (-1);
	return (ret);
}
