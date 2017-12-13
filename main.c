#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <fcntl.h>

#define BUFF_SIZE 150
typedef struct 	s_fd
{
    int			fd;
    int 		buf_size;
    char		*buf;
    struct s_fd *next;
    struct s_fd *prev;

}				t_fd;

t_fd *lst_new(int fd, char *content, t_fd *prev)
{
    t_fd *res;
    res = (t_fd *)malloc(sizeof(t_fd));
    res->fd = fd;
    res->buf = strdup(content);
    res->next = NULL;
    res->prev = prev;
    return (res);
}

void    check_stack(int fd, t_fd **stack)
{
	t_fd *start;

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
        return (0);
    (*line)[i] = '\0';
    while (++j < orig_i)
        (*line)[j] = stack->buf[j];
    if (!(tmp_str = (char *)malloc(sizeof(char) * (strlen(stack->buf) - i + 1))))
        return (0);
    tmp_str[strlen(stack->buf) - i ] = '\0';
    j = 0;
    while (j < strlen(stack->buf) - orig_i)
        tmp_str[j++] = stack->buf[++i];
    free(stack->buf);
    stack->buf = strdup(tmp_str);
    free(tmp_str);
    return (1);
}

int     fill_stack_buf(t_fd *stack, char const *buf, ssize_t r_res)
{
    size_t i;
    size_t j;
    char *tmp;
    int res;

    j = (size_t )-1;
    i = 0;
    res = 1;
    while (buf[i] != '\n' && i < r_res)
        i++;
    if (buf[i] != '\n')
        res = 0;
    if(!(tmp = (char *)malloc(sizeof(char) * r_res + strlen(stack->buf) + 1)))
        return (-1);
    tmp[r_res + strlen(stack->buf)] = '\0';
    while (++j < strlen(stack->buf))
        tmp[j] = stack->buf[j];
    i = 0;
    while (i < r_res)
        tmp[j++] = buf[i++];
    stack->buf = strdup(tmp);
    free(tmp);
    return (res);
}

int     get_next_line(int fd, char **line)
{
    static t_fd *stack;
    size_t i;
    ssize_t r_res;
    char buf[BUFF_SIZE];

    i = (size_t)-1;
    check_stack(fd, &stack);
    while (stack->buf[++i] != '\n')
    	if (stack->buf[i] == '\0')
            break;
    if (stack->buf[i] == '\n')
        return write_to_line(&(*line), stack, i);
    else
    {
        if((r_res = read(fd, buf, BUFF_SIZE)) == -1)
			return (-1);
        while (fill_stack_buf(stack, buf, r_res) == 0)
            if (!(r_res = read(fd, buf, BUFF_SIZE)))
                if (r_res == 0 && strlen(stack->buf) > 0)
                    return (write_to_line(&(*line), stack, strlen(stack->buf)));
        while (stack->buf[++i] != '\n')
            if (stack->buf[i] == '\0')
                break;
        if (stack->buf[i] == '\n')
            return (write_to_line(&(*line),stack, i));
    }
    return (0);
}

int main(int argc, char **argv)
{
    char *line;
    int fd;
	int fd1;

    fd = open(*(argv + 1), O_RDONLY);
	fd1 = open(*(argv + 2), O_RDONLY);

    get_next_line(fd, &line);
    printf(" 1 1 %s|\n", line);
    free(line);
    line = NULL;
    get_next_line(fd, &line);
    printf(" 1 2 %s|\n", line);
    free(line);
    line = NULL;

	get_next_line(fd1, &line);
	printf(" 2 1 %s|\n", line);
	free(line);
	line = NULL;
	get_next_line(fd1, &line);
	printf(" 2 2 %s|\n", line);
	free(line);
	line = NULL;

	get_next_line(fd, &line);
	printf(" 1 3 %s|\n", line);
	get_next_line(fd1, &line);
	printf(" 2 3 %s|\n", line);

	close(fd);
	close(fd1);
 return (0);
}