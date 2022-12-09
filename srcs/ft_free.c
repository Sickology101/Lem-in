/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_free.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marius <marius@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/12/08 12:52:18 by marius            #+#    #+#             */
/*   Updated: 2022/12/09 08:42:03 by marius           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#include "lemin.h"

int		ft_delete_line(char *line)
{
	ft_memdel((void *)&line);
	return (-1);
}

int		free_links(char *line, char **rooms, int ret)
{
	ft_memdel((void*)&rooms[0]);
	ft_memdel((void*)&rooms[1]);
	ft_memdel((void*)&line);
	return (ret);
}

void	ft_free_input(t_input *input, t_input *start)
{
	t_input *tmp;

	input = start;
	while (input->line)
	{
		tmp = input;
		ft_memdel((void*)&input->line);
		input = input->next;
		ft_memdel((void*)&tmp);
	}
	ft_memdel((void*)&input);
}

int		ft_free_lemin(t_farm *farm, t_room *room)
{
	int		index;
	t_room	*temp;

	index = 0;
	while (index < farm->room_nb)
		ft_memdel((void*)&farm->links[index++]);
	while (room)
	{
		temp = room->next;
		ft_memdel((void*)&room->name);
		ft_memdel((void*)&room->links);
		ft_memdel((void*)&room);
		room = temp;
	}
	ft_memdel((void*)&farm->id_table);
	ft_memdel((void*)&farm->links[index]);
	ft_memdel((void*)&farm->links);
	ft_free_input(farm->input, farm->input_start);
	ft_memdel((void*)&farm);
	return (-1);
}

int		error_free_line(char *line)
{
	ft_memdel((void*)&line);
	return (-1);
}
