/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   solve.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marius <marius@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/24 12:54:48 by marius            #+#    #+#             */
/*   Updated: 2022/11/24 15:59:09 by marius           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "lemin.h"

int	set_to_n(int **set, int	len, int n)
{
	int index;
	
	index = -1;
	while (++index < len)
		set[0][index] = n;
	return (0);
}

void	reset_queue(t_queue *queue, int start, int end)
{
	queue->queue[0] = start;
	queue->visited[start] = 1;
	queue->position = 1;
	if (end >= 0)
		queue->visited[end] = 0;
}

int	init_queue(t_queue *queue, t_farm *farm)
{
	int	index;

	index = 0;
	queue->length = farm->room_nb;
	queue->queue = NULL;
	queue->prev = NULL;
	queue->flow = NULL;
	queue->visited = NULL;
	if (!(queue->queue = ft_memalloc(sizeof(int) * queue->length)))
		return (-1);
	if (!(queue->prev = ft_memalloc(sizeof(int) * queue->length)))
		return (-1);
	if (!(queue->visited = ft_memalloc(sizeof(int) * queue->length)))
		return (-1);
	if (!(queue->flow = ft_memalloc(sizeof(int) * queue->length)))
		return (-1);
	while (index < queue->length)
	{
		if (!(queue->flow[index++] = ft_memalloc(sizeof(int) * queue->length)))
			return (-1);
	}
	set_to_n(&queue->queue, queue->length, -1);
	set_to_n(&queue->prev, queue->length, -1);
	reset_queue(queue, farm->start->id, -1);
	return (0);
}

void	free_queue(t_queue *queue)
{
	int	index;
	
	index = 0;
	while (index < queue->length)
	{
		ft_memdel((void *)&queue->flow[index]);
		++index;
	}
	if (queue->flow != NULL)
		free(queue->flow);
	if (queue->prev != NULL)
		free(queue->prev);
	if (queue->queue != NULL)
		free(queue->queue);
	if (queue->visited != NULL)
		free(queue->visited);
}

t_path *new_path(int *path, int len)
{
	t_path *new;

	if (!(new = ft_memalloc(sizeof(t_path) + (sizeof(int) * (len + 1)))))
		return (NULL);
	if (path == NULL)
	{
		new->path = NULL;
		new->len = 0;
	}
	else
	{
		if (!(new->path = ft_memalloc(sizeof(int) * (len + 1))))
			return (NULL);
		ft_memcpy(new->path, path, ((len) * sizeof(int)));
		new->len = 0;
	}
	new->next = NULL;
	new->division = NULL;
	return (new);
}

void	set_weights(t_farm *farm)
{
	int	index;

	index = -1;
	while (++index < farm->room_nb)
		farm->id_table[index]->weight = 2147483647;
	farm->start->weight = 0;
}

void	clear_queue(t_queue *queue)
{
	set_to_n(&queue->prev, queue->length, -1);
	set_to_n(&queue->queue, queue->length, -1);
}

int	check_start_end(t_farm *farm, t_queue *queue)
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

int	check_loops(t_room *current, t_room *next, t_queue *queue, t_farm *farm)
{
	int	index;

	index = queue->prev[current->id];
	while (index != farm->end->id && index != farm->start->id)
	{
		if (index == next->id)
			return (1);
		if (index == current->id)
			return (1);
		if (index == queue->prev[next->id])
			return (1);
		index = queue->prev[index];
	}
	return (0);
}

int	compare_weights(t_room *next, t_room *current, t_queue *queue)
{
	int pos;

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

int	check_weights(t_room *next, t_room *current, t_queue *queue, t_farm *farm)
{
	if (next == farm->start || current == farm->start || next == current)
		return (0);
	if (check_loops(current, next, queue, farm) == 1)
		return (0);
	if (compare_weights(next, current, queue) == 0)
		return (0);
	if ((farm->id_table[queue->prev[next->id]] != farm->start) && (check_loops(farm->id_table[queue->prev[next->id]], next, queue, farm) == 0) && (compare_weights(farm->id_table[queue->prev[next->id]], next, q) == 1))
		queue->prev[queue->prev[next->id]] = next->id;
	queue->prev[next->id] = current->id;
	return (1);
}

int	find_neg_flow(t_queue *queue, t_room *room, t_farm *farm)
{
	int	index;

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

int	find_flow(t_queue *queue, t_room *room, int prev_flow, t_farm *farm)
{
	int	index;

	index = 0;
	if (prev_flow == 0 && find_neg_flow(queue, room, farm) == 1)
		return (0);
	while (index < room->links_nb)
	{
		if (farm->id_table[room->links[index]]->weight != 2147483647)
			check_weights(farm->id_table[room->links[index]], room, queue, farm);
		if (queue->visited[room->links[index]] != 1 && queue->flow[room->id][room->links[index]] != 1 && (room != farm->start || room->links[index] != farm->end->id))
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

int	optimise(t_farm *farm, t_queue *queue, int t)
{
	int	index;
	int node;
	int prev_flow;

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
	if (queue->prev[farm->end->id] == -1 || (queue->prev[farm->end->id] == farm->start->id && t == 1))
		return (-1);
	return (0);
}

int	solve(t_queue *queue, t_farm *farm, t_path **path, int t)
{
	t_path *new;
	
	*path = new_path(NULL, 0);
	(*path)->longest = 0;
	set_weights(farm);
	while (optimise(farm, queue,t) == 0 && (t = 1))
	{
		//here
	}
}

int		generate(t_farm *farm)
{
	t_queue queue;
	t_path	*path_list;
	int	index;

	if (init_queue(&queue, farm) < 0)
	{
		ft_printf("ERROR\n");
		free_queue(&queue);
		return (-1);
	}
	if (solve(&queue, farm, &path_list, 0) == -1)
	{
		
	}
}