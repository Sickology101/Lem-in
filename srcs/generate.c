/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   generate.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: parkharo <parkharo@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/12/08 12:52:12 by marius            #+#    #+#             */
/*   Updated: 2022/12/10 17:22:31 by parkharo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "lemin.h"

static int	find_neg_flow(t_queue *q, t_room *r, t_farm *farm)
{
	int	i;

	i = 0;
	while (i < r->links_nb)
	{
		if (farm->id_table[r->links[i]]->weight != 2147483647)
			check_weights(farm->id_table[r->links[i]], r, q, farm);
		if (q->visited[r->links[i]] != 1 && q->flow[r->id][r->links[i]] == -1)
		{
			q->queue[q->position] = r->links[i];
			q->prev[r->links[i]] = r->id;
			q->visited[r->links[i]] = 1;
			++q->position;
			farm->id_table[r->links[i]]->weight = r->weight - 1;
			return (1);
		}
		++i;
	}
	return (0);
}

static int	find_flow(t_queue *q, t_room *room, int prev_flow, t_farm *farm)
{
	int		index;

	index = 0;
	if (prev_flow == 0 && find_neg_flow(q, room, farm) == 1)
		return (0);
	while (index < room->links_nb)
	{
		if (farm->id_table[room->links[index]]->weight != 2147483647)
			check_weights(farm->id_table[room->links[index]], room, q, farm);
		if (q->visited[room->links[index]] != 1
			&& q->flow[room->id][room->links[index]] != 1
			&& (room != farm->start || room->links[index] != farm->end->id))
		{
			q->queue[q->position] = room->links[index];
			q->prev[room->links[index]] = room->id;
			q->visited[room->links[index]] = 1;
			++q->position;
			if (q->flow[room->id][room->links[index]] == 0)
				farm->id_table[room->links[index]]->weight = room->weight + 1;
			else
				farm->id_table[room->links[index]]->weight = room->weight - 1;
		}
		++index;
	}
	return (0);
}

void	save_flow(t_queue *queue, t_farm *farm)
{
	int		i1;
	int		i2;

	i1 = farm->end->id;
	if (queue->prev[i1] == farm->start->id)
		return ;
	while (i1 != farm->start->id)
	{
		i2 = queue->prev[i1];
		if (queue->flow[i1][i2] == 0)
		{
			queue->flow[i1][i2] = -1;
			queue->flow[i2][i1] = 1;
		}
		else if (queue->flow[i1][i2] == -1 || queue->flow[i1][i2] == 1)
		{
			queue->flow[i1][i2] = 0;
			queue->flow[i2][i1] = 0;
		}
		i1 = i2;
	}
}

static int	optimise_flow(t_farm *farm, t_queue *queue, int t)
{
	int		index;
	int		node;
	int		prev_flow;

	index = -1;
	clear_queue(queue);
	reset_queue(queue, farm->start->id, farm->end->id);
	prev_flow = 0;
	check_start_end(farm, queue);
	while (++index < queue->length
		&& queue->visited[farm->end->id] != 1 && queue->queue[index] >= 0)
	{
		node = queue->queue[index];
		if (index > 0)
			prev_flow = queue->flow[queue->prev[node]][node];
		find_flow(queue, farm->id_table[node], prev_flow, farm);
	}
	if (queue->prev[farm->end->id] == -1
		|| (queue->prev[farm->end->id] == farm->start->id && t == 1))
		return (-1);
	return (0);
}

int	generate(t_queue *queue, t_farm *farm, t_path **path, int t)
{
	t_path	*new;

	*path = ft_new_path(NULL, 0);
	(*path)->longest = 0;
	set_weights(farm);
	while (optimise_flow(farm, queue, t) == 0)
	{
		t = 1;
		new = ft_new_path(NULL, 0);
		new->longest = 0;
		new_function(queue, farm, &new);
		if (new->len == -1)
			return (-1);
		if ((*path)->longest == 0 || (*path)->longest > new->longest)
		{
			free_path((*path));
			*path = new;
		}
		else
			free_path(new);
		clear_queue(queue);
	}
	return (return_check3(t));
}
