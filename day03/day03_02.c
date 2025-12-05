#include <stdio.h>
#include <fcntl.h>
#include <stdlib.h>
#include <string.h>
#include "ft_io.h"
#include "ft_vectors.h"

long long	get_max_joltage(char *line);
void		free_char_star(t_vector *ptr_lines);
int			get_file_content(t_vector *ptr_lines, char *filename);

int	main(int argc, char **argv)
{
	int			ret;
	t_vector	lines;
	long long	res = 0;
	size_t		i;
	long long	add;

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
		printf("lines[%zu] = %s", i, ((char **)lines.data)[i]);
		add = get_max_joltage(((char **)lines.data)[i]);
		printf("add = %lld\n", add);
		printf("\n");
		res += add;
		i++;
	}
	printf("res = %lld\n", res);
	i = 0;
	ft_vector_free(&lines);
	return (0);
}

typedef struct s_token
{
	size_t	index;
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

long long get_max_joltage(char *line)
{
	size_t	len;
	size_t	i;
	t_token	token[12];
	size_t	joltage_i;
	char	joltage[13];

	if (line == NULL)
		return (0);
	len = strlen(line);
	joltage_i = 0;
	while (joltage_i < 12)
	{
		if (joltage_i == 0)
			token[joltage_i].index = 0;
		else
			token[joltage_i].index = token[joltage_i - 1].index + 1;
		token[joltage_i].content = '0' - 1;
		i = token[joltage_i].index;
		while (i < len - 12 + joltage_i)
		{
			if (line[i] > token[joltage_i].content)
			{
				token[joltage_i].index = i;
				token[joltage_i].content = line[i];
			}
			i++;
		}
		joltage[joltage_i] = token[joltage_i].content;
		joltage_i++;
	}
	return (atoll(joltage));
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
