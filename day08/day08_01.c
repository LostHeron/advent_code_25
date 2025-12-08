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
	double x;
	double y;
	double z;
}	t_point;

typedef struct s_connection
{
	double	distance;
	size_t	x;
	size_t	y;
}			t_connection;

typedef struct s_node
{
	size_t			node_id;
	struct s_node	*next;
}			t_node;


void	free_char_star(t_vector *ptr_lines);
void	free_copy(t_vector *ptr_lines);
int		get_file_content(t_vector *ptr_lines, char *filename);
size_t	get_connection(t_vector lines);
void	sort_connection_array(t_connection *connection_array, size_t nb_connections);
void	print_connection(t_connection *connection_array, size_t nb_connections, t_point *point_array);
int		check_not_already_in_list(t_vector chained_connection, size_t value, size_t *ptr_index_lst_in);

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
	res = get_connection(lines);
	printf("res = %zu\n", res);
	ft_vector_free(&lines);
	return (0);
}

void	group_connections(t_connection *connection_array, size_t nb_connection);

size_t	get_connection(t_vector lines)
{
	size_t			res;
	t_vector		points;
	size_t			i;
	char			**tmp_nb;
	t_point			point_tmp;
	t_connection	*connection_array;

	res = 0;
	ft_vector_init(&points, 200, sizeof(t_point), NULL);
	i = 0;
	while (i < lines.size)
	{
		tmp_nb = ft_split(((char **)lines.data)[i], ",\n");
		//printf("tmp_nb[0] = '%s', tmp_nb[1] = '%s', tmp_nb[2] = '%s'\n", tmp_nb[0] ,tmp_nb[1], tmp_nb[2]);
		if (ft_atof_safe(tmp_nb[0], &point_tmp.x) != 0)
			printf("atof_safe 1 failed !\n");
		if (ft_atof_safe(tmp_nb[1], &point_tmp.y) != 0)
			printf("atof_safe 2 failed !\n");
		if (ft_atof_safe(tmp_nb[2], &point_tmp.z) != 0)
			printf("atof_safe 3 failed !\n");
		ft_vector_add_single(&points, &point_tmp);
		ft_split_free(tmp_nb);
		i++;
	}
	i = 0;
	while (i < points.size)
	{
		//printf("points : %f %f %f\n", ((t_point *)points.data)[i].x, ((t_point *)points.data)[i].y, ((t_point *)points.data)[i].z);
		i++;
	}
	size_t nb_connections = points.size * (points.size - 1) / 2;
	//printf("nb_connection = %zu\n", nb_connections);
	//printf("points.size = %zu\n", points.size);
	connection_array = malloc(sizeof(t_connection) * nb_connections);
	size_t	y_i = 0;
	while (y_i < points.size)
	{
		size_t	x_i = 0;
		while (x_i < y_i)
		{
			size_t	index = ((y_i - 1) * ((y_i - 1) + 1))/2 + x_i;
			//printf("index = %zu\n", index);
			//printf("x_i = %zu\n", x_i);
			//printf("y_i = %zu\n", y_i);
			fflush(stdout);
			connection_array[index].distance = sqrt(
					pow(((t_point *)points.data)[x_i].x - ((t_point *)points.data)[y_i].x, 2)
					+ pow(((t_point *)points.data)[x_i].y - ((t_point *)points.data)[y_i].y, 2)
					+ pow(((t_point *)points.data)[x_i].z - ((t_point *)points.data)[y_i].z, 2)
					);
			connection_array[index].x = x_i;
			connection_array[index].y = y_i;
			x_i++;
		}
		y_i++;
	}
	printf("nb_connections = %zu\n", nb_connections);
	printf("before sort connection\n");
	sort_connection_array(connection_array, nb_connections);
	printf("after sort connection\n");
	print_connection(connection_array, nb_connections, points.data);
	group_connections(connection_array, nb_connections);
	free(points.data);
	return (res);
}

void	print_chained_connection(t_vector chained_connection);
void	mix_chained_lst(t_vector *ptr_chained_connection, size_t index1, size_t index2);
void	sort_array_nb_connection(size_t	*array_nb_connection, size_t nb_connections);

// deux structures
// une structure qui contient un array de list chaine
// des liste chaine qui contiennent des neoud :
void	group_connections(t_connection *connection_array, size_t nb_connection)
{
	t_vector	chained_connection;
	size_t		group_i;
	size_t		connection_i;
	size_t		has_been_placed;

	(void) nb_connection;
	ft_vector_init(&chained_connection, 200, sizeof(t_node *), NULL); 
	group_i = 0;
	while (group_i < chained_connection.capacity)
	{
		((t_node **)chained_connection.data)[group_i] = NULL;
		group_i++;
	}
	connection_i = 0;
	while (connection_i < MAX_CONNECTIONS)
	{
		printf("connection %zu\n", connection_i);
		group_i = 0;
		//printf("try to place connection : %zu, %zu\n", connection_array[connection_i].x + 1, connection_array[connection_i].y + 1);
		has_been_placed = FALSE;
		while (group_i < chained_connection.size)
		{
			t_node *element_i;
			element_i = ((t_node **)chained_connection.data)[group_i];
			while (element_i != NULL)
			{
				if (element_i->node_id == connection_array[connection_i].x)
				{
					size_t	index_lst_in;
					if (check_not_already_in_list(chained_connection, connection_array[connection_i].y, &index_lst_in) == TRUE)
					{
						t_node *new_node = malloc(sizeof(t_node));
						new_node->node_id = connection_array[connection_i].y;
						new_node->next = element_i->next;
						element_i->next = new_node;
					}
					else if (index_lst_in != group_i)
					{
						mix_chained_lst(&chained_connection, group_i, index_lst_in);
					}
					has_been_placed = TRUE;
					break;
				}
				else if (element_i->node_id == connection_array[connection_i].y)
				{
					size_t	index_lst_in;
					if (check_not_already_in_list(chained_connection, connection_array[connection_i].x, &index_lst_in) == TRUE)
					{
						t_node *new_node = malloc(sizeof(t_node));
						new_node->node_id = connection_array[connection_i].x;
						new_node->next = element_i->next;
						element_i->next = new_node;
					}
					else if (index_lst_in != group_i)
					{
						mix_chained_lst(&chained_connection, group_i, index_lst_in);
					}
					has_been_placed = TRUE;
					break;
				}
				element_i = element_i->next;
			}
			if (has_been_placed == TRUE)
				break;
			group_i++;
		}
		if (has_been_placed == FALSE)
		{
			t_node *new_node1 = ft_malloc(sizeof (t_node));
			new_node1->node_id = connection_array[connection_i].x;
			new_node1->next = NULL;
			t_node *new_node2 = ft_malloc(sizeof (t_node));
			new_node2->node_id = connection_array[connection_i].y;
			new_node2->next = new_node1;
			ft_vector_add_single(&chained_connection, &new_node2);
		}
		connection_i++;
	}
	print_chained_connection(chained_connection);
	size_t	*array_nb_connection;
	array_nb_connection = malloc(sizeof(size_t) * chained_connection.size);
	size_t	i = 0;
	t_node *tmp;
	while (i < chained_connection.size)
	{
		array_nb_connection[i] = 0;
		tmp = ((t_node **)chained_connection.data)[i];
		while (tmp != NULL)
		{
			array_nb_connection[i]++;
			tmp = tmp->next;
		}
		i++;
	}
	sort_array_nb_connection(array_nb_connection, chained_connection.size);
}

void	sort_array_nb_connection(size_t	*array_nb_connection, size_t nb_connections)
{
	size_t	tmp;
	size_t	i;
	size_t	j;

	j = 0;
	while (j < nb_connections)
	{
		i = 0;
		while (i < nb_connections - 1)
		{
			if (array_nb_connection[i] < array_nb_connection[i + 1])
			{
				tmp = array_nb_connection[i];
				array_nb_connection[i] = array_nb_connection[i + 1];
				array_nb_connection[i + 1] = tmp;
			}
			i++;
		}
		j++;
	}
	i = 0;
	while (i < nb_connections)
	{
		printf("nb_connections[%zu] = %zu\n", i, array_nb_connection[i]);
		i++;
	}
}


void	mix_chained_lst(t_vector *ptr_chained_connection, size_t index1, size_t index2)
{
	t_node	*last_index1;
	t_node	*tmp_node;
	size_t	i;

	last_index1 = ((t_node **)ptr_chained_connection->data)[index1];
	while (last_index1->next != NULL)
		last_index1 = last_index1->next;
	last_index1->next = ((t_node **)ptr_chained_connection->data)[index2];
	i = index2;
	while (i < ptr_chained_connection->size - 1)
	{
		((t_node **)ptr_chained_connection->data)[i] = NULL;
		tmp_node = ((t_node **)ptr_chained_connection->data)[i];
		((t_node **)ptr_chained_connection->data)[i] = ((t_node **)ptr_chained_connection->data)[i + 1];
		((t_node **)ptr_chained_connection->data)[i + 1] = tmp_node;
		i++;
	}
	ptr_chained_connection->size -= 1;
	return ;
}

void	print_chained_connection(t_vector chained_connection)
{
	size_t	i;
	t_node	*tmp;

	i = 0;
	while (i < chained_connection.size)
	{
		tmp = ((t_node **)chained_connection.data)[i];
		printf("connection[%zu] : ", i);
		while (tmp != NULL)
		{
			printf("%zu, ", tmp->node_id + 1);
			tmp = tmp->next;
		}
		printf("\n");
		i++;
	}
}

int	check_not_already_in_list(t_vector chained_connection, size_t value, size_t *ptr_index_lst_in)
{
	size_t	connection_i;
	t_node	*tmp_lst;

	connection_i = 0;
	while (connection_i < chained_connection.size)
	{
		tmp_lst = ((t_node **)chained_connection.data)[connection_i];
		while (tmp_lst != NULL)
		{
			if (tmp_lst->node_id == value)
			{
				*ptr_index_lst_in = connection_i;
				return (FALSE);
			}
			tmp_lst = tmp_lst->next;
		}
		connection_i++;
	}
	return (TRUE);
}

void	sort_connection_array(t_connection *connection_array, size_t nb_connections)
{
	size_t			i;
	size_t			j;
	size_t			min_connection_index;
	t_connection	tmp_connection;

	i = 0;
	while (i < nb_connections && i < MAX_CONNECTIONS + 1)
	{
		if (i % 1000 == 0)
			printf("progression : %zu / %zu\n", i, nb_connections);
		min_connection_index = i;
		j = i + 1;
		while (j < nb_connections)
		{
			if (connection_array[j].distance < connection_array[min_connection_index].distance)
			{
				min_connection_index = j;
			}
			j++;
		}
		tmp_connection = connection_array[min_connection_index];
		connection_array[min_connection_index] = connection_array[i];
		connection_array[i] = tmp_connection;
		i++;
	}
	return ;
}

void	print_connection(t_connection *connection_array, size_t nb_connections, t_point *point_array)
{
	size_t	i;

	i = 0;
	while (i < nb_connections)
	{
		printf("connection %zu : x = %zu (%.0f %.0f %.0f), y = %zu (%.0f %.0f %.0f), distance = %f\n", i, 
			connection_array[i].x + 1, point_array[connection_array[i].x].x, point_array[connection_array[i].x].y, point_array[connection_array[i].x].z,
		 	connection_array[i].y + 1, point_array[connection_array[i].y].x, point_array[connection_array[i].y].y, point_array[connection_array[i].y].z,
		 	connection_array[i].distance);
		i++;
	}
	return ;
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
