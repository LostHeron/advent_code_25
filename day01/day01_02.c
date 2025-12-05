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
	int	old_nb = 50;
	int	new_nb = 50;
	int occ_zero = 0;
	while (1)
	{
		int	add;
		line = get_next_line(fd, &err);
		if (line == NULL)
			break;
		printf("line = %s", line);
		printf("old_nb = %i\n", old_nb);
		add = atoi(line + 1);
		if (line[0] == 'R')
		{
			new_nb = old_nb + add;
			printf("%i = %i + %i\n", new_nb, old_nb, add);
		}
		else
		{
			new_nb = old_nb - add;
			printf("%i = %i - %i\n", new_nb, old_nb, add);
		}
		if (new_nb == 0)
		{
			occ_zero += 1;
			printf("---------> ON CHANGE !\n");
			old_nb = new_nb;
		}
		else if (new_nb < 0)
		{
			if (abs(new_nb / 100))
			{
				occ_zero += abs(new_nb / 100);
				printf("---------> ON CHANGE !\n");
			}
			if (old_nb != 0)
			{
				occ_zero += 1;
				printf("---------> ON CHANGE !\n");
			}
			old_nb = (100 + (new_nb % 100)) % 100;
		}
		else if (new_nb >= 100)
		{
			occ_zero += new_nb / 100;
			old_nb = new_nb % 100;
			printf("---------> ON CHANGE !\n");
		}
		else
			old_nb = new_nb;
		//printf("add = %i\n", add);
		//printf("new_nb = %i\n", new_nb);
		//printf("new old_nb = %i\n", old_nb);
		printf("occ_zero = %i\n\n\n\n", occ_zero);
		printf("\n\n\n");
		free(line);
	}
	close(fd);
	printf("%i\n", occ_zero);
	return (0);
}
