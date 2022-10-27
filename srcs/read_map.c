/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   read_map.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marius <marius@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/26 14:53:18 by marius            #+#    #+#             */
/*   Updated: 2022/10/27 17:21:28 by marius           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "lemin.h"

void	my_exit(void)
{
	ft_printf("Reading error");
	exit (1);
}
// checks the line, if it is a room then it returns 1,
//if it is a link then it returns 2 
//if it is a comment then it returns 3 
//if it is invalid then it returns 0

int	check_line(char *line)
{
	int	index;

	index = 0;
	if(line[0] == '\0')
		return (0);
	else if(line[0] == '#')
			return (3);
	else
	{
		while(line[index] != ' ' && line[index] != '\0')
			index++;
		if (line[index] == '\0')
			return (2);
		else
			return (1);
	}
	return (0);
}

void	read_file(t_head *header)
{
	char	*line;
	//t_room	*room;
	int		ret;

	if (get_next_line(0, &line))
		header->ants = ft_atoi(line);
	if (header->ants == 0)
	{
		my_exit();
	}
	free(line);
	while (get_next_line(0, &line))
	{
		ret = check_line(line);
		if(ret == 0)
			my_exit();
		if(ret == 1)
			ft_printf("%d\n",ret);
		if(ret == 2)
			ft_printf("%d\n",ret);
		if(ret == 3)
			ft_printf("%d\n",ret);
		free(line);
	}
	free(line);
}
