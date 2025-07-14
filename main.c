/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rafael <rafael@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/18 14:50:11 by rafael            #+#    #+#             */
/*   Updated: 2025/06/18 14:50:11 by rafael           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "headers/minishell.h"

pid_t	g_signal_pid;

static int	init_env(t_data *shell_data, char **env)
{
	t_env	*list;
	int		i;
	char	*tmp;

	if (!(*env))
		return (make_env2(shell_data));
	i = -1;
	list = NULL;
	while (env[++i])
	{
		tmp = ft_strdup(env[i]);
		if (!tmp)
			return (free_list(&list));
		if (!append(&list, tmp))
			return (free_list(&list));
	}
	shell_data->env = list;
	return (1);
}

static void	init_data(t_data *shell_data, int argc, char **argv)
{
	(void)argc;
	(void)argv;
	shell_data->env = NULL;
	shell_data->token_node = NULL;
	shell_data->command_node = NULL;
	shell_data->exit_code = 0;
	shell_data->pip[0] = -1;
	shell_data->pip[1] = -1;
	g_signal_pid = 0;
	signals();
}

static void	init_cd_dot(t_data *data)
{
	char	*params[3];

	params[0] = "cd";
	params[1] = ".";
	params[2] = NULL;
	ft_cd(data, params);
}

static bool	parse_line(t_data *data, char *line)
{
	if (open_quote(data, line))
		return (free(line), false);
	if (!replace_dollar(&line, data)
		|| !create_list_token(&data->token_node, line))
		return (free(line), free_all(data, ERR_MALLOC, EXT_MALLOC), false);
	free(line);
	if (data->token_node && data->token_node->prev->type == PIPE)
	{
		write(2, "Error: Unclosed pipe\n", 21);
		data->exit_code = 2;
		free_token(&data->token_node);
		return (false);
	}
	if (!data->token_node || !create_list_cmd(data))
	{
		free_token(&data->token_node);
		free_cmd(&data->command_node);
		return (false);
	}
	return (check_pipe(data));
}

int	main(int argc, char **argv, char **env)
{
	t_data	shell_data;
	char	*line;

	init_data(&shell_data, argc, argv);
	if (!init_env(&shell_data, env))
		free_all(&shell_data, ERR_MALLOC, EXT_MALLOC);
	init_cd_dot(&shell_data);
	while (1)
	{
		line = readline("minishell> ");
		if (!line)
			free_all(&shell_data, "exit\n", shell_data.exit_code);
		if (is_empty_line(line))
			continue ;
		add_history(line);
		if (!parse_line(&shell_data, line))
			continue ;
		if (!exec(&shell_data))
			free_all(&shell_data, ERR_PIPE, EXT_PIPE);
		free_cmd(&shell_data.command_node);
		free_token(&shell_data.token_node);
		g_signal_pid = 0;
	}
	rl_clear_history();
	return (free_all(&shell_data, NULL, -1), 0);
}
