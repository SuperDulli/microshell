/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   microshell.h                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chelmerd <chelmerd@student.42wolfsburg.de> +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/11 12:02:19 by chelmerd          #+#    #+#             */
/*   Updated: 2022/07/11 12:15:08 by chelmerd         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MICROSHELL_H
# define MICROSHELL_H

# include <stdlib.h>
# include <sys/wait.h> // waitpid
# include <string.h> // strcmp
# include <unistd.h> // dup, pipe, write, ...

# define SUCCESS 0
# define ERROR 1

# define STDIN 0
# define STDOUT 1
# define STDERR 2

# define READ 0
# define WRITE 1

enum e_type {
	STD = 0,
	PIPE_OUT,
	END	
};

struct s_lst {
	char			**args;
	int				argc;
	enum e_type		type;
	struct s_lst	*next;
	struct s_lst	*prev;
};
typedef struct s_lst	t_lst;


#endif
