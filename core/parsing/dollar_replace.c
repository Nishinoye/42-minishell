/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   dollar_replace.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rafael <rafael@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/23 21:13:53 by rafael            #+#    #+#             */
/*   Updated: 2025/06/23 21:13:55 by rafael           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../headers/minishell.h"

static int	in_env(t_data *shell_data, char *line, int size, char **str)
{
	char	*tmp;
	char	*key;
	char	*value;

	key = get_dollar_word(line, size);
	value = get_elem_env(shell_data->env, key);
	if (key)
		free(key);
	tmp = ft_strjoin(*str, value);
	if (value)
		free(value);
	free(*str);
	if (!tmp)
		return (0);
	*str = tmp;
	return (1);
}

static int	dollar_point_interrogation(t_data *shell_data, char **str)
{
	char	*tmp;
	char	*tmp2;

	tmp = ft_itoa(shell_data->exit_code);
	if (!tmp)
		return (0);
	tmp2 = ft_strjoin(*str, tmp);
	free(tmp);
	free(*str);
	if (!tmp2)
		return (0);
	*str = tmp2;
	return (1);
}

int	add_dollar(char *line, int *index, char **str, t_data *shell_data)
{
	int		ctrl;
	int		n;

	n = *index;
	ctrl = exist_in_env(line, index, shell_data);
	if (ctrl == 1)
		return (in_env(shell_data, &line[n], *index - n, str));
	else if (ctrl == 2)
	{
		(*index) += 2;
		return (dollar_point_interrogation(shell_data, str));
	}
	else
	{
		++(*index);
		while (line[*index] && \
			(ft_isalnum(line[*index]) || line[*index] == '_'))
			++(*index);
		return (1);
	}
}

int	add_char(char *c, char **str, t_data *shell_data, int *index)
{
	char	char_to_str[2];
	char	*tmp2;
	int		i;

	i = 0;
	if (c[i] == '$' && !shell_data->sq && exist_in_env(c, &i, shell_data))
		return (1);
	char_to_str[0] = *c;
	char_to_str[1] = '\0';
	(*index)++;
	tmp2 = ft_strjoin(*str, char_to_str);
	free(*str);
	if (!tmp2)
		return (0);
	*str = tmp2;
	return (1);
}

int	replace_dollar(char **line, t_data *shell_data)
{
	bool	dq;
	int		i;
	char	*str;

	i = 0;
	dq = false;
	shell_data->sq = false;
	str = ft_strdup("");
	if (!str)
		return (0);
	while ((*line)[i])
	{
		quoting_choice(&dq, &shell_data->sq, NULL, (*line)[i]);
		if ((*line)[i] == '$'
			&& (ft_isalpha((*line)[i + 1]) || (*line)[i + 1] == '?' \
			|| (*line)[i + 1] == '_')
			&& !shell_data->sq
			&& !add_dollar((*line), &i, &str, shell_data))
			return (0);
		if ((*line)[i] && !add_char(&(*line)[i], &str, shell_data, &i))
			return (0);
	}
	free(*line);
	*line = str;
	return (1);
}
