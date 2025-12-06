#include <stddef.h>
#include <stdio.h>
#include <fcntl.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include "ft_io.h"
#include "ft_standard.h"
#include "ft_vectors.h"

#define TRUE 1
#define FALSE 0

void	free_char_star(t_vector *ptr_lines);
void	free_copy(t_vector *ptr_lines);
int		get_file_content(t_vector *ptr_lines, char *filename);
long long	get_sum(t_vector lines);

int	main(int argc, char **argv)
{
	int			ret;
	t_vector	lines;
	size_t		res;
	size_t		i;

	if (argc <= 1)
	{
		printf("no input file\n");
		return (1);
	}
	ft_vector_init(&lines, 200, sizeof(char *), free_char_star);
	ret = get_file_content(&lines, argv[1]);
	if (ret != 0)
	{
		ft_vector_free(&lines);
		return (1);
	}
	i = 0;	
	printf("lines.size = %zu\n", lines.size);
	while (i < lines.size)
	{
		printf("test lines: %s", ((char **)lines.data)[i]);
		if (((char **)lines.data)[i] != NULL)
			printf("test lines len: %zu\n", strlen(((char **)lines.data)[i]));
		i++;
	}
	printf("\n");
	res = get_sum(lines);
	printf("res = %zu\n", res);
	ft_vector_free(&lines);
	return (0);
}

int	lines_contain_only_spaces(char *line);

long long	get_sum(t_vector lines)
{
	long long	res;
	t_vector	lines_inverted;
	char		*line_tmp;
	size_t		col_i;
	size_t		line_i;
	size_t		i;

	ft_vector_init(&lines_inverted, 200, sizeof(char *), NULL);
	col_i = 0;
	while (((char **)lines.data)[0][col_i] != '\n')
	{
		line_tmp = malloc((lines.size + 1) * sizeof(char));
		line_i = 0;
		while (line_i < lines.size - 1)
		{
			line_tmp[line_i] = ((char **)lines.data)[line_i][col_i];
			line_i++;
		}
		line_tmp[line_i] = '\0';
		ft_vector_add_single(&lines_inverted, &line_tmp);
		col_i++;
	}
	i = 0;
	res = 0;
	while (i < lines_inverted.size)
	{
		printf("lines_inverted[%zu] = '%s'\n", i, ((char **)lines_inverted.data)[i]);
		char operation;
		char *line_tmp;
		long long tmp_calc;
		line_tmp = ((char **)lines_inverted.data)[i];
		if (line_tmp[lines.size - 2] == '+')
		{
			operation = '+';
			tmp_calc = 0;
		}
		else
		{
			operation = '*';
			tmp_calc = 1;
		}
		while (i < lines_inverted.size && lines_contain_only_spaces(line_tmp) == FALSE)
		{
			long long tmp_nb = atoll(line_tmp);
			printf("tmp_nb = %lld\n", tmp_nb);
			if (operation == '+')
				tmp_calc += tmp_nb;
			else
				tmp_calc *= tmp_nb;
			i++;
			line_tmp = ((char **)lines_inverted.data)[i];
		}
		printf("tmp_calc = %lld\n", tmp_calc);
		res += tmp_calc;
		i++;
	}
	return (res);
}

int	lines_contain_only_spaces(char *line)
{
	size_t	i;

	i = 0;
	while (line[i] != '\0')
	{
		if (line[i] != ' ')
			return (FALSE);
		i++;
	}
	return (TRUE);
}

typedef struct s_token
{
	char	*addr;
	char	content;
}		t_token;

int	get_file_content(t_vector *ptr_lines, char *filename)
{
	int		fd;
	int		ret;
	char	*line_tmp;

	fd = open(filename, O_RDONLY);
	if (fd < 0)
	{
		printf("could not open file : %s\n", filename);
		return (1);
	}
	line_tmp = (char *)0x01;
	while (line_tmp != NULL)
	{
		line_tmp = get_next_line(fd, &ret);
		if (ret != 0)
			return (1);
		ret = ft_vector_add_single(ptr_lines, &line_tmp);
		if (ret != 0)
			return (1);
	}
	return (0);
}

void	free_char_star(t_vector *ptr_lines)
{
	char	**lines;
	size_t	i;

	lines = (char **)ptr_lines->data;
	i = 0;
	while (i < ptr_lines->size)
	{
		free(lines[i]);
		i++;
	}
	free(ptr_lines->data);
	return ;
}

void	free_copy(t_vector *ptr_lines)
{
	free(ptr_lines->data);
	return ;
}
