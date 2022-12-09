/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   new_room.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marius <marius@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/12/08 12:52:56 by marius            #+#    #+#             */
/*   Updated: 2022/12/09 09:33:32 by marius           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#include "lemin.h"

static int	room_exist(char *room, t_farm *farm)
{
	t_room *temp;

	temp = farm->first_room;
	while (temp && temp->next != NULL)
	{
		if (room[0] == temp->name[0])
			if (ft_strcmp(room, temp->name) == 0)
				return (-1);
		temp = temp->next;
	}
	return (0);
}

static int	init_room(t_farm *farm, t_room *room, char *line, int id)
{
	int		name_size;

	name_size = 0;
	while (line[name_size] != ' ')
		name_size++;
	if (!(room->name = ft_strndup(line, name_size)))
		return (-1);
	if (room_exist(room->name, farm) == -1)
		return (-1);
	room->id = id;
	room->empty = -1;
	if (farm->flags & 1 && farm->start == NULL)
	{
		farm->start = room;
		farm->flags &= ~2;
		farm->flags &= ~1;
	}
	else if (farm->flags & 2 && farm->end == NULL)
	{
		farm->end = room;
		farm->flags &= ~1;
		farm->flags &= ~2;
	}
	return (0);
}

t_room		*new_room(t_farm *farm, t_room *room, char *line, long id)
{
	t_room *new;
	t_room *temp;

	temp = room;
	if (id > 2147483647)
		return (NULL);
	if (room->name != NULL)
	{
		if (!(new = ft_memalloc(sizeof(t_room))))
			return (NULL);
		room->next = new;
		room = new;
		room->prev = temp;
	}
	if (init_room(farm, room, line, id) == -1)
		return (NULL);
	return (room);
}
