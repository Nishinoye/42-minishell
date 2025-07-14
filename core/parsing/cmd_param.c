/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd_param.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rafael <rafael@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/23 21:13:27 by rafael            #+#    #+#             */
/*   Updated: 2025/06/23 21:13:29 by rafael           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../headers/minishell.h"

static int	is_valid_arg(t_data *data, t_tok *tok)
{
	return (tok->type == CMD
		|| (tok->type == ARG
			&& tok->prev != data->token_node->prev
			&& tok->prev->type > 5));
}

static int	count_args(t_data *data, t_tok *tok)
{
	int		count;
	t_tok	*tmp;

	count = 0;
	tmp = tok;
	if (is_valid_arg(data, tmp))
		count++;
	tmp = tmp->next;
	while (tmp != data->token_node && tmp->type != PIPE)
	{
		if (is_valid_arg(data, tmp))
			count++;
		tmp = tmp->next;
	}
	return (count);
}

static int	add_to_cmd_param(char **cmd, int *i, char *str)
{
	cmd[*i] = ft_strdup(str);
	if (!cmd[*i])
		return (0);
	(*i)++;
	return (1);
}

static void	*free_cmd_param(char **cmd, int i)
{
	while (--i >= 0)
		free(cmd[i]);
	free(cmd);
	return (NULL);
}

char	**get_param(t_data *data, t_tok *tok)
{
	char	**cmd;
	int		i;
	t_tok	*tmp;

	i = 0;
	cmd = malloc(sizeof(char *) * (count_args(data, tok) + 1));
	if (!cmd)
		return (NULL);
	tmp = tok;
	if (tmp->type != PIPE && is_valid_arg(data, tmp)
		&& !add_to_cmd_param(cmd, &i, tmp->str))
		return (free_cmd_param(cmd, i));
	tmp = tmp->next;
	while (tmp != data->token_node && tmp->type != PIPE)
	{
		if (is_valid_arg(data, tmp)
			&& !add_to_cmd_param(cmd, &i, tmp->str))
			return (free_cmd_param(cmd, i));
		tmp = tmp->next;
	}
	cmd[i] = NULL;
	return (cmd);
}
