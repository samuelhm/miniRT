/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shurtado <shurtado@student.42barcelona.fr> +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/24 14:13:09 by erigonza          #+#    #+#             */
/*   Updated: 2025/01/23 12:22:29 by shurtado         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "rt.h"

int	er(t_data *data, const char *s, const char *argv)
{
	fprintf(stderr, "%s", RED);
	if (s)
		fprintf(stderr, "%s\n", s);
	fprintf(stderr, "%s", BOLD);
	if (argv)
		fprintf(stderr, "%s\n", argv);
	fprintf(stderr, "%s", RESET);
	free(data->console);
	if (data->args)
		free_strs(data->args);
	free_willy_lst(data);
	return (1);
}

int	type_obj(char *str)
{
	static const char	*bts[] = {"sp", "pl", "cy", "A", "C", "L", "co", "cu", \
		NULL};
	int				i;

	i = 0;
	while (bts[i] && !str_cmp(bts[i], str))
		i++;
	if (bts[i])
		return (i);
	return (50);
}

t_obj	*new_obj(t_data *data)
{
	t_obj	*tmp;

	tmp = calloc(1, sizeof(*tmp));
	if (!tmp)
		exit(er(data, "error: new_obj: malloc", NULL));
	tmp->next = NULL;
	return (tmp);
}

void	objadd_back(t_obj **lst, t_obj *new)
{
	t_obj	*current;

	if (lst)
	{
		if (*lst)
		{
			current = *lst;
			while (current->next != NULL)
				current = current->next;
			current->next = new;
		}
		else
			*lst = new;
	}
}
