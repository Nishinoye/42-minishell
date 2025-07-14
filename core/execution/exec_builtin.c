/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   launch_builtin.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rafael <rafael@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/13 22:56:38 by rafael            #+#    #+#             */
/*   Updated: 2025/06/13 22:56:39 by rafael           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../headers/minishell.h"

static void	run_builtin_command(int out, t_data *data, t_cmd *cmd)
{
	char	*name;

	name = cmd->cmd_param[0];
	if (!ft_strncmp("echo", name, INT_MAX))
		data->exit_code = ft_echo(cmd->cmd_param);
	else if (!ft_strncmp("cd", name, INT_MAX))
		data->exit_code = ft_cd(data, cmd->cmd_param);
	else if (!ft_strncmp("pwd", name, INT_MAX))
		data->exit_code = ft_pwd();
	else if (!ft_strncmp("export", name, INT_MAX))
		data->exit_code = ft_export(cmd->cmd_param, &data->env);
	else if (!ft_strncmp("unset", name, INT_MAX))
		data->exit_code = ft_unset(cmd->cmd_param, &data->env);
	else if (!ft_strncmp("env", name, INT_MAX))
		data->exit_code = ft_env(data->env);
	else if (!ft_strncmp("exit", name, INT_MAX))
	{
		if (cmd->outfile >= 0)
		{
			dup2(out, 1);
			close(out);
		}
		ft_exit(data, cmd->cmd_param);
	}
}

bool	launch_builtin(t_data *data, t_cmd *cmd)
{
	int	save_out;

	save_out = -1;
	if (cmd->outfile >= 0)
	{
		save_out = dup(1);
		dup2(cmd->outfile, 1);
	}
	run_builtin_command(save_out, data, cmd);
	if (cmd->outfile >= 0)
	{
		dup2(save_out, 1);
		close(save_out);
	}
	return (true);
}
