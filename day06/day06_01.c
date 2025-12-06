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
	while (i < lines.size)
	{
		printf("lines: %s", ((char **)lines.data)[i]);
		i++;
	}
	printf("\n");
	res = get_sum(lines);
	printf("res = %zu\n", res);
	ft_vector_free(&lines);
	return (0);
}

long long	get_sum(t_vector lines)
{
	long long	res;
	t_vector	lines_plus;
	size_t		calc_i;
	size_t		line_i;
	char		**tmp_split;

	ft_vector_init(&lines_plus, 200, sizeof(char **), NULL);
	line_i = 0;
	while (line_i < lines.size)
	{
		if (((char **)lines.data)[line_i] != NULL)
		{
			tmp_split = ft_split(((char **)lines.data)[line_i], " \n");
			int tmp_split_i = 0;
			while (tmp_split[tmp_split_i] != NULL)
			{
				printf("tmp_split[%i] = %s\n", tmp_split_i, tmp_split[tmp_split_i]);
				tmp_split_i++;
			}
			ft_vector_add_single(&lines_plus, &tmp_split);
		}
		line_i++;
	}
	calc_i = 0;
	res = 0;
	while (((char ***)lines_plus.data)[0][calc_i] != NULL)
	{
		printf("calc of line beginning with '%s'\n", ((char ***)lines_plus.data)[0][calc_i]);
		long long tmp_calc = atoll(((char ***)lines_plus.data)[0][calc_i]);
		line_i = 1;	
		if (((char ***)lines_plus.data)[lines_plus.size - 1][calc_i][0] == '+')
		{
			while (line_i < lines_plus.size - 1)
			{
				long long to_add = atoll(((char ***)lines_plus.data)[line_i][calc_i]);
				printf("to_add = %lld\n", to_add);
				tmp_calc += to_add;
				line_i++;
			}
		}
		else
		{
			while (line_i < lines_plus.size - 1)
			{
				long long to_mult = atoll(((char ***)lines_plus.data)[line_i][calc_i]);
				printf("to_mult = %lld\n", to_mult);
				tmp_calc *= atoll(((char ***)lines_plus.data)[line_i][calc_i]);
				line_i++;
			}
		}
		res += tmp_calc;
		calc_i++;
	}
	return (res);
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
