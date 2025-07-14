/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.h                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rafael <rafael@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/13 22:04:39 by rafael            #+#    #+#             */
/*   Updated: 2025/06/13 22:04:39 by rafael           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef GET_NEXT_LINE_H
# define GET_NEXT_LINE_H

# include <stdio.h>
# include <stdlib.h>
# include <unistd.h>
# include "libft.h"

# ifndef BUFFER_SIZE
#  define BUFFER_SIZE 1
# endif

typedef struct s_list_gnl
{
	char				*content;
	struct s_list_gnl	*next;
	struct s_list_gnl	*prev;
}					t_list_gnl;

int		free_list_gnl(t_list_gnl **list);
int		how_many(t_list_gnl *list, char **line);
int		make_line(t_list_gnl *list, char **line);
int		new_line(t_list_gnl *list);
int		add_to_list_gnl(t_list_gnl **list, char *buf);
int		write_in_list_gnl(int fd, t_list_gnl **list);
int		clear_list_gnl(t_list_gnl **list);
char	*get_next_line(int fd);

#endif