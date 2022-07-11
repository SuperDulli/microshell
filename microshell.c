/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   microshell.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chelmerd <chelmerd@student.42wolfsburg.de> +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/11 12:11:17 by chelmerd          #+#    #+#             */
/*   Updated: 2022/07/11 14:18:55 by chelmerd         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "microshell.h"

#include "debug.c"

size_t	ft_strlen(const char *s)
{
	size_t	len;

	len = 0;
	while (*s)
	{
		len++;
		s++;
	}
	return (len);
}

void	print_error(const char *msg)
{
	// write(STDERR, "error: ", 8);
	write(STDERR, msg, ft_strlen(msg));
}

void	exit_fatal(void)
{
	print_error("error: fatal\n");
	exit(ERROR);
}

t_lst	*new_lst(int argc, char **args, int type)
{
	t_lst	*new;

	new = malloc(sizeof(t_lst));
	if (!new)
		exit_fatal();
	new->argc = argc;
	// new->args = malloc(sizeof(char *) * argc);
	// if (!new->argc)
	// 	exit_fatal();
	new->args = args;
	new->type = type;
	new->pipe[READ] = -1;
	new->pipe[WRITE] = -1;
	new->next = NULL;
	new->prev = NULL;
	return (new);
}

void	lst_push(t_lst **lst, t_lst *elem)
{
	if (!*lst)
	{
		*lst = elem;
		return ;
	}
	while ((*lst)->next)
	{
		*lst = (*lst)->next;
	}
	(*lst)->next = elem;
	elem->prev = *lst;
}

void	lst_rewind(t_lst **lst)
{
	while (*lst && (*lst)->prev)
	{
		*lst = (*lst)->prev;
	}
}

char **arr_to_vec(int argc, char *args[])
{
	char	**result;
	int		i;

	result = malloc(sizeof(char *) * (argc + 1));
	if (!result)
		exit_fatal();
	i = 0;
	while (i < argc)
	{
		result[i] = args[i];
		i++;
	}
	result[i] = NULL;
	return (result);
}

int	parse_input(int argc, char *argv[], t_lst **cmds)
{
	int	i, j, type;

	i = 1;
	while (i < argc)
	{
		j = 0;
		type = 0;
		while (i + j < argc)
		{
			if (!strcmp(argv[i + j], "|"))
			{
				print_error("pipe found\n"); // debug
				type = PIPE;
				break ;
			}
			else if (!strcmp(argv[i + j], ";"))
			{
				print_error("semi colon found\n"); // debug
				type = END;
				break ;
			}
			j++;
		}
		if (!j)
		{
			print_error("j == 0");
			return (ERROR);
		}
		lst_push(cmds, new_lst(j, &argv[i], type));
		i += j + 1;
	}
	return (SUCCESS);
}

int	execute_cmd(t_lst *cmd, char *envp[])
{
	pid_t	pid;
	int		result;
	int		status;
	int		pipe_open;

	result = ERROR;
	pipe_open = 0;
	if (cmd->type == PIPE || (cmd->prev && cmd->prev->type == PIPE))
	{
		if (pipe(cmd->pipe) < 0)
			exit_fatal();
		pipe_open = 1;
	}
	
	pid = fork();
	if (pid < 0)
		exit_fatal();
	if (pid == 0) // child
	{
		if (cmd->type == PIPE
			&& dup2(cmd->pipe[WRITE], STDOUT) < 0)
			exit_fatal();
		if (cmd->prev && cmd->prev->type == PIPE
			&& dup2(cmd->prev->pipe[READ], STDIN) < 0)
			exit_fatal();
		result = execve(cmd->args[0], arr_to_vec(cmd->argc, cmd->args), envp);
		if (result < 0)
		{
			print_error("error: cannont execute ");
			print_error(cmd->args[0]);
			print_error("\n");
		}
		exit(result);
	}
	// parent
	waitpid(pid, &status, 0);
	if (pipe_open)
	{
		close(cmd->pipe[WRITE]);
		if (!cmd->next || cmd->type == END)
			close(cmd->pipe[READ]);
	}
	if (cmd->prev && cmd->prev->type == PIPE)
		close(cmd->prev->pipe[READ]);
	if (WIFEXITED(status))
		result = WEXITSTATUS(status);
	
	return result;
}

int	execute(t_lst *cmds, char *envp[])
{
	while (cmds)
	{
		execute_cmd(cmds, envp);
		cmds = cmds->next;
	}
	return SUCCESS;
}

int	main(int argc, char *argv[], char *envp[])
{
	t_lst	*cmds;

	cmds = NULL;
	(void) argc;
	(void) argv;
	(void) envp;
	
	if (argc < 2)
		return (SUCCESS);

	// lst_push(&cmds, new_lst(argc - 1, &argv[1], 0));
	parse_input(argc, argv, &cmds);
	lst_rewind(&cmds);
	show_lst(cmds);

	execute(cmds, envp);

	return (SUCCESS);
}
