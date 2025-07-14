/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   create_cmd.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rafael <rafael@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/23 21:13:32 by rafael            #+#    #+#             */
/*   Updated: 2025/06/23 21:13:34 by rafael           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../headers/minishell.h"

static bool	fill_cmd(t_data *shell_data, t_tok *tmp)
{
	if (!get_infile(shell_data, tmp, shell_data->command_node->prev) && \
		shell_data->command_node->prev->infile != -1)
		return (false);
	if (shell_data->command_node->prev->infile == -1)
	{
		shell_data->command_node->prev->skip_cmd = true;
		shell_data->command_node->prev->outfile = -1;
		return (true);
	}
	if (!get_outfile(tmp, shell_data->command_node->prev, shell_data) && \
		shell_data->command_node->prev->outfile \
		!= -1)
		return (false);
	if (shell_data->command_node->prev->outfile == -1)
	{
		if (shell_data->command_node->prev->infile >= 0)
			close (shell_data->command_node->prev->infile);
		shell_data->command_node->prev->skip_cmd = true;
		shell_data->command_node->prev->infile = -1;
		return (true);
	}
	shell_data->command_node->prev->cmd_param = get_param(shell_data, tmp);
	if (!shell_data->command_node->prev->cmd_param)
		free_all(shell_data, ERR_MALLOC, EXT_MALLOC);
	return (true);
}

static bool	norm(t_data *shell_data, t_tok *tmp)
{
	if (!append_cmd(&shell_data->command_node, -2, -2, NULL))
		free_all(shell_data, ERR_MALLOC, EXT_MALLOC);
	if (!fill_cmd(shell_data, tmp))
	{
		shell_data->exit_code = 2;
		return (false);
	}
	return (true);
}

bool	create_list_cmd(t_data *shell_data)
{
	t_tok	*tmp;

	tmp = shell_data->token_node;
	if (!norm(shell_data, tmp))
		return (false);
	tmp = tmp->next;
	while (tmp != shell_data->token_node)
	{
		if (tmp->prev->type == PIPE)
		{
			if (!norm(shell_data, tmp))
				return (false);
		}
		tmp = tmp->next;
	}
	return (true);
}
