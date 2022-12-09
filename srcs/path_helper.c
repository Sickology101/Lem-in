/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   path_functions.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marius <marius@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/12/08 12:53:07 by marius            #+#    #+#             */
/*   Updated: 2022/12/09 09:43:58 by marius           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#include "lemin.h"

t_path	*ft_new_path(int *path, int len)
{
	t_path *new_elem;

	if (!(new_elem = ft_memalloc(sizeof(t_path) + (sizeof(int) * (len + 1)))))
		return (NULL);
	if (path == NULL)
	{
		new_elem->path = NULL;
		new_elem->len = 0;
	}
	else
	{
		if (!(new_elem->path = ft_memalloc(sizeof(int) * (len + 1))))
			return (NULL);
		ft_memcpy(new_elem->path, path, ((len) * sizeof(int)));
		new_elem->len = len;
	}
	new_elem->next = NULL;
	new_elem->division = NULL;
	return (new_elem);
}

void	ft_add_path(t_path *paths, t_path *new)
{
	t_path *tracker;

	tracker = paths;
	if ((!paths) || (!new))
		return ;
	while (tracker->next != NULL)
		tracker = tracker->next;
	tracker->next = new;
}

t_path	*clean_path(t_path *path_list)
{
	t_path *temp;

	if (path_list->path == NULL && path_list->next->path != NULL)
	{
		path_list->next->max = path_list->max;
		path_list->next->division = path_list->division;
		path_list->next->longest = path_list->longest;
		temp = path_list;
		path_list = path_list->next;
		ft_memdel((void*)&temp);
	}
	return (path_list);
}

t_path	**set_path(t_path **path_list, int i, t_farm *farm)
{
	*path_list = clean_path(*path_list);
	(*path_list)->max = i;
	if (!((*path_list)->division = divide_ants(farm, *path_list)))
		(*path_list)->len = -1;
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
			ft_memdel((void*)&tracker->division);
		ft_memdel((void*)&tracker->path);
		ft_memdel((void*)&tracker);
		tracker = path_list;
	}
}
