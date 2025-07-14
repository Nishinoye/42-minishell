/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ms_utils.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rafael <rafael@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/13 22:57:51 by rafael            #+#    #+#             */
/*   Updated: 2025/06/13 22:57:52 by rafael           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../headers/minishell.h"

bool	is_space(char c)
{
	if (c && (c == ' ' || c == '\n' || c == '\r' || c == '\f' || c == '\t' \
	|| c == '\v'))
		return (true);
	return (false);
}

int	is_special(char *str)
{
	if (str && *str && ft_strlen(str) >= 2)
	{
		if (!ft_strncmp(str, "<<", 2))
			return (HEREDOC);
		if (!ft_strncmp(str, ">>", 2))
			return (APPEND);
	}
	if (*str && ft_strlen(str) >= 1)
	{
		if (!ft_strncmp(str, "<", 1))
			return (INPUT);
		if (!ft_strncmp(str, ">", 1))
			return (TRUNC);
		if (!ft_strncmp(str, "|", 1))
			return (PIPE);
	}
	return (0);
}

bool	check_pipe(t_data *shell_data)
{
	if (shell_data->token_node->type == PIPE)
	{
		write(2, "syntax error near unexpected token_node '|'\n", 39);
		free_token(&shell_data->token_node);
		free_cmd(&shell_data->command_node);
		return (false);
	}
	return (true);
}

bool	make_env2(t_data *shell_data)
{
	char	path[PATH_MAX];
	char	*tmp;

	tmp = ft_strdup("OLDPWD");
	if (!tmp || !append(&(shell_data->env), tmp) || \
		getcwd(path, PATH_MAX) == NULL)
		free_all(shell_data, ERR_MALLOC, EXT_MALLOC);
	tmp = ft_strjoin("PWD=", path);
	if (!tmp || !append(&(shell_data->env), tmp))
		free_all(shell_data, ERR_MALLOC, EXT_MALLOC);
	return (1);
}

void	absolute_path(char **path, char *command_node, t_data *shell_data)
{
	*path = ft_strdup(command_node);
	if (!(*path))
		free_all(shell_data, ERR_MALLOC, EXT_MALLOC);
	if (access((*path), F_OK))
	{
		write(2, (*path), ft_strlen((*path)));
		write(2, " : command not found\n", 21);
		free(*path);
		*path = NULL;
	}
}
