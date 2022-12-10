/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ant_helper.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: parkharo <parkharo@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/12/08 12:51:58 by marius            #+#    #+#             */
/*   Updated: 2022/12/10 16:53:32 by parkharo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "lemin.h"

/*
** If the first room on the path after start is empty, and
** we have not yet sent out all ants, send a new ant onto the path.
*/

int	send_new_ant(t_farm *farm, int *path, int mov, int *fin)
{
	int		index;

	index = 0;
	if (mov <= farm->ant_nb && farm->id_table[path[1]]->empty == -1)
	{
		++mov;
		farm->id_table[path[1]]->empty = mov;
		if (path[1] == farm->end->id)
		{
			farm->id_table[path[1]]->empty = -1;
			++fin[0];
		}
		ft_printf("L%d-%s", mov, farm->id_table[path[1]]->name);
		++index;
	}
	return (mov);
}

/*
** Check's if it's necessary to print a space
*/

int	check_print_space(int x)
{
	if (x != 0)
		ft_putchar(' ');
	x = 1;
	return (x);
}

int	*get_path_lengths(t_farm *farm, t_path *paths, int *total)
{
	int		index;
	int		*steps;
	t_path	*path;

	index = 0;
	path = paths;
	steps = (int *)ft_memalloc(sizeof(int) * farm->max_paths);
	if (!(steps))
		return (NULL);
	while (index < farm->max_paths)
	{
		steps[index] = path->len;
		total[0] = total[0] + steps[index];
		++index;
		path = path->next;
	}
	return (steps);
}

t_path	*reset_ants(int *x, int *i, t_path *paths)
{
	x[0] = 0;
	i[0] = -1;
	return (paths);
}
