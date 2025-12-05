#include <stdio.h>
#include <fcntl.h>
#include <stdlib.h>
#include <string.h>
#include "ft_io.h"
#include "ft_vectors.h"

int		get_max_joltage(char *line);
void	free_char_star(t_vector *ptr_lines);
int		get_file_content(t_vector *ptr_lines, char *filename);

int	main(int argc, char **argv)
{
	int			ret;
	t_vector	lines;
	int			res = 0;
	size_t		i;
	int			add;

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
		printf("joltage[%zu] = %i\n", i, add);
		printf("\n");
		res += add;
		i++;
	}
	printf("res = %i\n", res);
	i = 0;
	ft_vector_free(&lines);
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

int	get_max_joltage(char *line)
{
	size_t	len;
	size_t	i;
	t_token	token;
	char	joltage[3];

	if (line == NULL)
		return (0);
	joltage[2] = '\0';
	len = strlen(line);
	i = 0;
	token.addr = 0x0;
	token.content = '0' - 1;
	while (i < len - 2)
	{
		if (line[i] > token.content)
		{
			token.addr = line + i;
			token.content = line[i];
		}
		i++;
	}
	joltage[0] = token.content;
	printf("joltage[0] = %c\n", joltage[0]);
	i = 0;
	line = token.addr + 1;
	printf("line = %s\n", line);
	joltage[1] = '0' - 1;
	while (line[i] != '\0')
	{
		if (line[i] > joltage[1])
			joltage[1] = line[i];
		i++;
	}
	return (atoi(joltage));
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
