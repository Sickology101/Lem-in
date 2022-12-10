/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_links.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: parkharo <parkharo@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/12/08 12:52:31 by marius            #+#    #+#             */
/*   Updated: 2022/12/10 17:19:26 by parkharo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "lemin.h"

static int	init_links(t_farm *farm, char **room)
{
	int	index;

	index = 0;
	room[0] = NULL;
	room[1] = NULL;
	farm->links = ft_memalloc(sizeof(int *) * (farm->room_nb + 1));
	if (!(farm->links))
		return (-1);
	while (index < farm->room_nb + 1)
	{
		farm->links[index] = ft_memalloc(sizeof(int) * (farm->room_nb + 1));
		if (!(farm->links[index++]))
			return (-1);
	}
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

static int	get_rooms_name(char *line, int mode, char **r)
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
	*r = room;
	if (!room)
		return (0);
	return (1);
}

int	get_links(t_farm *farm)
{
	int		ret;
	char	*line;
	char	*room[2];
	t_room	*ids[2];

	if (init_links(farm, room) == -1)
		return (-1);
	ret = useless_function(farm, &line);
	while (ret > 0)
	{
		if ((!(line)) || ((get_rooms_name(line, 1, &room[0])) == 0)
			|| (room_exist(farm, room[0], ids, 0) != 1)
			|| ((get_rooms_name(line, 2, &room[1])) == 0)
			|| (room_exist(farm, room[1], ids, 1) != 1))
			return (free_links(line, room, -1));
		save_links(farm, ids);
		free_links(line, room, 0);
		ret = gnl_store(0, &line, farm, 1);
	}
	return (return_check2(ret));
}
