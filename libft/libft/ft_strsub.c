/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strsub.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mangheli <mangheli@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/08 19:11:20 by mangheli          #+#    #+#             */
/*   Updated: 2021/11/08 19:11:22 by mangheli         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strsub(char const *s, unsigned int start, size_t len)
{
	size_t	index;
	char	*dest;

	dest = (char *)malloc(sizeof(char) * len + 1);
	if (!dest)
		return (NULL);
	index = 0;
	while (index < len)
	{
		dest[index] = s[start];
		index++;
		start++;
	}
	dest[index] = '\0';
	return (dest);
}
