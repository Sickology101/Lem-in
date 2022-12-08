/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: parkharo <parkharo@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/12/08 12:46:36 by marius            #+#    #+#             */
/*   Updated: 2022/12/08 22:05:25 by parkharo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "lemin.h"

static void	write_input(t_farm *f)
{
	f->input = f->input_start;
	while (f->input->line)
	{
		ft_putstr(f->input->line);
		ft_putstr("\n");
		f->input = f->input->next;
	}
}

int			main(int argc, char **argv)
{
	t_farm	*farm;
	t_room	*room;

	if (argc > 1)
	{
		ft_printf("usage : %s < <map_file> \n", argv[0]);
		return (0);
	}
	if (!(farm = ft_memalloc(sizeof(t_farm))))
		return (-1);
	if (!(room = ft_memalloc(sizeof(t_room))))
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
