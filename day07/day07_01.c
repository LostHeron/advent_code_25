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
size_t	get_split(t_vector lines);

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
	res = get_split(lines);
	i = 0;	
	while (i < lines.size)
	{
		printf("lines: %s", ((char **)lines.data)[i]);
		i++;
	}
	printf("res = %zu\n", res);
	ft_vector_free(&lines);
	return (0);
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
