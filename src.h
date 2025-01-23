/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   src.h                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: peli <peli@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/29 13:57:26 by tbartocc          #+#    #+#             */
/*   Updated: 2025/01/23 11:38:47 by peli             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SRC_H
# define SRC_H

# include "libft/libft.h"
# include <errno.h>
# include <fcntl.h>
# include <limits.h>
# include <readline/history.h>
# include <readline/readline.h>
# include <signal.h>
# include <stdbool.h>
# include <stdio.h>
# include <stdlib.h>
# include <string.h>
# include <sys/wait.h>
# include <unistd.h>
# include <sys/stat.h>

extern int	g_signum;

typedef enum token
{
	WORD,
	PIPE,
	REDIR_IN, // <
	REDIR_OUT, // >
	APPEND, // >>
	HEREDOC, // <<
	ENV_VAR,
	EXIT_CODE,
	END
}	t_token;

typedef struct s_env
{
	int				index; // equale le nombre de l'environnement?
	char			*name;
	char			*value;

	struct s_env	*next;
}	t_env;

typedef struct s_lexer
{
	t_token			type;
	char			*value;
	struct s_lexer	*next;
}	t_lexer;

typedef struct s_parser
{
	char			**cmd; // execve recoit un **;
	int				(*builtin)(t_env **, struct s_parser *);
	int				num_redirections;
	t_lexer			*redirections;
	struct s_parser	*next;
	struct s_parser	*prev;
}	t_parser;

typedef struct s_exe
{
	char		**env;
	pid_t		*pid;
	char		*pathname;
	int			pipefd[2];
	int			nmb_cmd;
	int			fd[2];
	int			index_pid;
	int			prev_pipefd;
}	t_exe;

// Builtins
int			my_cd(t_env **env, struct s_parser *parser);
int			my_echo(t_env **env, struct s_parser *parser);
int			my_env(t_env **env, struct s_parser *parser);
int			my_exit(t_env **env, struct s_parser *parser);
int			my_export(t_env **env, struct s_parser *parser);
int			my_pwd(t_env **env, struct s_parser *parser);
int			my_unset(t_env **env, struct s_parser *parser);

// Lexer
void		add_token(t_lexer **head, t_lexer *new_token);
char		*concat(char *expanded_text, char *new_part);
t_lexer		*create_token(t_token type, char *value);
int			handle_double_quotes(int i, char *input, t_env *env, char **word);
int			handle_pipe_l(int i, t_lexer **tokens);
int			handle_regular_text(int i, char *input, t_env *env, char **word);
int			handle_redirection_in(int i, const char *input, t_lexer **tokens);
int			handle_redirection_out(int i, const char *input, t_lexer **tokens);
int			handle_single_quotes(int i, char *input, char **word);
int			handle_word(int i, char *input, t_lexer **tokens, t_env *env);
t_lexer		*lexer(char *input, t_env **env);
char		*replace_env_variables(const char *input, t_env *env);
char		*create_heredoc(char *delimiter);

// Parser
void		add_argument_to_cmd(t_parser *cmd, char *arg);
void		add_cmd(t_parser **head, t_parser *new_cmd);
void		add_redirection(t_parser *cmd, int redir_type, char *redir_value);
t_parser	*create_cmd(void);
int			(*get_builtin_function(char *cmd))(t_env **, t_parser *);
void		handle_command(t_parser **current_cmd, t_lexer *tokens);
t_lexer		*handle_redirection(t_parser **current_cmd, t_lexer *tokens);
int			handle_pipe_p(t_parser **a, t_parser **b, t_lexer *c, t_env **d);
int			is_builtin(char *cmd);
t_parser	*parse_lexer(t_lexer *tokens, t_env **env);

// Main
int			add_node(t_env **env, t_env *nw, int ret);
void		free_cmds(t_parser *cmds);
void		free_env(t_env *env);
void		free_tokens(t_lexer *tokens);
void		free_array(char	**str);
void		free_exe(t_exe *exe);
char		*ft_getenv(char *var_name, t_env *env);
t_env		*get_env(char **initial_env);
char		*get_value(char *name, t_env *env);
t_env		*ft_new_node(char *name, char *value);
int			max_index(t_env *env);
void		print_env(t_env *env);
void		print_export(t_env *env);
void		print_parser(t_parser *cmds);
void		setup_signals(int in_child);
void		signal_handler(int signum);
char		*get_user_input(const char *prompt);

// Executor
int			executor(t_env **env, t_parser *cmds);
t_exe		*init_exe(t_env *env, t_parser *cmds, int *exit_code);
t_exe		*init_exe_second(t_env *env, t_exe *exe, int *exit_code);
void		pipex(t_exe *exe);
char		*get_pathname(t_env *env_lst);
char		*find_path(char *pathname, char *cmd);
char		**trans_env(t_env	*env_lst);
char		*check_path(t_exe *exe, t_parser *cmd);
int			pipeline(t_exe *exe, t_parser *cmds, t_env **env);
int			exec_commande(t_exe *exe, t_parser *cmds, t_env **env);
int			handle_redir(t_exe *exe, t_parser *cmds);
void		exc_solo_cmd(t_exe *exe, t_parser *cmds, t_env **env);
int			handle_redir_solo(t_exe *exe, t_parser *cmds);
int			open_input_redirection(t_lexer *redirection);
int			open_output_redirection(t_lexer *redirection);
void		pipex(t_exe *exe);
char		*join_path_and_cmd(const char *dir, const char *cmd);
int			handle_waitpid(t_exe *exe);
#endif
