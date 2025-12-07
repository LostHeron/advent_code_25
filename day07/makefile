NAME_01 = day07_01
NAME_02 = day07_02

all: makelibft $(NAME_01) $(NAME_02)

makelibft:	
	$(MAKE) -C ../libft/

$(NAME_01): $(NAME_01).c
	cc  -Wall -Wextra -Werror -g3 -o $@ $< -lft -L ../libft/ -I ../libft/includes/

$(NAME_02): $(NAME_02).c
	cc -Wall -Wextra -Werror -g3 -o $@ $< -lft -L ../libft/ -I ../libft/includes/
