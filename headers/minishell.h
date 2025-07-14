/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rafael <rafael@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/13 22:56:57 by rafael            #+#    #+#             */
/*   Updated: 2025/06/18 14:07:55 by rafael           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include <stdio.h>
# include <stdlib.h>
# include <stdbool.h>
# include <sys/types.h>
# include <sys/wait.h>
# include <limits.h>
# include <fcntl.h>
# include <sys/stat.h>
# include <readline/readline.h>
# include <readline/history.h>
# include "../Libft/libft.h"
# include "../Libft/get_next_line.h"

# define INPUT		1	//"<"
# define HEREDOC	2	//"<<"
# define TRUNC		3	//">"
# define APPEND		4	//">>"
# define PIPE		5	//"|"
# define CMD		6	//"|"
# define ARG		7	//"|"

# define ERR_MALLOC	"malloc error\n"
# define ERR_PIPE	"pipe error\n"
# define ERR_FORK	"fork error\n"

# define EXT_MALLOC	1
# define EXT_PIPE	1
# define EXT_FORK	1

extern pid_t	g_signal_pid;

typedef struct s_cmd
{
	bool			skip_cmd;
	int				infile;
	int				outfile;
	char			**cmd_param;
	struct s_cmd	*prev;
	struct s_cmd	*next;
}				t_cmd;

typedef struct s_tok
{
	char			*str;
	int				type;
	struct s_tok	*prev;
	struct s_tok	*next;
}				t_tok;

typedef struct s_env
{
	char			*str;
	struct s_env	*prev;
	struct s_env	*next;
}					t_env;

typedef struct s_data
{
	t_env	*env;
	t_tok	*token_node;
	t_cmd	*command_node;
	int		exit_code;
	int		pip[2];
	bool	sq;
}				t_data;

/* main */
int		make_env(t_data *shell_data, char **env);

/* List utils */
int		free_list(t_env **list);
int		append(t_env **list, char *elem);
size_t	len_list(t_env *list);

/* quote */
void	quoting_choice(bool *dq, bool *sq, int *index, char c);
int		open_quote(t_data *shell_data, char *line);

/* dollar_env */
int		exist_in_env(char *line, int *i, t_data *shell_data);
char	*get_elem_env(t_env *env, char *key);
char	*get_dollar_word(char *line, int size);

/* dollar_replace */
int		add_dollar(char *line, int *index, char **str, t_data *shell_data);
int		add_char(char *c, char **str, t_data *shell_data, int *index);
int		replace_dollar(char **line, t_data *shell_data);

//signals.c
void	clear_rl_line(void);
void	signals(void);

//create_token.c
bool	create_list_token(t_tok **begin, char *command);

//list_token.c
int		append_token(t_tok **list, char *str, int type);
void	free_token(t_tok **list);

//ms_utils.c
bool	is_space(char c);
int		is_special(char *str);
bool	check_pipe(t_data *shell_data);
bool	make_env2(t_data *shell_data);
void	absolute_path(char **path, char *command_node, t_data *shell_data);

//ft_env.c
int		ft_env(t_env	*env);
//ft_export.c
int		ft_export(char **str, t_env **env);
bool	export(char *str, t_env **env);
//ft_echo
int		ft_echo(char **args);
//ft_unset.c
int		ft_unset(char **str, t_env **env);
// ft_pwd
int		ft_pwd(void);
// ft_cd
int		ft_cd(t_data *shell_data, char **params);
//ft_exit.c
void	ft_exit(t_data *shell_data, char **args);

//free.c
void	free_array(char **arr);
bool	print_error(char *str);
void	free_all(t_data *shell_data, char *err, int ext);
bool	print_error_token(t_tok *token_node, t_data *shell_data);

//array_utils.c
char	**lst_to_arr(t_env *env);
void	sort_array(char **arr, int len);
bool	is_empty_line(char *line);

//list_cmd.c
int		append_cmd(t_cmd **list, int infile, int outfile, char **cmd_param);
void	free_cmd(t_cmd **list);
size_t	len_cmd(t_cmd *list);

//create_cmd.c
bool	create_list_cmd(t_data *shell_data);

//cmd_fd.c
bool	get_infile(t_data *shell_data, t_tok *token_node, t_cmd *command_node);
bool	get_outfile(t_tok *token_node, t_cmd *command_node, t_data *shell_data);

//here_doc.c
int		here_doc(t_data *shell_data, char *word);

//cmd_param.c
char	**get_param(t_data *shell_data, t_tok *token_node);

//exec.c
bool	exec(t_data *shell_data);
bool	is_builtin(char *command_node);

//launch_builtin.c
bool	launch_builtin(t_data *shell_data, t_cmd *command_node);

//find_cmd.c
char	*find_cmd(t_data *shell_data, char *sample, t_env *env);

//exec2.c
void	child_process(t_data *shell_data, t_cmd *command_node, int *pip);

//signals2.c
void	signals2(void);

#endif
