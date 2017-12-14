/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: earteshc <earteshc@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/12/14 15:21:44 by earteshc          #+#    #+#             */
/*   Updated: 2017/12/14 18:39:48 by earteshc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line.h"

t_fd *lst_new(int fd, char *content, t_fd *prev)
{
	t_fd *res;
	res = (t_fd *)malloc(sizeof(t_fd));
	res->fd = fd;
	res->buf = ft_strdup(content);
	res->next = NULL;
	res->prev = prev;
	return (res);
}

void    check_stack(int fd, t_fd **stack, size_t *i, int flag)
{
	t_fd *start;
	
	if (flag == 1)
	{
		while ((*stack)->buf[++(*i)] != '\n')
			if ((*stack)->buf[*i] == '\0')
				return ;
		return ;
	}
	start = *stack;
	if (!*stack)
	{
		*stack = lst_new(fd, "", NULL);
		return ;
	}
	while ((*stack)->prev != NULL)
		*stack = (*stack)->prev;
	while ((*stack)!= NULL)
	{
		if ((*stack)->fd == fd)
			return ;
		*stack = (*stack)->next;
	}
	*stack = lst_new(fd, "", start);
	start->next = *stack;
}

int write_to_line(char **line, t_fd *stack, size_t i)
{
	char    *tmp_str;
	size_t    orig_i;
	size_t    j;
	
	orig_i = i;
	j = (size_t)-1;
	if (!(*line = (char *)malloc(sizeof(char) * i + 1)))
		return (-1);
	(*line)[i] = '\0';
	while (++j < orig_i)
		(*line)[j] = stack->buf[j];
	if (!(tmp_str = (char *)malloc(sizeof(char) * (ft_strlen(stack->buf) - i + 1))))
		return (-1);
	tmp_str[ft_strlen(stack->buf) - i ] = '\0';
	j = 0;
	while (j < ft_strlen(stack->buf) - orig_i)
		tmp_str[j++] = stack->buf[++i];
	free(stack->buf);
	stack->buf = ft_strdup(tmp_str);
	free(tmp_str);
	return (1);
}

int     fill_stack_buf(t_fd *stack, char  **buf, ssize_t r_res)
{
	size_t i;
	size_t j;
	char *tmp;
	int res;
	
	j = (size_t )-1;
	i = 0;
	res = 1;
	while ((*buf)[i] != '\n' && i < (size_t)r_res)
		i++;
	if ((*buf)[i] != '\n')
		res = 0;
	if(!(tmp = (char *)malloc(sizeof(char) * r_res + ft_strlen(stack->buf) + 1)))
		return (-1);
	tmp[r_res + ft_strlen(stack->buf)] = '\0';
	while (++j < ft_strlen(stack->buf))
		tmp[j] = stack->buf[j];
	i = 0;
	while (i < (size_t)r_res)
		tmp[j++] = (*buf)[i++];
	stack->buf = ft_strdup(tmp);
	free(tmp);
	return (res);
}

int     get_next_line(int fd, char **line)
{
	static t_fd *stack;
	size_t i;
	ssize_t r_res;
	char *buf;
	
	i = (size_t) - 1;
	check_stack(fd, &stack, &i, 0);
	check_stack(fd, &stack, &i, 1);
	if (stack->buf[i] == '\n')
		return write_to_line(&(*line), stack, i);
	if(!(buf = (char *)malloc(sizeof(char) * BUFF_SIZE + 1)))
		return (-1);
	if((r_res = read(fd, buf, BUFF_SIZE)) == -1)
	{
		free(buf);
		return (-1);
	}
	if(r_res == 0)
	{
		free(buf);
		return (0);
	}
	while (fill_stack_buf(stack, &buf, r_res) == 0)
		if (!(r_res = read(fd, buf, BUFF_SIZE)))
			if (ft_strlen(stack->buf) > 0)
				return (write_to_line(&(*line), stack, ft_strlen(stack->buf)));
	check_stack(fd, &stack, &i, 1);
	if (stack->buf[i] == '\n')
		return (write_to_line(&(*line),stack, i));
	return (0);
}
