/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shurtado <shurtado@student.42barcelona.fr> +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/19 10:53:01 by erigonza          #+#    #+#             */
/*   Updated: 2025/01/13 11:58:16 by shurtado         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "miniRT.h"

void	correct_file(t_data *data, char *name)
{
	int	i;

	i = 0;
	while (name[i] != '.' && name[i])
		i++;
	if (name[i + 3] || !name[i - 1])
		exit(er(data, "error: correct_file: wrong file name -> ", name));
	else if (name[i] == '.' && name[i + 1] == 'r' && name[i + 2] == 't')
		return ;
	exit(er(data, "error: correct_file: wrong file name -> ", name));
}

void	validate_args_and_open(t_data *data, int ac, char **av, int *fd)
{
	if (ac != 2)
		exit(er(data, "error: 2 args needed", NULL));
	correct_file(data, av[1]);
	*fd = open(av[1], O_RDONLY);
	if (*fd < 0)
		exit(er(data, "error: fd failed", NULL));
}

void	parse(t_data *data, int fd)
{
	int		type;
	char	*str;

	str = NULL;
	while (true)
	{
		if (str)
			free(str);
		str = get_next_line(fd);
		if (!str)
			break ;
		if (str[0] == '#')
			continue ;
		type = type_obj(str);
		if (type > 7)
			exit(er(data, "error: parse: wrong map: obj type", str));
		else if ((type <= 2 || type == CO || type == CU) && IS_SPACE(str[2]))
			objadd_back(&data->obj, create_obj(data, str, type));
		create_alight(data, str, type);
		create_cam(data, str, type);
		create_slight(data, &data->s_light, str, type);
	}
	check_params(data);
}
