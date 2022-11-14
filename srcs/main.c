/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marius <marius@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/26 10:19:50 by marius            #+#    #+#             */
/*   Updated: 2022/11/14 11:31:55 by marius           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "lemin.h"

void	free_input(t_input *input, t_input *start)
{
	t_input	*temp;
	
	input = start;
	while (input->line)
	{
		temp = input;
		ft_memdel((void *)&input->line);
		input = input->next;
		ft_memdel((void *)&temp);
	}
	ft_memdel((void *)&input);
}

int	free_lemin(t_farm *farm, t_room *room)
{
	int	index;
	t_room	*temp;
	
	index = 0;
	while (index <farm->room_nb)
		ft_memdel((void *)&farm->links[index++]);
	while (room)
	{
		temp = room->next;
		ft_memdel((void *)&room->name);
		ft_memdel((void *)&room->links);
		ft_memdel((void *)&room);
		room = temp;
	}
	ft_memdel((void *)&farm->id_table);
	ft_memdel((void *)&farm->links[index]);
	ft_memdel((void *)&farm->links);
	free_input(farm->input, farm->input_start);
	ft_memdel((void *)&farm);
	return (-1);
}

int	main(int argc, char **argv)
{
	t_farm	*farm;
	t_room	*room;
	//int		flag;

	if (argc > 1)
	{
		ft_printf("usage : %s < <map_file> \n", argv[0]);
		return (0);
	}
	if (!(farm = ft_memalloc(sizeof(t_farm))))
		return (-1);
	if (!(room = ft_memalloc(sizeof(t_room))))
		return (free_lemin(farm, room));
	farm->first_room = room;
	if (read_map(farm, room) == -1)
		return (free_lemin(farm, room));
	return (0);
}

/*int	main()
{
	t_head *header;

	header = (t_head *)malloc(sizeof(t_head));
	read_file(header);
	free(header);
	return (0);
}*/
