/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   read_file.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marius <marius@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/12/08 12:52:25 by marius            #+#    #+#             */
/*   Updated: 2022/12/08 13:25:56 by marius           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#include "lemin.h"

static int	creates_table(t_room *room, t_farm *farm)
{
	if (farm->start == NULL || farm->end == NULL)
	{
		ft_memdel((void*)&farm->line);
		return (-1);
	}
	while (room->next)
		room = room->next;
	farm->room_nb = room->id + 1;
	if (!(farm->id_table = ft_memalloc(sizeof(t_room*) * (farm->room_nb + 1))))
		return (-1);
	while (room->prev)
	{
		farm->id_table[room->id] = room;
		room = room->prev;
	}
	farm->id_table[room->id] = room;
	return (0);
}

int	get_num(char *str)
{
	long long	n;
	int			index;
	long long	sign;

	n = 0;
	sign = 1;
	index = 0;
	if (str[index] == '-')
	{
		sign = -1;
		index++;
	}
	while (ft_isdigit(str[index]))
	{
		n = n * 10 + str[index] - '0';
		if (n > 2147483647)
			break ;
		index++;
	}
	n = n * sign;
	return (n);
}

static int	get_quantity_ants(t_farm *farm)
{
	int			index;
	char		*line;
	int			ret;
	long long	t;

	index = 0;
	line = NULL;
	if ((ret = gnl_store(0, &line, farm, 1) >= 0) && line)
	{
		while (line[index] != '\0')
		{
			if (ft_isdigit(line[index++]) != 1)
				return (ft_delete_line(line));
		}
		if ((t = get_num(line)) == 0 || t > 2147483647 || t <= 0 \
			|| ft_strlen(line) > 10)
			return (ft_delete_line(line));
		farm->ant_nb = t;
	}
	else
		return (-1);
	ft_memdel((void*)&line);
	return (0);
}

int			create_link_list(t_farm *farm)
{
	int			index1;
	int			index2;
	int			index3;

	index1 = 0;
	index2 = 0;
	index3 = 0;
	while (index1 < farm->room_nb)
	{
		if (!(farm->id_table[index1]->links = ft_memalloc(sizeof(int) * \
		(farm->id_table[index1]->links_nb + 1))))
			return (-1);
		while (index2 < farm->room_nb)
		{
			if (farm->links[index1][index2] == 1)
				farm->id_table[index1]->links[index3++] = index2;
			index2++;
		}
		index3 = 0;
		index2 = 0;
		index1++;
	}
	return (0);
}

int			read_file(t_farm *farm, t_room *room)
{
	t_input		*start;

	if (!(start = ft_memalloc(sizeof(t_input))))
		return (-1);
	farm->input = start;
	farm->input_start = start;
	if (get_quantity_ants(farm) != 0 || get_room(room, farm) != 0 \
		|| creates_table(room, farm) != 0 || get_links(farm) != 0 \
		|| create_link_list(farm) != 0)
	{
		ft_printf("ERROR\n");
		return (-1);
	}
	return (0);
}
