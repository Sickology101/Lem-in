/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   send_ants.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marius <marius@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/12/08 12:53:27 by marius            #+#    #+#             */
/*   Updated: 2022/12/09 10:10:37 by marius           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#include "lemin.h"

static int	find_last_ant(t_farm *farm, int *path)
{
	int	index;

	index = 0;
	while (path[index] != farm->end->id)
		++index;
	while (index != 0 && farm->id_table[path[index]]->empty == -1)
		--index;
	return (index + 1);
}

static int	reach_finish(int *path, t_farm *farm, int index, int *x)
{
	if (path[index] == farm->end->id)
	{
		x[0] = check_print_space(x[0]);
		ft_printf("L%d-%s", farm->id_table[path[index - 1]]->empty,
			farm->id_table[path[index]]->name);
		if (index >= 1)
			farm->id_table[path[index - 1]]->empty = -1;
		return (1);
	}
	return (0);
}

static void	move_the_ant(int *path, t_farm *farm, int index, int *x)
{
	if (farm->id_table[path[index]]->empty == -1 \
		&& index > 1 && farm->id_table[path[index - 1]]->empty != -1)
	{
		x[0] = check_print_space(x[0]);
		farm->id_table[path[index]]->empty = farm->id_table[path[index - 1]]->empty;
		farm->id_table[path[index - 1]]->empty = -1;
		ft_printf("L%d-%s", farm->id_table[path[index]]->empty,
			farm->id_table[path[index]]->name);
	}
}

static void	move_ants_on_path(t_farm *farm, int *path, int *finished_ants, int *x)
{
	int		index;

	index = find_last_ant(farm, path);
	if (path[0] == farm->start->id && path[1] == farm->end->id)
		return ;
	while (index != 0)
	{
		if ((reach_finish(path, farm, index, x)) == 1)
			++finished_ants[0];
		else if (finished_ants[0] < farm->ant_nb)
			move_the_ant(path, farm, index, x);
		--index;
	}
}

int			send_ants(t_farm *farm, t_path *paths, int mv_ants, int x)
{
	int		finished_ants;
	int		index1;
	int		index2;
	t_path	*path;

	mv_ants = 0;
	index2 = 0;
	finished_ants = 0;
	while (finished_ants < farm->ant_nb && (++index2 || 1))
	{
		path = reset_ants(&x, &index1, paths);
		while (++index1 < paths->max)
		{
			move_ants_on_path(farm, path->path, &finished_ants, &x);
			if (mv_ants < farm->ant_nb && paths->division[index1] > 0)
			{
				x = check_print_space(x);
				mv_ants = send_new_ant(farm, path->path, mv_ants, &finished_ants);
				--paths->division[index1];
			}
			path = path->next;
		}
		ft_putchar('\n');
	}
	return (index2);
}
