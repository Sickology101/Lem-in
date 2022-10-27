/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marius <marius@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/26 10:19:50 by marius            #+#    #+#             */
/*   Updated: 2022/10/27 17:22:30 by marius           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "lemin.h"

int	main()
{
	t_head *header;

	header = (t_head *)malloc(sizeof(t_head));
	read_file(header);
	free(header);
	return (0);
}
