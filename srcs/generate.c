/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   generate.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marius <marius@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/12/08 12:52:12 by marius            #+#    #+#             */
/*   Updated: 2022/12/09 09:08:45 by marius           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#include "lemin.h"

static int	find_neg_flow(t_queue *queue, t_room *room, t_farm *farm)
{
	int		index;

	index = 0;
	while (index < room->links_nb)
	{
		if (farm->id_table[room->links[index]]->weight != 2147483647)
			check_weights(farm->id_table[room->links[index]], room, queue, farm);
		if (queue->visited[room->links[index]] != 1 && queue->flow[room->id][room->links[index]] == -1)
		{
			queue->queue[queue->position] = room->links[index];
			queue->prev[room->links[index]] = room->id;
			queue->visited[room->links[index]] = 1;
			++queue->position;
			farm->id_table[room->links[index]]->weight = room->weight - 1;
			return (1);
		}
		++index;
	}
	return (0);
}

static int	find_flow(t_queue *queue, t_room *room, int prev_flow, t_farm *farm)
{
	int		index;

	index = 0;
	if (prev_flow == 0 && find_neg_flow(queue, room, farm) == 1)
		return (0);
	while (index < room->links_nb)
	{
		if (farm->id_table[room->links[index]]->weight != 2147483647)
			check_weights(farm->id_table[room->links[index]], room, queue, farm);
		if (queue->visited[room->links[index]] != 1
			&& queue->flow[room->id][room->links[index]] != 1
			&& (room != farm->start || room->links[index] != farm->end->id))
		{
			queue->queue[queue->position] = room->links[index];
			queue->prev[room->links[index]] = room->id;
			queue->visited[room->links[index]] = 1;
			++queue->position;
			if (queue->flow[room->id][room->links[index]] == 0)
				farm->id_table[room->links[index]]->weight = room->weight + 1;
			else
				farm->id_table[room->links[index]]->weight = room->weight - 1;
		}
		++index;
	}
	return (0);
}

static void	save_flow(t_queue *queue, t_farm *farm)
{
	int		index1;
	int		index2;

	index1 = farm->end->id;
	if (queue->prev[index1] == farm->start->id)
		return ;
	while (index1 != farm->start->id)
	{
		index2 = queue->prev[index1];
		if (queue->flow[index1][index2] == 0)
		{
			queue->flow[index1][index2] = -1;
			queue->flow[index2][index1] = 1;
		}
		else if (queue->flow[index1][index2] == -1 || queue->flow[index1][index2] == 1)
		{
			queue->flow[index1][index2] = 0;
			queue->flow[index2][index1] = 0;
		}
		index1 = index2;
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
	
	while (++index < queue->length && queue->visited[farm->end->id] != 1 && queue->queue[index] >= 0)
	{
		node = queue->queue[index];
		if (index > 0)
			prev_flow = queue->flow[queue->prev[node]][node];
		find_flow(queue, farm->id_table[node], prev_flow, farm);
	}
	if (queue->prev[farm->end->id] == -1 ||
		(queue->prev[farm->end->id] == farm->start->id && t == 1))
		return (-1);
	return (0);
}

int			generate(t_queue *queue, t_farm *farm, t_path **path, int t)
{
	t_path	*new;

	*path = ft_new_path(NULL, 0);
	(*path)->longest = 0;
	set_weights(farm);
	while (optimise_flow(farm, queue, t) == 0 && (t = 1))
	{
		new = ft_new_path(NULL, 0);
		new->longest = 0;
		save_flow(queue, farm);
		set_to_n(&queue->visited, queue->length, 0);
		reset_queue(queue, farm->start->id, farm->end->id);
		save_paths(queue, farm, &new);
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
	return ((t == 1) ? 0 : -1);
}
