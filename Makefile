NAME = game

SRCS_DIR = ./srcs/

FLAGS = -Wall -Wextra -Werror

NCURSES = -lncurses

SRCS = game \
		lists

HEADER = -I./include
			
OBJ = $(addprefix obj/, $(addsuffix .o, $(SRCS)))

all: $(NAME)

obj/%.o: srcs/%.c
	@gcc -c $< $(HEADER) -o $@
	
$(NAME): obj $(OBJ)
	@tput setaf 7; tput bold; echo "Compiling game"
	@gcc $(OBJ) $(HEADER) $(NCURSES) -o $(NAME)
	@tput setaf 2; tput bold; echo "DONE Compiling"

obj:
	@mkdir obj

clean:
	@rm -rf obj
	@tput setaf 6; tput bold; echo "DONE Clean!"
	
fclean: clean
	@rm -f $(NAME)
	@tput setaf 6; tput bold; echo "DONE Fclean!"

re: fclean all
.PHONY: all clean fclean re