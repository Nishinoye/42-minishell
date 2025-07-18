/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   dollar_env.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rafael <rafael@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/23 21:13:47 by rafael            #+#    #+#             */
/*   Updated: 2025/06/23 21:13:49 by rafael           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../headers/minishell.h"

static int	ft_search(char *str, char c)
{
	int	i;

	i = -1;
	while (str[++i])
		if (str[i] == c)
			return (i);
	return (0);
}

static int	end_word(char *str, char *env)
{
	int	i;

	i = 0;
	while (str[i] && (ft_isalnum(str[i]) || str[i] == '_'))
		++i;
	if (i == ft_search(env, '='))
		return (i);
	return (0);
}

int	exist_in_env(char *line, int *i, t_data *shell_data)
{
	t_env	*tmp;
	int		len;

	if (line[*i + 1] == '?' || line[*i + 1] == '$')
		return (2);
	tmp = shell_data->env;
	len = len_list(tmp);
	while (len--)
	{
		if (ft_strncmp(tmp->str, &line[*i + 1], \
			end_word(&line[*i + 1], tmp->str)) == 0)
		{
			*i += ft_strlen(tmp->str) - \
				ft_strlen(ft_strchr(tmp->str, '=')) + 1;
			return (1);
		}
		tmp = tmp->next;
	}
	return (0);
}

char	*get_elem_env(t_env *env, char *key)
{
	t_env	*tmp;
	int		len;
	int		t;

	if (!key)
		return (NULL);
	tmp = env;
	len = len_list(tmp);
	t = ft_strlen(key);
	while (len--)
	{
		if (ft_strncmp(tmp->str, key, t) == 0)
		{
			len = 0;
			while (tmp->str[len])
				if (tmp->str[len++] == '=')
					break ;
			return (ft_strdup(&(tmp->str[len])));
		}
		tmp = tmp->next;
	}
	return (NULL);
}

char	*get_dollar_word(char *line, int size)
{
	char	*dollar;
	int		i;

	dollar = malloc(sizeof(char) * size);
	if (!dollar)
		return (NULL);
	i = 0;
	while (line[++i] && i < size)
		dollar[i - 1] = line[i];
	dollar[i - 1] = '\0';
	return (dollar);
}
