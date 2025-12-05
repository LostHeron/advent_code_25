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
size_t	get_valid_ids(t_vector ranges);
int		is_valid_id(long long nb, t_vector ranges, size_t nb_line);
void	free_valid_ranges(t_vector *ptr_vec);
void	place_new_ranges(long long min, long long max, long long valid_ranges[1000][2], int *ptr_nb_valid_ranges);
void	sort_valide_ranges(long long valid_ranges[1000][2], int nb_valid_ranges);

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
	res = get_valid_ids(ranges);
	printf("res = %zu\n", res);
	ft_vector_free(&lines);
	ft_vector_free(&ranges);
	ft_vector_free(&nb);
	return (0);
}

size_t	get_valid_ids(t_vector ranges)
{
	size_t		i;
	char		**nbs;
	size_t		res;
	long long	min;
	long long	max;
	long long	valid_ranges[1000][2];
	int			nb_valid_ranges;

	i = 0;
	res = 0;
	nb_valid_ranges = 0;
	while (i < ranges.size)
	{
		nbs = ft_split(((char **)ranges.data)[i], "-");
		min = atoll(nbs[0]);
		max = atoll(nbs[1]);
		place_new_ranges(min, max, valid_ranges, &nb_valid_ranges);
		ft_split_free(nbs);
		i++;
	}
	return (res);
}

void	place_new_ranges(long long min, long long max, long long valid_ranges[1000][2], int *ptr_nb_valid_ranges)
{
	int	valid_ranges_i;

	valid_ranges_i = 0;
	while (valid_ranges_i < *ptr_nb_valid_ranges)
	{
		// pour chaque range i, on va regarder si min ou max est dans la range,
		// si les deux sont dedans on fait rient,
		// si une des deux est dedans on modifi l'autre accordingly
		// sinon, on ajoute au bonne endroit !

		if (valid_ranges[valid_ranges_i][0] <= min && min <= valid_ranges[valid_ranges_i][1] && valid_ranges[valid_ranges_i][0] <= max && max <= valid_ranges[valid_ranges_i][1])
		{
			return ;
		}
		else if (valid_ranges[valid_ranges_i][0] <= min && min <= valid_ranges[valid_ranges_i][1])
		{
			// uniquement min est comprise dans la range et max sort a droite!
			// must check if max is within next range 
			if (valid_ranges_i != *ptr_nb_valid_ranges - 1)
			{

			}
			valid_ranges[valid_ranges_i][1] = max;
			return ;
		}
		else if (valid_ranges[valid_ranges_i][0] <= max && max <= valid_ranges[valid_ranges_i][1])
		{
			// uniquement max est comprise dans la range et min sort a gauche
			valid_ranges[valid_ranges_i][0] = min;
			return ;
		}
		else
		{
			// min et max ne sont pas dans la range, il faut continuer a checker si c'est dans les suivantes 
			continue;
		}
	}
	if (*ptr_nb_valid_ranges == 0)
	{
		valid_ranges[*ptr_nb_valid_ranges][0] = min;
		valid_ranges[*ptr_nb_valid_ranges][1] = max;
		*ptr_nb_valid_ranges++;
		sort_valide_ranges(valid_ranges, *ptr_nb_valid_ranges);
	}
	return ;
}

void	sort_valide_ranges(long long valid_ranges[1000][2], int nb_valid_ranges)
{
	int			i;
	int			j;
	long long	range_tmp[2];

	j = 0;
	while (j < nb_valid_ranges)
	{
		i = 0;
		while (i < nb_valid_ranges - 1)
		{
			if (valid_ranges[i][0] > valid_ranges[i + 1][0])
			{
				range_tmp[0] = valid_ranges[i][0];
				range_tmp[1] = valid_ranges[i][1];
				valid_ranges[i][0] = valid_ranges[i + 1][0];
				valid_ranges[i][1] = valid_ranges[i + 1][1];
				valid_ranges[i + 1][0] = range_tmp[0];
				valid_ranges[i + 1][1] = range_tmp[1];
			}
			i++;
		}
		j++;
	}
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

void	free_valid_ranges(t_vector *ptr_vec)
{
	free(ptr_vec->data);
	return ;
}
