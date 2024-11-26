/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   src.h                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: peli <peli@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/29 13:57:26 by tbartocc          #+#    #+#             */
/*   Updated: 2024/11/25 15:39:48 by peli             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SRC_H
# define SRC_H

# include "./Printf/printf.h"
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
	int				(*builtin)(t_env *, struct s_parser *);
	int				num_redirections;
	// char			*hd_file_name; stock in a pipe instead of a file;
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
	int			hd_pipe[2];
}	t_exe;

// Builtins
int			my_cd(t_env *env, struct s_parser *parser);
int			my_echo(t_env *env, struct s_parser *parser);
int			my_env(t_env *env, struct s_parser *parser);
int			my_exit(t_env *env, struct s_parser *parser);
int			my_export(t_env *env, struct s_parser *parser);
int			my_pwd(t_env *env, struct s_parser *parser);
int			my_unset(t_env *env, struct s_parser *parser);

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

// Parser
void		add_argument_to_cmd(t_parser *cmd, char *arg);
void		add_cmd(t_parser **head, t_parser *new_cmd);
void		add_redirection(t_parser *cmd, int redir_type, char *redir_value);
t_parser	*create_cmd(void);
int			(*get_builtin_function(char *cmd))(t_env *, t_parser *);
void		handle_command(t_parser **current_cmd, t_lexer *tokens);
t_lexer		*handle_redirection(t_parser **current_cmd, t_lexer *tokens);
int			handle_pipe_p(t_parser **parser, t_parser **cmd, t_lexer *tokens);
int			is_builtin(char *cmd);
t_parser	*parse_lexer(t_lexer *tokens);

// Main
void		free_cmds(t_parser *cmds);
void		free_env(t_env *env);
void		free_tokens(t_lexer *tokens);
char		*ft_getenv(char *var_name, t_env *env);
void		print_env(t_env *env);
t_env		*get_env(char **initial_env);
void		print_parser(t_parser *cmds);
void		setup_signals(void);

// Excutor
int			excutor(t_env *env, t_parser *cmds);
t_exe		*init_exe(t_env *env, t_parser *cmds);
char		*get_pathname(t_env *env_lst);
char		**trans_env(t_env	*env_lst);
int			pipeline(t_exe *exe, t_parser *cmds);
int			exec_commande(t_exe *exe, t_parser *cmds);
int			handle_redir(t_exe *exe, t_parser *cmds);
void		redir_heredoc(t_exe *exe, t_parser *cmds);
#endif
