#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include "ft_io.h"
int	main(void)
{
	int fd = open("input", O_RDONLY);
	char *line;
	line = (char *)0x01;
	int	err;
	int	nb = 50;
	int occ_zero = 0;
	while (1)
	{
		line = get_next_line(fd, &err);
		if (line == NULL)
			break;
		if (line[0] == 'R')
			nb += atoi(line + 1);
		else
			nb -= atoi(line + 1);
		nb = nb % 100;
		if (nb == 0)
			occ_zero += 1;
		free(line);
	}
	close(fd);
	printf("%i\n", occ_zero);
	return (0);
}
