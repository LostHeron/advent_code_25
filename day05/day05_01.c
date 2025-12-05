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
size_t	get_valid_ids(t_vector ranges, t_vector nbv);
int		is_valid_id(long long nb, t_vector ranges, size_t nb_line);

int	main(int argc, char **argv)
{
	int			ret;
	t_vector	lines;
	t_vector	ranges;
	t_vector	nb;
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
	ft_vector_init(&ranges, 200, sizeof(char *), free_copy);
	ft_vector_init(&nb, 200, sizeof(char *), free_copy);
	i = 0;
	while (i < lines.size && ((char **)lines.data)[i][0] != '\n')
	{
		ret = ft_vector_add_single(&ranges, &((char **)lines.data)[i]);
		printf("ranges added : %s", ((char **)lines.data)[i]);
		i++;
	}
	i++;
	while (i < lines.size && ((char **)lines.data)[i] != NULL && ((char **)lines.data)[i][0] != '\n')
	{
		ret = ft_vector_add_single(&nb, &((char **)lines.data)[i]);
		printf("nb added : %s", ((char **)lines.data)[i]);
		i++;
	}
	i = 0;
	while (i < nb.size)
	{
		printf("%s", ((char **)nb.data)[i]);
		i++;
	}
	i = 0;
	while (i < ranges.size)
	{
		printf("%s", ((char **)ranges.data)[i]);
		i++;
	}
	res = get_valid_ids(ranges, nb);
	printf("res = %zu\n", res);
	ft_vector_free(&lines);
	ft_vector_free(&ranges);
	ft_vector_free(&nb);
	return (0);
}

size_t	get_valid_ids(t_vector ranges, t_vector nbv)
{
	size_t		i;
	long long	nb;
	size_t		res;

	i = 0;
	res = 0;
	while (i < nbv.size)
	{
		nb = atoll(((char **)nbv.data)[i]);
		printf("nb to check = %lld\n", nb);
		if (is_valid_id(nb, ranges, i + 1) == TRUE)
		{
			res++;
		}
		i++;
	}
	printf("ranges.size = %zu\n", ranges.size);
	printf("nb.size = %zu\n", nbv.size);
	return (res);
}

int	is_valid_id(long long nb, t_vector ranges, size_t nb_line)
{
	size_t		j;
	char		**nbs;
	long long	min;
	long long	max;

	j = 0;
	while (j < ranges.size)
	{
		nbs = ft_split(((char **)ranges.data)[j], "-");
		min = atoll(nbs[0]);
		max = atoll(nbs[1]);
		if (min <= nb && nb <= max)
		{
			//printf("%lld (line %zu), is valid id (falls into %lld - %lld (range line %zu))\n", nb, nb_line, min, max, j + 1);
			(void) nb_line;
			ft_split_free(nbs);
			return (TRUE);
		}
		ft_split_free(nbs);
		j++;
	}
	return (FALSE);
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
