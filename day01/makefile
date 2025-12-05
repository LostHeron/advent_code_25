NAME_01 = day01_01
NAME_02 = day01_02

all: makelibft $(NAME_01) $(NAME_02)

makelibft:	
	$(MAKE) -C ../libft/

$(NAME_01): $(NAME_01).c
	cc -Wall -Wextra -Werror -o $@ day01_01.c -lft -L ../libft/ -I ../libft/includes/

$(NAME_02): $(NAME_02).c
	cc -Wall -Wextra -Werror -o $@ day01_02.c -lft -L ../libft/ -I ../libft/includes/
