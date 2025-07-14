/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rafael <rafael@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/13 22:58:33 by rafael            #+#    #+#             */
/*   Updated: 2025/06/13 22:58:34 by rafael           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../headers/minishell.h"

void	free_array(char **arr)
{
	int	i;

	i = 0;
	while (arr && arr[i])
		free(arr[i++]);
	if (arr)
		free(arr);
	arr = NULL;
}

bool	print_error(char *str)
{
	if (str)
		write(2, str, ft_strlen(str));
	return (true);
}

bool	print_error_token(t_tok *token_node, t_data *shell_data)
{
	write(2, "syntax error near unexpected token_node ", 35);
	write(2, "'", 1);
	if (token_node->next == shell_data->token_node)
		write(2, "newline", 7);
	else
		write(2, token_node->next->str, ft_strlen(token_node->next->str));
	write(2, "'\n", 2);
	return (false);
}

void	free_all(t_data *shell_data, char *err, int ext)
{
	if (shell_data->command_node)
		free_cmd(&shell_data->command_node);
	if (shell_data->token_node)
		free_token(&shell_data->token_node);
	if (shell_data->env)
		free_list(&shell_data->env);
	if (shell_data->pip[0] && shell_data->pip[0] != -1)
		close(shell_data->pip[0]);
	if (shell_data->pip[1] && shell_data->pip[1] != -1)
		close(shell_data->pip[1]);
	if (err)
		print_error(err);
	rl_clear_history();
	if (!access(".heredoc.tmp", F_OK))
		unlink(".heredoc.tmp");
	if (ext != -1)
		exit(ext);
}
