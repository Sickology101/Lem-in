/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: parkharo <parkharo@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/12/08 12:46:36 by marius            #+#    #+#             */
/*   Updated: 2022/12/10 15:49:54 by parkharo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "lemin.h"

static void	write_input(t_farm *farm)
{
	farm->input = farm->input_start;
	while (farm->input->line)
	{
		ft_putstr(farm->input->line);
		ft_putstr("\n");
		farm->input = farm->input->next;
	}
}

int	main(int argc, char **argv)
{
	t_farm	*farm;
	t_room	*room;

	if (argc > 1)
	{
		ft_printf("usage : %s < <map_file> \n", argv[0]);
		return (0);
	}
	farm = ft_memalloc(sizeof(t_farm));
	if (!(farm))
		return (-1);
	room = ft_memalloc(sizeof(t_room));
	if (!(room))
		return (ft_free_lemin(farm, room));
	farm->first_room = room;
	if (read_file(farm, room) == -1)
		return (ft_free_lemin(farm, room));
	write_input(farm);
	ft_putchar('\n');
	solve(farm);
	ft_free_lemin(farm, room);
	return (0);
}
