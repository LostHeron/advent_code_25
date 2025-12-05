#include <stddef.h>
#include <stdio.h>
#include <fcntl.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include "ft_io.h"
#include "ft_string.h"
#include "ft_vectors.h"

void	free_char_star(t_vector *ptr_lines);
int		get_file_content(t_vector *ptr_lines, char *filename);
size_t	get_nb_rolls(t_vector lines_vec, t_vector lines_vec_cpy);
int		get_lines_cpy(t_vector lines, t_vector *ptr_lines_cpy);

int	main(int argc, char **argv)
{
	int			ret;
	t_vector	lines;
	t_vector	lines_cpy;
	size_t		res;

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
	ft_vector_init(&lines_cpy, 200, sizeof(char *), free_char_star);
	get_lines_cpy(lines, &lines_cpy);
	res = get_nb_rolls(lines, lines_cpy);
	printf("total res = %zu\n", res);
	ft_vector_free(&lines);
	ft_vector_free(&lines_cpy);
	return (0);
}

size_t get_nb_rolls(t_vector lines_vec, t_vector lines_vec_cpy)
{
	ssize_t	len;
	char	**lines;
	char	**lines_cpy;
	ssize_t	nb_lines;
	ssize_t	i;
	ssize_t	j;
	size_t	res;

	lines = lines_vec.data;
	lines_cpy = lines_vec_cpy.data;
	nb_lines = (ssize_t) lines_vec.size - 1;
	len = (ssize_t)strlen(lines[0]) - 1;
	res = 1;
	size_t total_res = 0;
	//while (res != 0)
	while (res != 0)
	{
		/*
		size_t	tmp_i = 0;
		while (tmp_i < lines_vec.size)
		{
			printf("%s", ((char **)lines_vec.data)[tmp_i]);
			tmp_i++;
		}
		*/
		res = 0;
		i = 0;
		while (i < nb_lines)
		{
			j = 0;
			while (j < len)
			{
				int	count = 0;
				ssize_t	dec_i = -1;
				while (dec_i <= 1)
				{
					ssize_t dec_j = -1;
					while (dec_j <= 1)
					{
						ssize_t tmp_i;
						ssize_t tmp_j;
						tmp_i = i + dec_i;
						tmp_j = j + dec_j;
						if (tmp_i >= 0 && tmp_i < nb_lines && tmp_j >= 0 && tmp_j < len)
						{
							if (tmp_j == j && tmp_i == i)
								;
							else
							{
								//printf("tmp_i = %zd, tmp_j = %zd\n", tmp_i, tmp_j);
								if (lines[tmp_i][tmp_j] == '@')
									count++;
							}
						}
						dec_j++;
					}
					dec_i++;
				}
				//printf("count = %i\n", count);
				if (lines[i][j] == '@' && count < 4)
				{
					lines_cpy[i][j] = '.';
					res++;
				}
				//printf("res = %zd\n", res);
				//printf("\n");
				j++;
			}
			i++;
		}
		total_res += res;
		i = 0;
		while (i < nb_lines)
		{
			j = 0;
			while (j < len)
			{
				lines[i][j] = lines_cpy[i][j];
				j++;
			}
			i++;
		}
	}
	return (total_res);
}

int	get_lines_cpy(t_vector lines, t_vector *ptr_lines_cpy)
{
	size_t	i;
	char	*tmp_line;
	int		ret;

	i = 0;
	while (i < lines.size)
	{
		if (((char **)lines.data)[i] != NULL)
			tmp_line = ft_strdup(((char **)lines.data)[i]);
		else
			tmp_line = NULL;
		ret = ft_vector_add_single(ptr_lines_cpy, &tmp_line);
		if (ret != 0)
			return (1);
		i++;
	}
	return (0);
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
