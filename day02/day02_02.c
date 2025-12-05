/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   day_02_01.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jweber <jweber@student.42Lyon.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/02 13:31:14 by jweber            #+#    #+#             */
/*   Updated: 2025/12/02 13:50:01 by jweber           ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include <fcntl.h>
#include <stdlib.h>
#include <unistd.h>
#include "ft_io.h"
#include "ft_standard.h"
#include "string.h"
#include <stdio.h>

#define TRUE 1
#define FALSE 0

int	is_repeated_sequence(char *nb);


static int	ft_get_size(long long n);
static char	*ft_lltoa_pos(long long n);
static char	*ft_lltoa_neg(long long n);

char	*ft_lltoa(long long n)
{
	if (n < 0)
		return (ft_lltoa_neg(n));
	else
		return (ft_lltoa_pos(n));
}

static char	*ft_lltoa_pos(long long n)
{
	int			size;
	long long	n_c;
	int			i;
	char		*res;

	size = ft_get_size(n);
	res = ft_malloc((unsigned long)(size + 1) * sizeof(char));
	if (res == NULL)
		return (NULL);
	i = 0;
	n_c = n;
	while (i < size)
	{
		res[size - 1 - i] = ((char)(n_c % 10)) + '0';
		n_c = n_c / 10;
		i++;
	}
	res[i] = '\0';
	return (res);
}

static char	*ft_lltoa_neg(long long n)
{
	int			size;
	long long	n_c;
	int			i;
	char		*res;

	size = ft_get_size(n) + 1;
	res = ft_malloc((unsigned long)(size + 1) * sizeof(char));
	if (res == NULL)
		return (NULL);
	res[0] = '-';
	i = 0;
	n_c = n;
	while (i < size - 1)
	{
		res[size - 1 - i] = (((char)(-(n_c % 10))) + '0');
		n_c = n_c / 10;
		i++;
	}
	res[size] = '\0';
	return (res);
}

static int	ft_get_size(long long n)
{
	long	n_c;
	int		i;

	n_c = n;
	i = 0;
	while (!(-9 <= n_c && n_c <= 9))
	{
		n_c = n_c / 10;
		i++;
	}
	i++;
	return (i);
}


int	main(int argc, char **argv)
{
	int			fd;
	int			err;
	char		*line;
	char		**data;
	long long	res;
	size_t		i;

	if (argc <= 1)
	{
		printf("no input files\n");
		return (1);
	}
	fd = open(argv[1], O_RDONLY);
	if (fd < 0)
	{
		printf("could not open file : %s\n", argv[1]);
		return (1);
	}
	line = get_next_line(fd, &err);
	close(fd);
	if (line == NULL)
		return (1);
	data = ft_split(line, ",");
	free(line);
	if (data == NULL)
		return (1);
	res = 0;
	i = 0;
	while (data[i] != NULL)
	{
		printf("%s\n", data[i]);
		char	**ids;
		ids = ft_split(data[i], "-");
		if (ids == NULL)
		{
			ft_split_free(data);
			return (1);
		}
		long long	begin = atoll(ids[0]);
		long long	end = atoll(ids[1]);
		ft_split_free(ids);
		while (begin <= end)
		{
			char *nb = ft_lltoa(begin);
			if (is_repeated_sequence(nb) == TRUE)
			{
				printf("ADDING : %lld\n", begin);
				if (res + begin < res)
					printf("ERRROR!\n");
				res += begin;
				printf("res = %lld\n", res);
			}
			/*
			else
				printf("not adding : %lld\n", begin);
			*/
			free(nb);
			begin++;
		}
		printf("\n");
		i++;
	}
	ft_split_free(data);
	printf("res = %lld\n", res);
	return (0);
}

int	check_sub_sequence(char *nb, size_t len, size_t len_i);

int	is_repeated_sequence(char *nb)
{
	size_t	len;
	size_t	half_len;
	size_t	len_i;

	//"25252525"
	//"22222222"
	len = strlen(nb);
	half_len = len / 2;
	len_i = 1;
	while (len_i <= half_len)
	{
		if (check_sub_sequence(nb, len, len_i) == TRUE)
			return (TRUE);
		len_i++;
	}
	return (FALSE);
}

int	check_sub_sequence(char *nb, size_t len, size_t len_i)
{
	size_t	step;

	step = 1;
	while (step * len_i < len)
	{
		if (strncmp(nb, nb + step * len_i, len_i) != 0)
			return (FALSE);
		step++;
	}
	return (TRUE);
}
