/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   solve.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marius <marius@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/24 12:54:48 by marius            #+#    #+#             */
/*   Updated: 2022/12/07 19:26:20 by marius           ###   ########.fr       */
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
		new->len = len;
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
		farm->id_table[index]->weight = __MAX_INT__;
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
	if (pos < next->weight && next->weight != __MAX_INT__)
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
	if ((farm->id_table[queue->prev[next->id]] != farm->start) && (check_loops(farm->id_table[queue->prev[next->id]], next, queue, farm) == 0) && (compare_weights(farm->id_table[queue->prev[next->id]], next, queue) == 1))
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
		if (farm->id_table[room->links[index]]->weight != __MAX_INT__)
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
		if (farm->id_table[room->links[index]]->weight != __MAX_INT__)
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

void	save_flow(t_queue *queue, t_farm *farm)
{
	int	index1;
	int	index2;
	
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

int	find_neighbours(t_queue *queue, t_room *room)
{
	int	index;
	
	index = 0;
	while (index < room->links_nb)
	{
		if (queue->visited[room->links[index]] == 0 && queue->flow[room->id][room->links[index]] == 1)
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

int	breadth_first_search(t_farm *farm, t_queue *queue)
{
	int	index;
	int	node;
	
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
	if (queue->flow[farm->start->id][farm->end->id] == 1 && queue->prev[farm->end->id] == farm->start->id)
		queue->flow[farm->start->id][farm->end->id] = 0;
	return (0);
}

size_t	count_steps(t_queue *queue, int start, int end)
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

int	*rev_path(t_farm *farm, t_queue *queue)
{
	int	*rev_path;
	int	steps;
	int	index;
	int	pos;

	index = 0;
	pos = farm->end->id;
	steps = count_steps(queue, farm->start->id, farm->end->id);
	if (!(rev_path = ft_memalloc((sizeof(int)) * (steps + 1))))
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

t_path **path_error(t_path **path)
{
	(*path)->len = -1;
	return (path);
}

void	mark_path(t_farm *farm, t_queue *queue)
{
	int path;
	int index;
	
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

void	add_path(t_path *paths, t_path *new)
{
	t_path *tracker;
	
	tracker = paths;
	if ((!paths) || (!new))
		return ;
	while (tracker->next != NULL)
	{
		tracker = tracker->next;
	}
	tracker->next = new;
}

t_path *clean_path(t_path *path_list)
{
	t_path *temp;
	
	if (path_list->path == NULL && path_list->next->path != NULL)
	{
		path_list->next->max = path_list->max;
		path_list->next->division = path_list->division;
		path_list->next->longest = path_list->longest;
		temp = path_list;
		path_list = path_list->next;
		ft_memdel((void *)&temp);
	}
	return (path_list);
}

int	*get_path_lengths(t_farm *farm, t_path *paths, int *total)
{
	int index;
	int *steps;
	t_path *path;
	
	index = 0;
	path = paths;
	if (!(steps = (int *)ft_memalloc(sizeof(int) * farm->max_paths)))
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

int *calculate_divide(int *ant_div, t_farm *farm, int total, int *steps)
{
	int index;

	index = 0;
	total = (total + farm->ants) / farm->max_paths;
	while (index < farm->max_paths)
	{
		ant_div[index] = total - steps[index];
		++index;
	}
	return (ant_div);
}

int get_longest(int *ant_division, int *steps, int max)
{
	int index;
	int longest;

	index = -1;
	longest = 0;
	while (++index < max)
	{
		if (longest < steps[index] + ant_division[index] - 1)
			longest = steps[index] + ant_division[index] - 1;
	}
	return (longest);
}

int *split_remainder(int *ant_division, int remainder, t_path **paths)
{
	int index;
	int full;

	full = 0;
	while (remainder > 0)
	{
		index = 0;
		while (index < (*paths)->max)
		{
			if (ant_division[index] + 1 <= (*paths)->longest)
			{
				--remainder;
				++ant_division[index];
			}
			if (ant_division[index] >= (*paths)->longest)
				++full;
			if (full == (*paths)->max)
			{
				full = 0;
				++(*paths)->longest;
			}
			++index;
		}
	}
	return (ant_division);
}

int *check_total_ants(int *ant_division, t_farm *farm, t_path **paths)
{
	int index;
	int total;

	index = 0;
	total = 0;
	while (index < (*paths)->max)
	{
		total = total + ant_division[index];
		++index;
	}
	if (total < farm->ants)
		ant_division = split_remainder(ant_division, farm->ants - total, paths);
	return (ant_division);
}

int	*divide_ants(t_farm *farm, t_path *paths)
{
	int	*ant_division;
	int	*steps;
	int total;
	
	total = 0;
	if (!(ant_division = (int *)ft_memalloc(sizeof(int) * paths->max)))
		return (NULL);
	farm->max_paths = paths->max;
	if (paths->max == 1)
	{
		ant_division[0] = farm->ant_nb;
		return (ant_division);
	}
	if ((steps = get_path_lengths(farm, paths, &total)) == NULL)
		return (NULL);
	ant_division = calculate_divide(ant_division, farm, total, steps);
	paths->longest = get_longest(ant_division, steps, paths->max);
	ant_division = check_total_ants(ant_division, farm, &paths); 
	paths->longest = get_longest(ant_division, steps, paths->max); //HERE
	ft_memdel((void *)&steps);
	return (ant_division);
}

t_path	**set_path(t_path **path_list, int index, t_farm *farm)
{
	*path_list = clean_path(*path_list);
	(*path_list)->max = index;
	if (!((*path_list)->division = divide_ants(farm, *path_list)))
		(*path_list)->len = -1;
	return (path_list);
}

t_path **save_paths(t_queue *queue, t_farm *farm, t_path **path_list)
{
	int	*path;
	size_t steps;
	t_path *new;
	int	index;
	
	index = 0;
	set_weights(farm);
	while (breadth_first_search(farm, queue) == 0)
	{
		if (!(path = rev_path(farm, queue)))
			return (path_error(path_list));
		steps = count_steps(queue, farm->start->id, farm->end->id);
		mark_path(farm, queue);
		if (!(new = new_path(path, steps + 1)))
			return (path_error(path_list));
		ft_memdel((void *)&path);
		add_path(*path_list, new);
		++index;
	}
	path_list = set_path(path_list, index, farm);
	return (path_list);
}

void	free_path(t_path *path_list)
{
	t_path *tracker;

	tracker = path_list;
	while (tracker != NULL)
	{
		path_list = path_list->next;
		if (tracker->division != NULL)
			ft_memdel((void *)&tracker->division);
		ft_memdel((void *)&tracker->path);
		ft_memdel((void *)&tracker);
		tracker = path_list;
	}
}

int	solve(t_queue *queue, t_farm *farm, t_path **path, int t) // edmondskarp
{
	t_path *new;
	
	*path = new_path(NULL, 0);
	(*path)->longest = 0;
	set_weights(farm);
	while (optimise(farm, queue,t) == 0 && (t = 1)) // SEGFAULT HAPPENS HERE, if more than 1 iteration through while!
	{
		new = new_path(NULL, 0);
		new->longest = 0;
		save_flow(queue, farm);
		set_to_n(&queue->visited, queue->length, 0);
		reset_queue(queue, farm->start->id, farm->end->id);
		save_paths(queue, farm, &new); // HERE!
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
	ft_printf("\nHERE!\n");
	return ((t == 1) ? 0 : -1);
}

int find_last_ant(t_farm *farm, int *path)
{
	int index;
	
	index = 0;
	while (path[index] != farm->end->id)
		++index;
	while (index != 0 && farm->id_table[path[index]]->empty == -1)
		--index;
	return (index + 1);
}

t_path *reset_ants(int *x, int *index, t_path *paths)
{
	x[0] = 0;
	index[0] = -1;
	return (paths);
}

int check_print_space(int x)
{
	if (x != 0)
		ft_putchar(' ');
	x = 1;
	return (x);
}

int reach_finish(int *path, t_farm *farm, int index, int *x)
{
	if (path[index] == farm->end->id)
	{
		x[0] = check_print_space(x[0]);
		ft_printf("L%d-%s",farm->id_table[path[index - 1]]->empty, farm->id_table[path[index]]->name);
		if (index >= 1)
			farm->id_table[path[index - 1]]->empty = -1;
		return (1);
	}
	return (0);
}

void	move_ant(int *path, t_farm *farm, int index, int *x)
{
	if (farm->id_table[path[index]]->empty == -1 && index > 1 && farm->id_table[path[index - 1]]->empty != -1)
	{
		x[0] = check_print_space(x[0]);
		farm->id_table[path[index]]->empty = farm->id_table[path[index - 1]]->empty;
		farm->id_table[path[index - 1]]->empty = -1;
		ft_printf("L%d-%s", farm->id_table[path[index]]->empty, farm->id_table[path[index]]->name);
	}
}

void	move_ants_on_path(t_farm *farm, int *path, int *finished_ants, int *x)
{
	int	index;
	
	index = find_last_ant(farm, path);
	if (path[0] == farm->start->id && path[1] == farm->end->id)
		return ;
	while (index != 0)
	{
		if ((reach_finish(path, farm, index, x)) == 1)
			++finished_ants[0];
		else if (finished_ants[0] < farm->ants)
			move_ant(path, farm, index, x);
		--index;
	}
}

int send_new_ant(t_farm *farm, int *path, int mov, int *fin)
{
	int index;

	index = 0;
	if (mov <= farm->ants && farm->id_table[path[1]]->empty == -1)
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

int send_ants(t_farm *farm, t_path *paths, int mv_ants, int x)
{
	int	finished_ants;
	int	index1;
	int	index2;
	t_path	*path;

	mv_ants = 0;
	index2 = 0;
	finished_ants = 0;
	while (finished_ants < farm->ants && (++index2 || 1))
	{
		path = reset_ants(&x, &index1, paths);
		while (++index1 < paths->max)
		{
			move_ants_on_path(farm, path->path, &finished_ants, &x);
			if (mv_ants < farm->ants && paths->division[index1] > 0)
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

int		generate(t_farm *farm) // prev known as solve
{
	t_queue queue;
	t_path	*path_list;

	if (init_queue(&queue, farm) < 0)
	{
		ft_printf("ERROR\n");
		free_queue(&queue);
		return (-1);
	}
	if (solve(&queue, farm, &path_list, 0) == -1) // segfault happens here!
	{
		ft_printf("ERROR\n");
		free_queue(&queue);
		free_path(path_list);
		return (-1);
	}
	send_ants(farm, path_list, farm->ants, 0);
	free_path(path_list);
	free_queue(&queue);
	return (0);
}