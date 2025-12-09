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

enum e_direction
{
	NONE = 0,
	UP = 1,
	DOWN = -UP,
	RIGHT = 2,
	LEFT = -RIGHT,
};

typedef struct s_segment
{
	t_point	start;
	t_point	end;
	int		direction;
}	t_segment;

/* rectangle of form :
 * A------B
 * |      |
 * C------D
*/
typedef struct s_rectangle
{
	t_point a;
	t_point b;
	t_point c;
	t_point d;
	size_t	area;
	size_t	length;
	size_t	width;
}	t_rectangle;

void	free_char_star(t_vector *ptr_lines);
void	free_copy(t_vector *ptr_lines);
int		get_file_content(t_vector *ptr_lines, char *filename);
void	algo(t_vector points);

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
	algo(points);
	printf("res = %zu\n", res);
	free(points.data);
	ft_vector_free(&lines);
	return (0);
}

ssize_t	get_side(t_vector points);
void	get_rectangles(t_vector points, t_vector *ptr_rectangles);
void	get_segments(t_vector points, t_vector *ptr_segments);
void	sort_rectangles(t_vector rectangles, size_t	*ptr_nb_sorted, size_t nb_to_sort);
void	print_rectangles(t_vector rectangles, size_t nb_to_print);
void	print_single_rectangle(t_rectangle rectangle);
int		check_rectangle_valid(t_rectangle rectangle, t_vector segments);

void	algo(t_vector points)
{
	ssize_t		side;
	t_vector	rectangles;
	t_vector	segments;
	size_t		nb_sorted;

	get_segments(points, &segments);
	get_rectangles(points, &rectangles);
	nb_sorted = 0;
	while (nb_sorted < 10)
	{
		sort_rectangles(rectangles, &nb_sorted, 1);
		print_single_rectangle(((t_rectangle *)rectangles.data)[nb_sorted - 1]);
		if (check_rectangle_valid(((t_rectangle *)rectangles.data)[nb_sorted - 1], segments) == TRUE)
		{
			printf("rectangle is VALID !\n");
		}
		else
			printf("rectangle is NOT valid\n");
	}
	/*
	*/
	printf("nb_rectangles = %zu\n", rectangles.size);
	printf("nb_points = %zu\n", points.size);
	printf("(nb_points)*(nb_points - 1)/2 = %zu\n", (points.size)*(points.size - 1)/2);
	side = get_side(points);
	if (side > 0)
		printf("side = %zd (map on left)\n", side);
	else if (side < 0)
		printf("side = %zd (map on right)\n", side);
	rectangles.del(&rectangles);
	rectangles.del(&segments);
}

void	fill_segment_from_points(t_segment *ptr_segment, t_point p1, t_point p2);
int		check_intersection(t_segment s1, t_segment s2);

int	check_rectangle_valid(t_rectangle rectangle, t_vector segments)
{
	size_t		i;
	t_segment	ab;
	t_segment	cd;
	t_segment	ac;
	t_segment	bd;

	fill_segment_from_points(&ab, rectangle.a, rectangle.b);
	fill_segment_from_points(&cd, rectangle.c, rectangle.d);
	fill_segment_from_points(&ac, rectangle.a, rectangle.c);
	fill_segment_from_points(&bd, rectangle.b, rectangle.d);
	i = 0;
	while (i < segments.size)
	{
		if (check_intersection(ab, ((t_segment *)segments.data)[i]) == TRUE)
			return (FALSE);
		if (check_intersection(cd, ((t_segment *)segments.data)[i]) == TRUE)
			return (FALSE);
		if (check_intersection(ac, ((t_segment *)segments.data)[i]) == TRUE)
			return (FALSE);
		if (check_intersection(bd, ((t_segment *)segments.data)[i]) == TRUE)
			return (FALSE);
		i++;
	}
	// now must check if all points on the rectangle border are on a segment
	// aaaah but no, not good, they can be not on any segment and still be a valid
	// point
	// of maybe, do some bsp stuff for each point in the rectangle ?
	// yeah but not possible either, approx 10e9 points per square, so way too
	// long
	// raaaah if do not get it 
	// hm maybe check 4 coins of rectangle are within the maps 
	// using BSP stuff, but anyway, time to sleep !
	return (TRUE);
}

ssize_t	min(ssize_t	a, ssize_t b)
{
	if (a < b)
		return (a);
	else
		return (b);
}

ssize_t	max(ssize_t	a, ssize_t b)
{
	if (a > b)
		return (a);
	else
		return (b);
}

int	check_intersection(t_segment s1, t_segment s2)
{
	if (s1.direction == s2.direction || s1.direction == -s2.direction)
		return (FALSE);
	if (s1.direction == LEFT || s1.direction == RIGHT)// y const for s1
	{
		if ((min(s2.end.y, s2.start.y) < s1.start.y) && (s1.start.y < max(s2.end.y, s2.start.y)))
		{
			if ((min(s1.start.x, s1.end.x) < s2.end.x) && (s2.end.x < max(s1.start.x, s1.end.x)))
			{
				return (TRUE);
			}
		}
	}
	else if (s1.direction == DOWN || s1.direction == UP) // x const for s1
	{
		if ((min(s2.end.x, s2.start.x) < s1.start.x) && (s1.start.x < max(s2.end.x, s2.start.x)))
		{
			if ((min(s1.start.y, s1.end.y) < s2.end.y) && (s2.end.y < max(s1.start.y, s1.end.y)))
			{
				return (TRUE);
			}
		}
	}
	return (FALSE);
}

void	fill_segment_from_points(t_segment *ptr_segment, t_point p1, t_point p2)
{
	ptr_segment->start = p1;
	ptr_segment->end = p2;
	if (ptr_segment->start.x == ptr_segment->end.x)
	{
		if (ptr_segment->end.y - ptr_segment->start.y > 0)
			ptr_segment->direction = UP;
		else if (ptr_segment->end.y - ptr_segment->start.y < 0)
			ptr_segment->direction = DOWN;
		else
			ptr_segment->direction = NONE;
	}
	else
	{
		if (ptr_segment->end.x - ptr_segment->start.x > 0)
			ptr_segment->direction = RIGHT;
		else if (ptr_segment->end.x - ptr_segment->start.x < 0)
			ptr_segment->direction = LEFT;
		else
			ptr_segment->direction = NONE;
	}
}

void	free_rectangles(t_vector *ptr_vec);

void	get_segments(t_vector points, t_vector *ptr_segments)
{
	size_t		i;
	t_segment	segment_tmp;

	ft_vector_init(ptr_segments, 500, sizeof(t_segment), free_rectangles);
	i = 0;
	while (i < points.size)
	{
		segment_tmp.start = ((t_point *)points.data)[i];
		segment_tmp.end = ((t_point *)points.data)[(i + 1) % points.size];
		if (segment_tmp.start.x == segment_tmp.end.x)
		{
			if (segment_tmp.end.y - segment_tmp.start.y > 0)
				segment_tmp.direction = UP;
			else if (segment_tmp.end.y - segment_tmp.start.y < 0)
				segment_tmp.direction = DOWN;
			else
				segment_tmp.direction = NONE;
		}
		else
		{
			if (segment_tmp.end.x - segment_tmp.start.x > 0)
				segment_tmp.direction = RIGHT;
			else if (segment_tmp.end.x - segment_tmp.start.x < 0)
				segment_tmp.direction = LEFT;
			else
				segment_tmp.direction = NONE;
		}
		ft_vector_add_single(ptr_segments, &segment_tmp);
		i++;
	}
	return ;
}


void	get_rectangles(t_vector points, t_vector *ptr_rectangles)
{
	size_t		i;
	size_t		j;
	t_rectangle rectangle_tmp;

	ft_vector_init(ptr_rectangles, 200000, sizeof(t_rectangle), free_rectangles); 
	i = 0;
	while (i < points.size)
	{
		j = 0;
		while (j < i)
		{
			rectangle_tmp.a = ((t_point *)points.data)[i];
			rectangle_tmp.d = ((t_point *)points.data)[j];
			rectangle_tmp.b.x = rectangle_tmp.a.x;
			rectangle_tmp.b.y = rectangle_tmp.d.y;
			rectangle_tmp.c.x = rectangle_tmp.d.x;
			rectangle_tmp.c.y = rectangle_tmp.a.y;
			rectangle_tmp.length = llabs(((t_point *)points.data)[i].x - ((t_point *)points.data)[j].x) + 1;
			rectangle_tmp.width = llabs(((t_point *)points.data)[i].y - ((t_point *)points.data)[j].y) + 1;
			rectangle_tmp.area = rectangle_tmp.length * rectangle_tmp.width;
			ft_vector_add_single(ptr_rectangles, &rectangle_tmp);
			j++;
		}
		i++;
	}
}

void	free_rectangles(t_vector *ptr_vec)
{
	free(ptr_vec->data);
}

void	print_rectangles(t_vector rectangles, size_t nb_to_print)
{
	size_t	i;

	i = 0;
	while (i < rectangles.size && i < nb_to_print)
	{
		print_single_rectangle(((t_rectangle *)rectangles.data)[i]);
		i++;
	}
}

void print_single_rectangle(t_rectangle rectangle)
{
	printf("~~~~~~~~~~~~~~~~~~~\n");
	printf("rectangle between : \n");
	printf("a.x : %zu, a.y : %zu\n", rectangle.a.x, rectangle.a.y);
	printf("b.x : %zu, b.y : %zu\n", rectangle.b.x, rectangle.b.y);
	printf("c.x : %zu, c.y : %zu\n", rectangle.c.x, rectangle.c.y);
	printf("d.x : %zu, d.y : %zu\n", rectangle.d.x, rectangle.d.y);
	printf("area: %zu\n", rectangle.area);
	printf("length: %zu\n", rectangle.length);
	printf("with: %zu\n", rectangle.width);
	printf("~~~~~~~~~~~~~~~~~~~\n\n");
}

void	sort_rectangles(t_vector rectangles, size_t	*ptr_nb_sorted, size_t nb_to_sort)
{
	size_t		i;
	size_t		sorted_nb;
	size_t		index_area_max;
	t_rectangle	tmp_rectangle;

	sorted_nb = 0;
	while (sorted_nb < nb_to_sort)
	{
		index_area_max = *ptr_nb_sorted;
		i = *ptr_nb_sorted + 1;
		while (i < rectangles.size)
		{
			if (((t_rectangle *)rectangles.data)[i].area > ((t_rectangle *)rectangles.data)[index_area_max].area)
			{
				index_area_max = i;
			}
			i++;
		}
		tmp_rectangle = ((t_rectangle *)rectangles.data)[*ptr_nb_sorted];
		((t_rectangle *)rectangles.data)[*ptr_nb_sorted] = ((t_rectangle *)rectangles.data)[index_area_max];
		((t_rectangle *)rectangles.data)[index_area_max] = tmp_rectangle;
		*ptr_nb_sorted += 1;
		sorted_nb++;
	}
}

ssize_t	get_side(t_vector points)
{
	size_t	i;
	t_point	start;
	t_point	end;
	t_point next;
	ssize_t	side;
	ssize_t diff;

	i = 0;
	side = 0;
	while (i < points.size - 1)
	{
		start = ((t_point *)points.data)[i];
		end = ((t_point *)points.data)[(i + 1) % points.size];
		// with end and start point we can get the direction its going
		// and with next point we can get the direction
		next = ((t_point *)points.data)[(i + 2) % points.size];
		// going left increment side (left -> side++)
		// going right decrement side (right -> side--)
		if (start.x == end.x)
		{
			diff = end.y - start.y;
			if (diff > 0) // going upward
			{
				if (next.x > start.x) // next turn is on right
					side--;
				if (next.x < start.x) // next turn is on left
					side++;
			}
			else if (diff < 0) // going downard
			{
				if (next.x > start.x) // next turn is on right
					side++;
				if (next.x < start.x) // next turn is on left
					side--;
			}
		}
		else
		{
			diff = end.x - start.x;
			if (diff > 0) // going left to right
			{
				if (next.y > end.y) // next turn is on left
					side++;
				else if (next.y < end.y) // next turn is on right
					side--;
			}
			else if (diff < 0) // going right to left 
			{
				if (next.y > end.y) // next turn is on right
					side--;
				else if (next.y < end.y) // next turn is on left
					side++;
			}
		}
		i++;
	}
	return (side);
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
