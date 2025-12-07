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

typedef struct s_beam
{
	char	content;
	size_t	nb;
}	t_beam;

void	free_char_star(t_vector *ptr_lines);
void	free_copy(t_vector *ptr_lines);
int		get_file_content(t_vector *ptr_lines, char *filename);
void	fill_beam_array(t_beam *beam_array, t_vector vec_lines, size_t line_len);
void	print_beam_array(t_beam *beam_array, size_t nb_lines, size_t line_len);
size_t	calc_beam_possibility(t_beam *beam_array, size_t nb_lines, size_t line_len);

int	main(int argc, char **argv)
{
	int			ret;
	t_vector	lines;
	t_beam		*beam_array;
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
	res = 0;
	size_t	line_len = strlen(((char **)lines.data)[0]);
	line_len--;
	beam_array = malloc(sizeof(t_beam) * lines.size * line_len);
	fill_beam_array(beam_array, lines, line_len);
	printf("beam_array : \n");
	print_beam_array(beam_array, lines.size, line_len);
	res = calc_beam_possibility(beam_array, lines.size, line_len);
	printf("beam_array after calc: \n");
	print_beam_array(beam_array, lines.size, line_len);
	printf("res = %zu\n", res);
	ft_vector_free(&lines);
	return (0);
}

size_t	calc_beam_possibility(t_beam *beam_array, size_t nb_lines, size_t line_len)
{
	size_t	line_i;
	size_t	char_i;

	line_i = 0;
	while (line_i < nb_lines - 1)
	{
		char_i = 0;
		while (char_i < line_len)
		{
			if (beam_array[line_i * line_len + char_i].content == '|')
			{
				if (beam_array[(line_i + 1) * line_len + char_i].content == '^')
				{
					beam_array[(line_i + 1) * line_len + (char_i + 1)].content = '|';
					beam_array[(line_i + 1) * line_len + (char_i + 1)].nb += beam_array[line_i * line_len + char_i].nb;
					beam_array[(line_i + 1) * line_len + (char_i - 1)].content = '|';
					beam_array[(line_i + 1) * line_len + (char_i - 1)].nb += beam_array[line_i * line_len + char_i].nb;
				}
				else
				{
					beam_array[(line_i + 1) * line_len + (char_i)].content = '|';
					beam_array[(line_i + 1) * line_len + (char_i)].nb += beam_array[line_i * line_len + char_i].nb;
				}
			}
			char_i++;
		}
		line_i++;
	}
	size_t	res = 0;
	char_i = 0;
	while (char_i < line_len)
	{
		if (beam_array[line_i * line_len + char_i].content == '|')
		{
			res += beam_array[line_i * line_len + char_i].nb;
		}
		char_i++;
	}
	printf("res = %zu\n", res);
	return (res);
}

void	fill_beam_array(t_beam *beam_array, t_vector vec_lines, size_t line_len)
{
	size_t	line_i;
	size_t	char_i;
	char	**lines;

	line_i = 0;
	lines = vec_lines.data;
	while (line_i < vec_lines.size)
	{
		char_i = 0;
		while (char_i < line_len)
		{
			if (lines[line_i][char_i] == 'S')
			{
				beam_array[line_i * line_len + char_i].content = '|';
				beam_array[line_i * line_len + char_i].nb = 1;
			}
			else
			{
				beam_array[line_i * line_len + char_i].content = lines[line_i][char_i];
				beam_array[line_i * line_len + char_i].nb = 0;
			}
			char_i++;
		}
		line_i++;
	}
}

void print_beam_array(t_beam *beam_array, size_t nb_lines, size_t line_len)
{
	size_t	line_i;
	size_t	char_i;

	line_i = 0;
	while (line_i < nb_lines)
	{
		char_i = 0;
		while (char_i < line_len)
		{
			printf("%c", beam_array[line_i * line_len + char_i].content);
			char_i++;
		}
		printf("\n");
		line_i++;
	}
}

t_beam *get_beam(t_beam *beam_array, size_t	width, size_t x, size_t y)
{
	return (beam_array + (y * width + x));
}

size_t	get_split(t_vector lines)
{
	size_t	line_i;
	size_t	char_i;
	char	*line_tmp;
	size_t	nb_split;

	nb_split = 0;
	line_i = 0;
	while (line_i < lines.size - 1)
	{
		line_tmp = ((char **)lines.data)[line_i];
		char_i = 0;
		while (line_tmp[char_i] != '\n')
		{
			if (((char **)lines.data)[line_i][char_i] == 'S' || ((char **)lines.data)[line_i][char_i] == '|')
			{
				if (((char **)lines.data)[line_i + 1][char_i] == '.')
				{
					((char **)lines.data)[line_i + 1][char_i] = '|';
				}
				else if (((char **)lines.data)[line_i + 1][char_i] == '^')
				{
					((char **)lines.data)[line_i + 1][char_i - 1] = '|';
					((char **)lines.data)[line_i + 1][char_i + 1] = '|';
					nb_split+=1;
				}
			}
			char_i++;
		}
		line_i++;
	}
	return (nb_split);
}

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
		if (line_tmp != NULL)
		{
			ret = ft_vector_add_single(ptr_lines, &line_tmp);
			if (ret != 0)
				return (1);
		}
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
