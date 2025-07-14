/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rafael <rafael@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/13 22:04:03 by rafael            #+#    #+#             */
/*   Updated: 2025/06/13 22:04:03 by rafael           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line.h"

int	how_many(t_list_gnl *list, char **line)
{
	t_list_gnl	*tmp;
	int			len;
	int			i;

	tmp = list;
	len = 0;
	while (tmp->next != list)
	{
		len += ft_strlen(tmp->content);
		tmp = tmp -> next;
	}
	i = -1;
	while (tmp->content[++i])
	{
		if (tmp->content[i] == '\n')
		{
			++len;
			break ;
		}
		++len;
	}
	*line = malloc(sizeof(char *) * (len + 1));
	return (len);
}

int	make_line(t_list_gnl *list, char **line)
{
	int	i;
	int	j;
	int	len;

	len = how_many(list, line);
	if (!line)
		return (free_list_gnl(&list));
	j = 0;
	while (j < len)
	{
		i = -1;
		while (list -> content[++i])
		{
			if (list -> content[i] == '\n')
			{
				(*line)[j++] = list -> content[i];
				break ;
			}
			(*line)[j++] = list -> content[i];
		}
		list = list -> next;
	}
	(*line)[j] = '\0';
	return (1);
}

int	new_line(t_list_gnl *list)
{
	int			i;
	t_list_gnl	*current;

	if (!list)
		return (0);
	current = list->prev;
	i = -1;
	while (current -> content[++i])
		if (current -> content[i] == '\n')
			return (1);
	return (0);
}

char	*get_next_line(int fd)
{
	static t_list_gnl	*list = NULL;
	char				*line;

	line = NULL;
	if (fd < 0 && list)
		free_list_gnl(&list);
	if (fd < 0 || BUFFER_SIZE < 0)
		return (NULL);
	if (!write_in_list_gnl(fd, &list) || !list)
		return (NULL);
	if (!make_line(list, &line) || !clear_list_gnl(&list))
		return (NULL);
	if (line[0] == '\0')
	{
		free(line);
		free_list_gnl(&list);
		return (NULL);
	}
	return (line);
}
