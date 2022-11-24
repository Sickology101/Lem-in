/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   read_map.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marius <marius@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/26 14:53:18 by marius            #+#    #+#             */
/*   Updated: 2022/11/24 12:46:13 by marius           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "lemin.h"

int	is_start_end(char *line)
{
	if (ft_strcmp(line, "##start") == 0)
		return (-1);
	if (ft_strcmp(line, "##end") == 0)
		return (-1);
	return (0);
}

int	save_line(t_farm *farm, char *line, int ret)
{
	t_input *new;
	
	if (ret > 0)
	{
		farm->input->line = ft_strdup(line);
		if (!(new = ft_memalloc(sizeof(t_input))))
			return (-1);
		farm->input->next = new;
		farm->input = new;
	}
	return (0);
}

int	read_links(t_farm *farm, char **line, int ret, int fd)
{
	while (ret > 0 && line[0][0] == '#')
	{
		if (is_start_end(line[0]) == -1 || save_line(farm, line[0], ret) == -1)
			ret = -1;
		ft_memdel((void *)line);
		if (ret != -1)
			ret = get_next_line(fd, &line[0]);
	}
	return (ret);
}

int	parse_line(int fd, char **line, t_farm *farm, int origin)
{
	int	ret;
	
	ret = get_next_line(fd, line);
	if (ret > 0 && line[0] && line[0][0] == '#' && origin == 1)
	{
		if ((ret = read_links(farm, &line[0], ret, fd)) == -1)
		{
			ft_memdel((void *)line);
			return (-1);
		}
	}
	if (!(save_line(farm, line[0], ret) == 0))
		return (-1);
	return (ret);
}

int	delete_line(char *line)
{
	ft_memdel((void *)&line);
	return (-1);
}

int	get_num(char *str)
{
	long long	n;
	int			index;
	long long	sign;

	n = 0;
	sign = 1;
	index = 0;
	if (str[index] == '-')
	{
		sign = -1;
		index++;
	}
	while (ft_isdigit(str[index]))
	{
		n = n * 10 + str[index] - '0';
		if (n > 2147483648)
			break ;
		index++;
	}
	n = n * sign;
	return (n);
}

static int	get_ants(t_farm *farm)
{
	int	index;
	char	*line;
	int		ret;
	long long nb;
	
	index = 0;
	line = NULL;
	if ((ret = parse_line(0, &line, farm, 1) >= 0) && line)
	{
		while (line[index] != '\0')
		{
			if (ft_isdigit(line[index++]) != 1)
				return (delete_line(line));
		}
		if ((nb = get_num(line)) == 0 || nb > 2147483647 || nb <= 0 || ft_strlen(line) > 10)
			return(delete_line(line));
		farm->ants = nb; 
	}
	else
		return (-1);
	ft_memdel((void *)&line);
	return (0);
}
/*
	mode = 0 : line is not a link and is a comment return (0)
	mode = 1 : line is not a link and not a comment return (0)
*/
int	check_line(char *line, int mode)
{
	if (mode == 0)
	{
		if ((ft_strchr(line, '-') == NULL) || line[0] == '#')
			return (0);
		else
			return (-1);
	}
	if (mode == 1)
	{
		if ((ft_strchr(line, '-') != NULL && line[0] != '#'))
			return (0);
		else
			return (-1);
	}
	return (0);
}

int check_comment(t_farm *farm, char *line)
{
	if (ft_strcmp(line, "##start") == 0)
	{
		if (farm->start != NULL)
			return (-1);
		else
		{
			farm->flags |= 1; // 1 for START
			farm->flags &= ~2; // 2 for END
		}
	}
	else if (ft_strcmp(line, "##end") == 0)
	{
		if (farm->end != NULL)
			return (-1);
		else
		{
			farm->flags |= 2;
			farm->flags &= ~1;
		}
	}
	return (0);
}

int	check_coord(char *line)
{
	char	*temp;
	int		index;
	
	index = 0;
	temp = ft_strchr(line, ' ') + 1;
	while (temp[index] != ' ' && temp[index] != '\0')
		if (ft_isdigit(temp[index++]) != 1)
			return (-1);
	if (temp[index++] != ' ')
		return (-1);
	while (temp[index] != '\0' && index <= 20)
		if (ft_isdigit(temp[index++]) != 1)
			return (-1);
	return (0);
}

char	*check_syntax(char *line)
{
	int		index;
	char	*temp;

	index = 0;
	temp = line;
	if (line[0] != '#')
	{
		while (index <= 3 && temp != NULL)
			if (((temp = ft_strchr(temp, ' ')) != NULL) && temp++)
				index++;
		if (index != 2 || line[0] == 'L' || check_coord(line) != 0)
		{
			ft_memdel((void *)&line);
			return (NULL);
		}
	}
	return (line);
}

int	get_room(t_farm *farm, t_room *room)
{
	char	*line;
	int		ret;
	long	id;
	
	id = 0;
	line = NULL;
	ret = parse_line(0, &line, farm, 2);
	while (ret > 0 && line && check_line(line, 0) != -1)
	{
		if (line && line[0] == '#')
		{
			if (check_comment(farm, line) == -1)
				return (error_free(line));
		}
		else if (line && ft_strchr(line, '-') == NULL)
				if ((!(line) || (line = check_syntax(line)) == NULL) || (room = new_room(farm, room, line, id++)) == NULL)
					return (error_free(line));
		ft_memdel((void *)&line);
		ret = parse_line(0, &line, farm, 2);
		if (line && check_line(line, 1) != -1 && ((farm->line = line)))
			return (0);
	}
	ft_memdel((void *)&line);
	return ((ret > 0) ? 0 : -1);
}

int	room_exist(char *room, t_farm *farm)
{
	t_room *temp;
	
	temp = farm->first_room;
	while (temp && temp->next != NULL)
	{
		if (room[0] == temp->name[0])
			if (ft_strcmp(room, temp->name) == 0)
				return (-1);
		temp = temp->next;	
	}
	return (0);
}

int	init_room(t_farm *farm, t_room *room, char *line, int id)
{
	int	name_size;
	
	name_size = 0;
	while (line[name_size] != ' ')
		name_size++;
	if (!(room->name = ft_strndup(line,name_size)))
		return (-1);
	if (room_exist(room->name, farm) == -1)
		return (-1);
	room->id = id;
	room->empty = -1;
	if (farm->flags & 1 && farm->start == NULL)
	{
		farm->start = room;
		farm->flags &= ~2;
		farm->flags &= ~1;
	}
	else if (farm->flags & 2 && farm->end == NULL)
	{
		farm->end = room;
		farm->flags &= ~1;
		farm->flags &= ~2;
	}
	return (0);
}

t_room	*new_room(t_farm *farm, t_room *room, char *line, long id)
{
	t_room	*new;
	t_room	*temp;

	temp = room;
	if (id > 2147483647)
		return (NULL);
	if (room->name != NULL)
	{
		if(!(new = ft_memalloc(sizeof(t_room))))
			return (NULL);
		room->next = new;
		room = new;
		room->prev = temp;
	}
	if (init_room(farm, room, line, id) == -1)
		return (NULL);
	return (room);
}

int	create_table(t_farm *farm, t_room *room)
{
	if (farm->start == NULL || farm->end == NULL)
	{
		ft_memdel((void *)&farm->line);
		return (-1);
	}
	while (room->next)
		room = room->next;
	farm->room_nb = room->id + 1;
	if (!(farm->id_table = ft_memalloc(sizeof(t_room *) * (farm->room_nb + 1))))
		return (-1);
	while (room->prev)
	{
		farm->id_table[room->id] = room;
		room = room->prev;
	}
	farm->id_table[room->id] = room;
	return (0);
}

int	init_links(t_farm *farm, char **room)
{
	int	index;
	
	index = 0;
	room[0] = NULL;
	room[1] = NULL;
	if (!(farm->links = ft_memalloc(sizeof(int *) * (farm->room_nb + 1))))
		return (-1);
	while (index < farm->room_nb + 1)
		if (!(farm->links[index++] = ft_memalloc(sizeof(int) * (farm->room_nb + 1))))
			return (-1);
	return (0);
}

char *get_rooms_name(char *line, int mode)
{
	char *room;
	int		room_length;
	
	room_length = 0;
	if (mode == 1)
		while (line[room_length] != '-' && line[room_length] != '\0')
			room_length++;
	else if (mode == 2)
	{
		line = ft_strchr(line, '-') + 1;
		while (line[room_length] != '\0')
			room_length++;
	}
	room = ft_strndup(line, room_length);
	return (room);
}

int	room_exist2(t_farm *farm, char *room, t_room **ids, int mode)
{
	int	index;
	
	index = 0;
	while (index < farm->room_nb)
	{
		if (ft_strcmp(room, farm->id_table[index++]->name) == 0)
		{
			ids[mode] = farm->id_table[i - 1];
			return (1);
		}
	}
	return (0);
}

void	save_links(t_farm *farm, t_room **ids)
{
	farm->links[ids[0]->id][ids[1]->id] = 1;
	ids[0]->links_nb++;
	farm->links[ids[1]->id][ids[0]->id] = 1;
	ids[1]->links_nb++;
}

int	get_links(t_farm *farm)
{
	int	ret;
	char *line;
	char *room[2];
	t_room	*ids[2];

	if (init_links(farm, room) == -1)
		return (-1);
	if (farm->line && (ret = 1))
		line = farm->line;
	else
		ret = parse_line(0, &line, farm, 1);
	while (ret > 0)
	{
		if ((!(line)) || ((room[0] = get_rooms_name(line, 1)) == NULL) || (room_exist2(farm, room[0], ids, 0) != 1) || ((room[1] = get_rooms_name(line, 2)) == NULL) || (room_exist2(farm, room[1], ids, 1) != 1))
			return (free_links(line, room, -1));
		save_links(farm, ids);
		free_links(line, room, 0);
		ret = parse_line(0, &line, farm, 1);
	}
	return ((ret >= 0) ? 0 : -1);
}

int	create_link_list(t_farm *farm)
{
	int	index1;
	int	index2;
	int	index3;

	index1 = 0;
	index2 = 0;
	index3 = 0;
	while (index1 < farm->room_nb)
	{
		if (!(farm->id_table[index1]->links = ft_memalloc(sizeof(int) * (farm->id_table[index1]->links_nb + 1))))
			return (-1);
		while (index2 < farm->room_nb)
		{
			if (farm->links[index1][index2] == 1)
				farm->id_table[index1]->links[index3++] = index2;
			index2++;
		}
		index3 = 0;
		index2 = 0;
		index1++;
	}
	return (0);
}

int	read_map(t_farm *farm, t_room *room)
{
	t_input	*start;
	
	if(!(start = ft_memalloc(sizeof(t_input))))
		return (-1);
	farm->input = start;
	farm->input_start = start;
	room->next = NULL;
	if(get_ants(farm) != 0 || get_room(farm, room) != 0 || create_table(farm, room) != 0 || get_links(farm) != 0 || create_link_list(farm) != 0)// here
	{
		ft_printf("Error\n");
		return (-1);
	}
	return (0);
}

/*void	print_list(t_head *header)
{
	t_room	*room;
	ft_printf("\n%d\n",header->ants);
	room = header->next;
	while(room != NULL)
	{
		ft_printf("name = %s   status = %d   count = %d\n",room->name, room->status, room->count);
		room = room->next;
	}
}

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
	if (!ft_strcmp(line,"##start"))
		return (1);
	else if(!ft_strcmp(line,"##end"))
		return (2);
	else
		return (0);
}

char *get_name(char *line)
{
	char *str;
	int	index;

	index = 0;
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

void	add_room_to_list(t_head *header, t_room *room)
{
	t_room	*temp;
	
	if (header->next == NULL)
		header->next = room;
	else
	{
		temp = header->next;
		while (temp->next != NULL)
		{
			temp = temp->next;
		}
		temp->next = room;
	}
}

t_room	*get_room(char *line, int status, int *count)
{
	t_room	*room;

	room = NULL;
	if (check_line(line) == 1)
	{
		room = (t_room *)malloc(sizeof(t_room));
		room->name = get_name(line);
		room->status = status;
		if (status == 1)
			room->count = 0;
		else if (status == 2)
			room->count = 1;
		else
		{
			room->count = *count;
			(*count)++;
		}
		room->next = NULL;
	}
	else
		my_exit();
	return (room);
}

t_room	*get_start(int	status)
{
	t_room *room;
	char	*line;
	int		bogger;

	room = NULL;
	if (get_next_line(0,&line))
		room = get_room(line, status, &bogger);
	else
		my_exit();
	return (room);
}



void	read_file(t_head *header)
{
	char	*line;
	t_room	*room;
	int		ret;
	int		count;

	if (get_next_line(0, &line))
		header->ants = ft_atoi(line);
	if (header->ants == 0)
	{
		my_exit();
	}
	free(line);
	header->next = NULL;
	room = NULL;
	count = 2;
	while (get_next_line(0, &line))
	{
		ret = check_line(line);
		if(ret == 0)
			my_exit();
		if(ret == 1)
		{
			room = get_room(line, 3, &count);
			if (room != NULL)
			{
				add_room_to_list(header, room);
				room = NULL;
			}
		}
		if(ret == 2)
			ft_printf("%d",ret);
		if(ret == 3)
		{
			ret = check_start_end(line);
			if(ret == 1 || ret == 2)
				room = get_start(ret);
			if (room != NULL)
			{
				add_room_to_list(header, room);
				room = NULL;
			}
				
		}
		free(line);
	}
	free(line);
	print_list(header);
}
*/