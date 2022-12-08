/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   breadthsearch.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marius <marius@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/12/08 12:52:04 by marius            #+#    #+#             */
/*   Updated: 2022/12/08 13:39:23 by marius           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#include "lemin.h"

int				check_start_end(t_farm *farm, t_queue *queue)
{
	int index;

	index = -1;
	while (++index != farm->start->links_nb)
	{
		if (farm->start->links[index] == farm->end->id)
		{
			queue->flow[farm->start->id][farm->end->id] = 1;
			queue->flow[farm->end->id][farm->start->id] = -1;
			queue->prev[farm->end->id] = farm->start->id;
			return (1);
		}
	}
	return (0);
}

static int		find_neighbours(t_queue *queue, t_room *room)
{
	int index;

	index = 0;
	while (index < room->links_nb)
	{
		if (queue->visited[room->links[index]] == 0
			&& queue->flow[room->id][room->links[index]] == 1)
		{
			queue->queue[queue->position] = room->links[index];
			queue->prev[room->links[index]] = room->id;
			queue->visited[room->links[index]] = 1;
			++queue->position;
		}
		++index;
	}
	return (0);
}

/*
** This is a breadth-first search function. We traverse the graph in levels,
** terminating the search when we visit end. If we visit all possible nodes
** but do not visit end, we return -1, to indicate a path was not foud.
*/

int				breadth_first_search(t_farm *farm, t_queue *queue)
{
	int		index;
	int		node;

	index = -1;
	set_to_n(&queue->queue, queue->length, -1);
	reset_queue(queue, farm->start->id, farm->end->id);
	while (++index < queue->length && queue->visited[farm->end->id] != 1 && queue->queue[index] >= 0)
	{
		node = queue->queue[index];
		find_neighbours(queue, farm->id_table[node]);
	}
	if (queue->visited[farm->end->id] != 1)
		return (-1);
	if (queue->flow[farm->start->id][farm->end->id] == 1
		&& queue->prev[farm->end->id] == farm->start->id)
		queue->flow[farm->start->id][farm->end->id] = 0;
	return (0);
}
