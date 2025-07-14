/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_cd.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rafael <rafael@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/13 22:55:18 by rafael            #+#    #+#             */
/*   Updated: 2025/06/18 13:36:51 by rafael           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../headers/minishell.h"

static int	count_arg(char **params)
{
	int	count;

	count = 0;
	while (params[count])
		count++;
	return (count);
}

static char	*get_env_value(char *name, t_env *env)
{
	int		len;

	len = ft_strlen(name);
	while (env)
	{
		if (ft_strncmp(env->str, name, len) == 0 && env->str[len] == '=')
			return (env->str + len + 1);
		env = env->next;
	}
	return (NULL);
}

static void	update_pwd(t_data *data, char *param)
{
	char	cwd[PATH_MAX];
	char	*pwd;

	if (getcwd(cwd, PATH_MAX) == NULL)
	{
		perror(param);
		return ;
	}
	pwd = ft_strjoin("PWD=", cwd);
	if (!pwd)
		return ((void)print_error(ERR_MALLOC));
	export(pwd, &data->env);
	free(pwd);
}

int	ft_cd(t_data *data, char **params)
{
	int		res;
	char	*path;

	if (count_arg(params) == 1)
	{
		path = get_env_value("HOME", data->env);
		if (!path)
			return (ft_putstr_fd("minishell: cd: HOME not set\n", 2), 1);
	}
	else if (count_arg(params) == 2)
		path = params[1];
	else
		return (ft_putstr_fd("minishell: cd: too many arguments\n", 2), 1);
	res = chdir(path);
	if (res == -1)
		perror(path);
	else
		update_pwd(data, path);
	return (res != 0);
}
