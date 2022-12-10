/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   norm_stuff.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: parkharo <parkharo@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/12/10 17:17:57 by parkharo          #+#    #+#             */
/*   Updated: 2022/12/10 18:06:39 by parkharo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "lemin.h"

int	malloc_q(t_queue *queue)
{
	queue->queue = ft_memalloc(sizeof(int) * queue->length);
	queue->prev = ft_memalloc(sizeof(int) * queue->length);
	queue->visited = ft_memalloc(sizeof(int) * queue->length);
	queue->flow = ft_memalloc(sizeof(int *) * queue->length);
	if (!(queue->queue) || !(queue->prev) || !(queue->visited)
		|| !(queue->flow))
		return (0);
	return (1);
}

void	new_function(t_queue *queue, t_farm *farm, t_path **new)
{
	save_flow(queue, farm);
	set_to_n(&queue->visited, queue->length, 0);
	reset_queue(queue, farm->start->id, farm->end->id);
	save_paths(queue, farm, new);
}

int	return_check3(int t)
{
	if (t == 1)
		return (0);
	return (-1);
}

int	useless_function(t_farm *farm, char **line)
{
	int	ret;

	if (farm->line)
	{
		ret = 1;
		*line = farm->line;
	}
	else
		ret = gnl_store(0, line, farm, 1);
	return (ret);
}

int	bad_if_return(t_farm **farm, char *line)
{
	(*farm)->line = line;
	return (0);
}
