#ifndef GAME_H
# define GAME_H

# include <ncurses.h>
# include <stdlib.h>
# include <time.h>
# include <pthread.h>
# include <unistd.h>
# include <string.h>

# define true 		1
# define false 		0
# define EMPTY		' '
# define ASTEROID	'*'
# define STAR		'.'
# define PLAYER		'o'
# define GAME_OVER	"GAME OVER"
# define EXIT 		"Press any key to exit..."
# define LVL 		"You have reached level %d"
# define ENERGY 	" - E N E R G Y -       //"
# define NO_COLOR	"ERROR: Terminal does not support color.\n"

typedef struct     	s_objects
{
    int                 x;
    int                 y;
    struct s_objects   *next;
}                  	t_objects;

void    			push_back(t_objects **objects, int y_cord, int x_cord);
void				erase_element(t_objects **stars);
void				clear_list(t_objects **objects);

#endif