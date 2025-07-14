/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec2.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rafael <rafael@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/13 22:56:21 by rafael            #+#    #+#             */
/*   Updated: 2025/06/13 22:56:23 by rafael           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../headers/minishell.h"

static bool	check_dir(char **path, char *command_node, t_data *shell_data)
{
	struct stat	path_stat;

	stat(*path, &path_stat);
	if (!S_ISREG(path_stat.st_mode))
	{
		print_error(command_node);
		print_error(" : Is a directory\n");
		shell_data->exit_code = 126;
		return (false);
	}
	return (true);
}

static bool	cmd_exist(char **path, t_data *shell_data, char *command_node)
{
	if (!ft_strchr(command_node, '/'))
		*path = find_cmd(shell_data, command_node, shell_data->env);
	else
		absolute_path(path, command_node, shell_data);
	if (!(*path) && shell_data->exit_code == -1)
		free_all(shell_data, NULL, shell_data->exit_code);
	if (!(*path))
	{
		shell_data->exit_code = 127;
		return (false);
	}
	if (access((*path), X_OK))
	{
		perror(*path);
		free((*path));
		(*path) = NULL;
		shell_data->exit_code = 126;
		return (false);
	}
	if (!check_dir(path, command_node, shell_data))
		return (false);
	return (true);
}

static void	redirect_in_out(t_data *shell_data, t_cmd *command_node, int *pip)
{
	close(pip[0]);
	if (command_node->infile >= 0)
	{
		dup2(command_node->infile, 0);
		close(command_node->infile);
	}
	if (command_node->outfile >= 0)
	{
		dup2(command_node->outfile, 1);
		close(command_node->outfile);
	}
	else if (command_node->next != shell_data->command_node)
		dup2(pip[1], 1);
	close(pip[1]);
}

static void	built(int *pip, t_cmd *command_node, t_data *shell_data)
{
	close(pip[0]);
	if (command_node->outfile < 0 && \
		command_node->next != shell_data->command_node)
		command_node->outfile = pip[1];
	else
		close(pip[1]);
	launch_builtin(shell_data, command_node);
}

void	child_process(t_data *shell_data, t_cmd *command_node, int *pip)
{
	char	*path;
	char	**env;

	path = NULL;
	if (command_node->skip_cmd)
		shell_data->exit_code = 1;
	else if (is_builtin(command_node->cmd_param[0]))
		built(pip, command_node, shell_data);
	else if (cmd_exist(&path, shell_data, command_node->cmd_param[0]))
	{
		redirect_in_out(shell_data, command_node, pip);
		env = lst_to_arr(shell_data->env);
		if (!env)
			free_all(shell_data, ERR_MALLOC, EXT_MALLOC);
		rl_clear_history();
		signals2();
		execve(path, command_node->cmd_param, env);
		free(env);
	}
	if (path)
		free(path);
	free_all(shell_data, NULL, shell_data->exit_code);
}
