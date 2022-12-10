/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   save_path.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: parkharo <parkharo@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/12/08 12:53:22 by marius            #+#    #+#             */
/*   Updated: 2022/12/10 15:56:54 by parkharo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "lemin.h"

static size_t	count_steps(t_queue *queue, int start, int end)
{
	int	steps;

	steps = 0;
	while (end != start)
	{
		end = queue->prev[end];
		++steps;
	}
	return (steps);
}

static int	*rev_path(t_farm *farm, t_queue *queue)
{
	int		*rev_path;
	int		steps;
	int		index;
	int		pos;

	index = 0;
	pos = farm->end->id;
	steps = count_steps(queue, farm->start->id, farm->end->id);
	rev_path = ft_memalloc((sizeof(int)) * (steps + 1));
	if (!(rev_path))
		return (NULL);
	rev_path[steps] = pos;
	while (index <= steps)
	{
		rev_path[steps - index] = pos;
		pos = queue->prev[pos];
		++index;
	}
	return (rev_path);
}

static void	mark_path(t_farm *farm, t_queue *queue)
{
	int		path;
	int		index;

	index = 0;
	path = queue->prev[farm->end->id];
	while (path != farm->start->id)
	{
		queue->visited[path] = 2;
		path = queue->prev[path];
	}
	while (index < queue->length)
	{
		queue->prev[index] = -1;
		queue->queue[index] = -1;
		if (queue->visited[index] == 1)
			queue->visited[index] = 0;
		++index;
	}
}

static t_path	**path_error(t_path **path)
{
	(*path)->len = -1;
	return (path);
}

t_path	**save_paths(t_queue *queue, t_farm *farm, t_path **path_list)
{
	int		*path;
	size_t	steps;
	t_path	*new;
	int		index;

	index = 0;
	set_weights(farm);
	while (breadth_first_search(farm, queue) == 0)
	{
		path = rev_path(farm, queue);
		if (!(path))
			return (path_error(path_list));
		steps = count_steps(queue, farm->start->id, farm->end->id);
		mark_path(farm, queue);
		new = ft_new_path(path, steps + 1);
		if (!(new))
			return (path_error(path_list));
		ft_memdel((void *)&path);
		ft_add_path(*path_list, new);
		++index;
	}
	path_list = set_path(path_list, index, farm);
	return (path_list);
}
