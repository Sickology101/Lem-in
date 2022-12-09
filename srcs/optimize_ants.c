/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   optimize_ants.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marius <marius@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/12/08 12:53:01 by marius            #+#    #+#             */
/*   Updated: 2022/12/09 09:42:42 by marius           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#include "lemin.h"

static int	*calculate_divide(int *ant_div, t_farm *farm, int total, int *steps)
{
	int		index;

	index = 0;
	total = (total + farm->ant_nb) / farm->max_paths;
	while (index < farm->max_paths)
	{
		ant_div[index] = total - steps[index];
		++index;
	}
	return (ant_div);
}

static int	*split_remainder(int *ant_division, int remainder, t_path **paths)
{
	int		index;
	int		full;

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

static int	*check_total_ants(int *ant_division, t_farm *farm, t_path **paths)
{
	int		index;
	int		total;

	index = 0;
	total = 0;
	while (index < (*paths)->max)
	{
		total = total + ant_division[index];
		++index;
	}
	if (total < farm->ant_nb)
		ant_division = split_remainder(ant_division, farm->ant_nb - total, paths);
	return (ant_division);
}

static int	get_longest(int *ant_division, int *steps, int max)
{
	int		index;
	int		longest;

	index = -1;
	longest = 0;
	while (++index < max)
	{
		if (longest < steps[index] + ant_division[index] - 1)
			longest = steps[index] + ant_division[index] - 1;
	}
	return (longest);
}

int			*divide_ants(t_farm *farm, t_path *paths)
{
	int		*ant_division;
	int		*steps;
	int		total;

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
	paths->longest = get_longest(ant_division, steps, paths->max);
	ft_memdel((void*)&steps);
	return (ant_division);
}
