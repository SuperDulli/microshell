/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   microshell.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chelmerd <chelmerd@student.42wolfsburg.de> +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/11 12:11:17 by chelmerd          #+#    #+#             */
/*   Updated: 2022/07/11 13:23:08 by chelmerd         ###   ########.fr       */
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
	write(STDERR, "error: ", 8);
	write(STDERR, msg, ft_strlen(msg));
}

void	exit_fatal(void)
{
	print_error("fatal\n");
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
				type = PIPE_OUT;
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
	// lst_rewind(&cmds);
	parse_input(argc, argv, &cmds);
	show_lst(cmds);

	return (SUCCESS);
}
