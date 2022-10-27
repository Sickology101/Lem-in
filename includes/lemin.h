/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lemin.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marius <marius@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/26 10:07:38 by marius            #+#    #+#             */
/*   Updated: 2022/10/27 11:15:31 by marius           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef LEMIN_H
# define LEMIN_H
# include "ft_printf.h"

typedef struct s_room
{
	char name;
	int connections;
	char *con_names;
	struct s_room *next;
	struct s_room *prev;
}				t_room;


typedef struct	s_head
{
	int	ants;
	t_room *next;
	
} 				t_head;

void	read_file(t_head *header);


#endif