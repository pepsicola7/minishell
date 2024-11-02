/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   src.h                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tbartocc <tbartocc@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/29 13:57:26 by tbartocc          #+#    #+#             */
/*   Updated: 2024/11/01 19:07:00 by tbartocc         ###   ########.fr       */
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
	SINGLE,
	DOUBLE,
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
	int				index;
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
	char			**str;
	int				(*builtin)(t_env *, struct s_parser *);
	int				num_redirections;
	char			*hd_file_name;
	t_lexer			*redirections;
	struct s_parser	*next;
	struct s_parser	*prev;
}	t_parser;

t_lexer		*lexer(char *input, t_env **env);
t_lexer		*create_token(t_token type, char *value);
void		add_token(t_lexer **head, t_lexer *new_token);
void		free_cmds(t_parser *cmds);
void		free_tokens(t_lexer *tokens);
char		*ft_getenv(char *var_name, t_env *env);
int 		(*get_builtin_function(char *cmd))(t_env *, t_parser *);
t_env		*get_env(char **initial_env);
int			handle_pipe(int i, t_lexer **tokens);
int			handle_single_quotes(int i, const char *input, t_lexer **tokens);
int			handle_double_quotes(int i, const char *input, t_lexer **tokens, t_env **env);
int			handle_environement_var(int i, char *input, t_lexer **tokens);
int			handle_redirection_in(int i, const char *input, t_lexer **tokens);
int			handle_redirection_out(int i, const char *input, t_lexer **tokens);
int			handle_word(int i, const char *input, t_lexer **tokens/*, t_env **env*/);
int			is_builtin(char *cmd);
int			is_special_char(char c);
int			my_env(t_env *env, struct s_parser *parser);
t_parser	*parse_lexer(t_lexer *tokens);
char		*replace_env_variables(const char *input, t_env *env);
void		setup_signals(void);

#endif
