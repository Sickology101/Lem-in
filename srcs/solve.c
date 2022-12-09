/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   solve.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marius <marius@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/12/08 12:53:33 by marius            #+#    #+#             */
/*   Updated: 2022/12/09 10:12:20 by marius           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#include "lemin.h"

int			solve(t_farm *farm)
{
	t_queue	queue;
	t_path	*path_list;

	if (initialise_queue(&queue, farm) < 0)
	{
		ft_printf("ERROR\n");
		free_queue(&queue);
		return (-1);
	}
	if (generate(&queue, farm, &path_list, 0) == -1)
	{
		ft_printf("ERROR\n");
		free_queue(&queue);
		free_path(path_list);
		return (-1);
	}
	send_ants(farm, path_list, farm->ant_nb, 0);
	free_path(path_list);
	free_queue(&queue);
	return (0);
}
