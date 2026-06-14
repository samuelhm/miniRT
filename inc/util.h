#ifndef UTIL_H
# define UTIL_H

# include <stddef.h>

typedef struct s_list
{
	void			*content;
	struct s_list	*next;
}	t_list;

char	**free_strs(char **cmd);
char	*get_next_line(int fd);
char	*str_join(char const *s1, char const *s2);
char	**str_split(char const *s, char c);
char	*str_sub(char const *s, unsigned int start, size_t len);
char	*str_trim(char const *s1, char const *set);

t_list	*lst_new(void *content);
void	lst_add_front(t_list **lst, t_list *new);
void	lst_add_back(t_list **lst, t_list *new);
void	lst_clear(t_list **lst, void (*del)(void *));

#endif
