/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   read_map.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marius <marius@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/26 14:53:18 by marius            #+#    #+#             */
/*   Updated: 2022/10/28 16:03:53 by marius           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "lemin.h"

void	my_exit(void)
{
	ft_printf("Reading error");
	exit (1);
}
// checks the line, 
//if it is a room then it returns 1,
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

//checks if the comment is start, end or just a comment returning 1, 2 and 0 respectively

int	check_start_end(char *line)
{
	if (ft_strcmp(line,"##START"))
		return (1);
	else if(ft_strcmp(line,"##END"))
		return (2);
	else
		return (0);
}

char *get_name(char *line)
{
	char *str;
	int	index;

	while (line[index] != ' ')
		index++;
	str = (char *)malloc(sizeof(char) * index);
	str[index] = '\0';
	index = 0;
	while (line[index] != ' ')
	{
		str[index] = line[index];
		index++;
	}
	return (str);
}

t_room	*get_start(void)
{
	t_room *room;
	char	*line;

	if (get_next_line(0,&line))
	{
		if (check_line(line) == 1)
		{
			room = (t_room *)malloc(sizeof(t_room));
			room->name = get_name(line);
			room->status = 1;
		}
		else
			my_exit();
	}
	else
		my_exit();
	return (room);
}

void	read_file(t_head *header)
{
	char	*line;
	t_room	*room;
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
		if (room == NULL)
		{
			room = (t_room *)malloc(sizeof(t_room));
			room->next = NULL;
		}
		ret = check_line(line);
		if(ret == 0)
			my_exit();
		if(ret == 1)
			ft_printf("%d\n",ret);
		if(ret == 2)
			ft_printf("%d\n",ret);
		if(ret == 3)
		{
			ret = check_end_start(line);
			if(ret == 1)
				room = get_start();
			else if (ret == 2)
				room = get_end();
		}
		free(line);
	}
	free(line);
}
