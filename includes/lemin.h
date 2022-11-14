/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lemin.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marius <marius@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/26 10:07:38 by marius            #+#    #+#             */
/*   Updated: 2022/11/14 15:05:56 by marius           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef LEMIN_H
# define LEMIN_H
# include "ft_printf.h"

typedef struct s_input
{
	char *line;
	struct s_input	*next;
}				t_input;

typedef struct s_room
{
	char *name;
	int		*links;
	struct s_room	*next;
	struct s_room	*prev;
}				t_room;

typedef struct s_farm
{
	int	room_nb;
	int	**links;
	int	**id_table;
	int	ants;
	int	flags;
	t_room	*start;
	t_room	*end;
	t_input *input;
	t_input *input_start;
	t_room	*first_room;
}				t_farm;

int	read_map(t_farm *farm, t_room *room);

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