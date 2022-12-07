/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lemin.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marius <marius@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/26 10:07:38 by marius            #+#    #+#             */
/*   Updated: 2022/12/07 18:10:42 by marius           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef LEMIN_H
# define LEMIN_H
# include "ft_printf.h"

# define __MAX_INT__ 2147483647

typedef struct s_path
{
	int	*path;
	int	len;
	int	max;
	int	*division;
	int	longest;
	struct s_path *next;
}				t_path;


typedef struct s_queue
{
	int	*queue;
	int	*visited;
	int *prev;
	int	length;
	int	position;
	int	**flow;
}				t_queue;


typedef struct s_input
{
	char *line;
	struct s_input	*next;
}				t_input;

typedef struct s_room
{
	char *name;
	int		*links;
	int				links_nb;
	int				id;
	int				empty;
	int				weight;
	struct s_room	*next;
	struct s_room	*prev;
}				t_room;

typedef struct s_farm
{
	int	room_nb;
	int	**links;
	t_room	**id_table;
	int	ants;
	int	flags;
	char	*line;
	int		max_paths;
	int		ant_nb;
	t_room	*start;
	t_room	*end;
	t_input *input;
	t_input *input_start;
	t_room	*first_room;
}				t_farm;

int	read_map(t_farm *farm, t_room *room);
int		generate(t_farm *farm);
int	error_free(char *line);
int	free_links(char *line, char **rooms, int ret);

#endif

/*typedef struct s_room
{
	char			*name;
	int				status; // 1 for start, 2 for end, 0 for normal
	int				connections;
	int				count;
	struct s_room	*next;
	struct s_room	*prev;
}				t_room;


typedef struct	s_head
{
	int	ants;
	t_room *next;
	
} 				t_head;

void	read_file(t_head *header);*/