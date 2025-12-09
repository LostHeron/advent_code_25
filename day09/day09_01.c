#include <stddef.h>
#include <stdio.h>
#include <fcntl.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include "ft_io.h"
#include "ft_standard.h"
#include "ft_vectors.h"
#include <math.h>

#define TRUE 1
#define FALSE 0
#define MAX_CONNECTIONS 4536

typedef struct s_point
{
	ssize_t	x;
	ssize_t	y;
}	t_point;

void	free_char_star(t_vector *ptr_lines);
void	free_copy(t_vector *ptr_lines);
int		get_file_content(t_vector *ptr_lines, char *filename);

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
	res = 0;
	printf("res = %zu\n", res);
	t_vector	points;
	t_point		tmp_point;
	char		**nbs;
	ft_vector_init(&points, 200, sizeof(t_point), NULL);
	i = 0;
	while (i < lines.size)
	{
		nbs = ft_split(((char **)lines.data)[i], ",\n");
		tmp_point.x = atoi(nbs[0]);
		tmp_point.y = atoi(nbs[1]);
		ft_vector_add_single(&points, &tmp_point);
		ft_split_free(nbs);
		i++;
	}
	i = 0;
	while (i < points.size)
	{
		printf("%zu, %zu\n", ((t_point *)points.data)[i].x, ((t_point *)points.data)[i].y);
		i++;
	}
	ssize_t	area;
	ssize_t	area_tmp;
	area = 0;
	i = 0;
	while (i < points.size)
	{
		size_t j = 0;
		while (j < points.size)
		{
			if (i != j)
			{
				area_tmp = (llabs(((t_point *)points.data)[i].x - ((t_point *)points.data)[j].x) + 1)
						* (llabs(((t_point *)points.data)[i].y - ((t_point *)points.data)[j].y) + 1);
				if (area_tmp > area)
					area = area_tmp;
			}
			j++;
		}
		i++;
	}
	printf("area = %zu\n", area);
	ft_vector_free(&lines);
	return (0);
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
