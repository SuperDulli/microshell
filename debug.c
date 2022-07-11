/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   debug.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chelmerd <chelmerd@student.42wolfsburg.de> +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/11 12:23:34 by chelmerd          #+#    #+#             */
/*   Updated: 2022/07/11 13:00:55 by chelmerd         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "microshell.h"

#include <stdio.h>

void	show_lst(t_lst *lst)
{
	int	i;

	printf("%u, argc=%d\n\t", lst->type, lst->argc);
	i = 0;
	while (i < lst->argc)
	{
		printf("#%d: %s,", i, lst->args[i]);
		i++;
	}
	printf("\n");
	if (lst->next)
		show_lst(lst->next);
}
