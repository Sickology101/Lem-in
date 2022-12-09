/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_links.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marius <marius@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/12/08 12:52:31 by marius            #+#    #+#             */
/*   Updated: 2022/12/09 09:19:29 by marius           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#include "lemin.h"

static int	init_links(t_farm *farm, char **room)
{
	int	index;

	index = 0;
	room[0] = NULL;
	room[1] = NULL;
	if (!(farm->links = ft_memalloc(sizeof(int*) * (farm->room_nb + 1))))
		return (-1);
	while (index < farm->room_nb + 1)
		if (!(farm->links[index++] = ft_memalloc(sizeof(int) * (farm->room_nb + 1))))
			return (-1);
	return (0);
}

static void	save_links(t_farm *farm, t_room **ids)
{
	farm->links[ids[0]->id][ids[1]->id] = 1;
	ids[0]->links_nb++;
	farm->links[ids[1]->id][ids[0]->id] = 1;
	ids[1]->links_nb++;
}

static int	room_exist(t_farm *farm, char *room, t_room **ids, int mode)
{
	int	index;

	index = 0;
	while (index < farm->room_nb)
	{
		if (ft_strcmp(room, farm->id_table[index++]->name) == 0)
		{
			ids[mode] = farm->id_table[index - 1];
			return (1);
		}
	}
	return (0);
}

static char	*get_rooms_name(char *line, int mode)
{
	char	*room;
	int		room_length;

	room_length = 0;
	if (mode == 1)
		while (line[room_length] != '-' && line[room_length] != '\0')
			room_length++;
	else if (mode == 2)
	{
		line = ft_strchr(line, '-') + 1;
		while (line[room_length] != '\0')
			room_length++;
	}
	room = ft_strndup(line, room_length);
	return (room);
}

int			get_links(t_farm *farm)
{
	int		ret;
	char	*line;
	char	*room[2];
	t_room	*ids[2];

	if (init_links(farm, room) == -1)
		return (-1);
	if (farm->line && (ret = 1))
		line = farm->line;
	else
		ret = gnl_store(0, &line, farm, 1);
	while (ret > 0)
	{
		if ((!(line)) || ((room[0] = get_rooms_name(line, 1)) == NULL)	\
		|| (room_exist(farm, room[0], ids, 0) != 1)						\
		|| ((room[1] = get_rooms_name(line, 2)) == NULL)			\
		|| (room_exist(farm, room[1], ids, 1) != 1))
			return (free_links(line, room, -1));
		save_links(farm, ids);
		free_links(line, room, 0);
		ret = gnl_store(0, &line, farm, 1);
	}
	return ((ret >= 0) ? 0 : -1);
}
