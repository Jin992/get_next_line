/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.h                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: earteshc <earteshc@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/12/14 15:20:46 by earteshc          #+#    #+#             */
/*   Updated: 2017/12/14 16:54:56 by earteshc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef GET_NEXT_LINE_H
# define GET_NEXT_LINE_H
# include <stdlib.h>
# include <unistd.h>
# include <fcntl.h>
# include "libft/includes/libft.h"
# include <stdio.h>

# define BUFF_SIZE 1

typedef struct 	s_fd
{
	int			fd;
	char		*buf;
	struct s_fd *next;
	struct s_fd *prev;
	
}				t_fd;

int     get_next_line(int fd, char **line);
int     fill_stack_buf(t_fd *stack, char  **buf, ssize_t r_res);
int		write_to_line(char **line, t_fd *stack, size_t i);
void    check_stack(int fd, t_fd **stack, size_t *i, int flag);
t_fd	*lst_new(int fd, char *content, t_fd *prev);


#endif
