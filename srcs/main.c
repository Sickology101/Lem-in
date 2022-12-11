/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: parkharo <parkharo@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/12/08 12:46:36 by marius            #+#    #+#             */
/*   Updated: 2022/12/11 16:12:43 by parkharo         ###   ########.fr       */
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

static int	read_flag(int argc, char **av)
{
	int	index;

	index = 0;
	if (argc > 1)
	{
		while (++index < argc)
		{
			if (ft_strcmp(av[index], "-l") == 0)
				return (1);
			else
			{
				ft_printf("usage: %s < <map> [-l flag for \
					number of moves]\n", av[0]);
				return (-1);
			}
		}
	}
	return (0);
}

int	main(int argc, char **argv)
{
	t_farm	*farm;
	t_room	*room;
	int		flag;

	flag = read_flag(argc, argv);
	if (flag == -1)
		return (0);
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
	solve(farm, flag);
	ft_free_lemin(farm, room);
	sleep (3);
	return (0);
}
