/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rafael <rafael@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/13 22:56:17 by rafael            #+#    #+#             */
/*   Updated: 2025/06/13 22:56:19 by rafael           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../headers/minishell.h"

bool	is_builtin(char *cmd)
{
	if (!cmd)
		return (false);
	if (!ft_strncmp("echo", cmd, INT_MAX)
		|| !ft_strncmp("cd", cmd, INT_MAX)
		|| !ft_strncmp("pwd", cmd, INT_MAX)
		|| !ft_strncmp("export", cmd, INT_MAX)
		|| !ft_strncmp("unset", cmd, INT_MAX)
		|| !ft_strncmp("env", cmd, INT_MAX)
		|| !ft_strncmp("exit", cmd, INT_MAX))
		return (true);
	return (false);
}

static void	parent_process(t_data *data, t_cmd *cmd, int *pip)
{
	close(pip[1]);
	if (cmd->infile >= 0)
		close(cmd->infile);
	if (cmd->infile == -2)
		cmd->infile = pip[0];
	if (cmd->next != data->command_node && cmd->next->infile == -2)
		cmd->next->infile = pip[0];
	else
		close(pip[0]);
}

static bool	run_exec(t_data *data, t_cmd *cmd, int *pip)
{
	g_signal_pid = fork();
	if (g_signal_pid < 0)
		free_all(data, ERR_FORK, EXT_FORK);
	else if (!g_signal_pid)
	{
		if (cmd->cmd_param && cmd->cmd_param[0])
			child_process(data, cmd, pip);
		else
			free_all(data, NULL, 0);
	}
	else
		parent_process(data, cmd, pip);
	return (true);
}

static void	wait_all(t_data *data)
{
	int		status;
	int		pid;
	int		len;
	t_cmd	*tmp;

	tmp = data->command_node;
	len = len_cmd(tmp);
	while (len--)
	{
		pid = waitpid(0, &status, 0);
		if (pid == g_signal_pid && WIFEXITED(status))
			data->exit_code = WEXITSTATUS(status);
		if (tmp->outfile >= 0)
			close(tmp->outfile);
		if (tmp->infile >= 0)
			close(tmp->infile);
		tmp = tmp->next;
	}
}

bool	exec(t_data *data)
{
	t_cmd	*tmp;
	int		*pip;

	pip = data->pip;
	tmp = data->command_node;
	if (tmp && !tmp->skip_cmd && tmp->next == tmp
		&& tmp->cmd_param[0] && is_builtin(tmp->cmd_param[0]))
		return (launch_builtin(data, tmp));
	if (pipe(pip) == -1)
		return (false);
	run_exec(data, tmp, pip);
	tmp = tmp->next;
	while (tmp != data->command_node)
	{
		if (pipe(pip) == -1)
			return (false);
		run_exec(data, tmp, pip);
		tmp = tmp->next;
	}
	wait_all(data);
	return (true);
}
