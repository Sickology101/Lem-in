/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   weight_helper.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: parkharo <parkharo@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/12/08 12:53:38 by marius            #+#    #+#             */
/*   Updated: 2022/12/10 16:56:11 by parkharo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "lemin.h"

void	set_weights(t_farm *farm)
{
	int	index;

	index = -1;
	while (++index < farm->room_nb)
		farm->id_table[index]->weight = 2147483647;
	farm->start->weight = 0;
}

static int	compare_weights(t_room *next, t_room *current, t_queue *queue)
{
	int	pos;

	if (queue->flow[current->id][next->id] == 1)
		return (0);
	if (queue->flow[current->id][next->id] == -1)
		pos = current->weight - 1;
	else
		pos = current->weight + 1;
	if (pos < next->weight && next->weight != 2147483647)
	{
		next->weight = pos;
		return (1);
	}
	return (0);
}

static int	check_loops(t_room *c, t_room *next, t_queue *queue, t_farm *farm)
{
	int	index;

	index = queue->prev[c->id];
	while (index != farm->end->id && index != farm->start->id)
	{
		if (index == next->id)
			return (1);
		if (index == c->id)
			return (1);
		if (index == queue->prev[next->id])
			return (1);
		index = queue->prev[index];
	}
	return (0);
}

int	check_weights(t_room *next, t_room *current, t_queue *q, t_farm *farm)
{
	if (next == farm->start || current == farm->start || next == current)
		return (0);
	if (check_loops(current, next, q, farm) == 1)
		return (0);
	if (compare_weights(next, current, q) == 0)
		return (0);
	if ((farm->id_table[q->prev[next->id]] != farm->start)
		&& (check_loops(farm->id_table[q->prev[next->id]], next, q, farm) == 0)
		&& (compare_weights(farm->id_table[q->prev[next->id]], next, q) == 1))
		q->prev[q->prev[next->id]] = next->id;
	q->prev[next->id] = current->id;
	return (1);
}
