/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strmapi.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rafael <rafael@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/12 13:39:57 by rafael            #+#    #+#             */
/*   Updated: 2024/11/12 13:43:45 by rafael           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strmapi(char const *s, char (*f)(unsigned int, char))
{
	char			*new_s;
	unsigned int	count;

	count = 0;
	new_s = (char *)malloc(ft_strlen(s) + 1);
	if (new_s == NULL)
		return (NULL);
	while (s[count] != '\0')
	{
		new_s[count] = (*f)(count, s[count]);
		count++;
	}
	new_s[count] = '\0';
	return (new_s);
}
