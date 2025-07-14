/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd_fd.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rafael <rafael@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/23 21:13:19 by rafael            #+#    #+#             */
/*   Updated: 2025/06/23 21:13:21 by rafael           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../headers/minishell.h"

static int	open_file(t_data *shell_data, char *filename, int type)
{
	int	fd;

	fd = -2;
	if (type == INPUT)
		fd = open(filename, O_RDONLY, 0644);
	else if (type == HEREDOC)
		fd = here_doc(shell_data, filename);
	else if (type == TRUNC)
		fd = open(filename, O_CREAT | O_WRONLY | O_TRUNC, 0644);
	else if (type == APPEND)
		fd = open(filename, O_CREAT | O_WRONLY | O_APPEND, 0644);
	if (type != HEREDOC && fd < 0)
		perror(filename);
	return (fd);
}

static bool	get_in(t_data *shell_data, t_tok *tmp, t_cmd *command_node)
{
	if (tmp->type == INPUT)
	{
		if (command_node->infile >= 0)
			close(command_node->infile);
		if (tmp == tmp->next || tmp->next->type <= 5)
			return (print_error_token(tmp, shell_data));
		command_node->infile = open_file(shell_data, tmp->next->str, INPUT);
		if (command_node->infile == -1)
			return (false);
	}
	else if (tmp->type == HEREDOC)
	{
		if (command_node->infile >= 0)
			close(command_node->infile);
		if (tmp == tmp->next || tmp->next->type <= 5)
			return (print_error_token(tmp, shell_data));
		command_node->infile = open_file(shell_data, tmp->next->str, HEREDOC);
		if (command_node->infile == -1)
			return (false);
	}
	return (true);
}

bool	get_infile(t_data *shell_data, t_tok *token_node, t_cmd *command_node)
{
	t_tok	*tmp;

	tmp = token_node;
	if (tmp->type != PIPE && !get_in(shell_data, tmp, command_node))
		return (false);
	if (tmp->type == PIPE)
		return (true);
	tmp = tmp->next;
	while (tmp->type != PIPE && tmp != shell_data->token_node)
	{
		if (!get_in(shell_data, tmp, command_node))
			return (false);
		tmp = tmp->next;
	}
	return (true);
}

static bool	get_out(t_tok *tmp, t_cmd *command_node, t_data *shell_data)
{
	if (tmp->type == TRUNC)
	{
		if (command_node->outfile >= 0)
			close(command_node->outfile);
		if (tmp == tmp->next || tmp->next->type <= 5)
			return (print_error_token(tmp, shell_data));
		command_node->outfile = open_file(NULL, tmp->next->str, TRUNC);
		if (command_node->outfile == -1)
			return (false);
	}
	else if (tmp->type == APPEND)
	{
		if (command_node->outfile >= 0)
			close(command_node->outfile);
		if (tmp == tmp->next || tmp->next->type <= 5)
			return (print_error_token(tmp, shell_data));
		command_node->outfile = open_file(NULL, tmp->next->str, APPEND);
		if (command_node->outfile == -1)
			return (false);
	}
	return (true);
}

bool	get_outfile(t_tok *token_node, t_cmd *command_node, t_data *shell_data)
{
	t_tok	*tmp;

	tmp = token_node;
	if (tmp->type != PIPE && !get_out(tmp, command_node, shell_data))
		return (false);
	tmp = tmp->next;
	while (tmp != shell_data->token_node && tmp->type != PIPE)
	{
		if (!get_out(tmp, command_node, shell_data))
			return (false);
		tmp = tmp->next;
	}
	return (true);
}
