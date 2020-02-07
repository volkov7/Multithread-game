#include "game.h"

WINDOW* 	game_wnd;
WINDOW* 	info_wnd;
int			game_over = false;
int			exit_request = false;
int			player_y;
int			player_x;
int			energy = 1000;
int			level = 1;
int 		info_height = 4;

pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

/* variables for Dekker algorithm */
int 		favouredthread = 1;
int 		thread1wantstoenter = false; 
int 		thread2wantstoenter = false;

int init()
{
    initscr();
    cbreak();
    noecho();
    curs_set(0);
	start_color();
	game_wnd = newwin(LINES - info_height, COLS, 0, 0);
	info_wnd = newwin(info_height, COLS, LINES - info_height, 0);
	keypad(game_wnd, TRUE);
	if(!has_colors())
	{
		endwin();
		printf(NO_COLOR);
		exit(1);
	}
	init_pair(0, COLOR_WHITE, COLOR_BLACK);
    wbkgd(info_wnd, COLOR_PAIR(0));
    wbkgd(game_wnd, COLOR_PAIR(0));
    init_pair(1, COLOR_RED, COLOR_BLACK);
	init_pair(2, COLOR_YELLOW, COLOR_BLACK);
    init_pair(3, COLOR_GREEN, COLOR_BLACK);
    init_pair(4, COLOR_MAGENTA, COLOR_BLACK);
	return 0;
}

void	*set_level(void *data)
{
	int		tick;

	tick = 0;
	while (energy != 0 && exit_request == false)
	{
		usleep(1000000);
		tick++;
		if (tick == 10 && level < 11)
		{
			tick = 0;
			level++;
		}
	}
	return (NULL);
}

void	set_pair(int *pair)
{
	int		rem;

	rem = energy / 5;
	if (rem >= 150)
		*pair = 4;
	else if (rem >= 100)
		*pair = 3;
	else if (rem >= 50)
		*pair = 2;
	else if (rem > 0)
		*pair = 1;
	else
		*pair = 0;
}

/* USER INTERFACE */
void	draw_info(void)
{
	int		i;
	int		pair;

	wmove(info_wnd, 0, 0);
	whline(info_wnd, '-', COLS);
	wmove(info_wnd, 1, 0);
	whline(info_wnd, ' ', 20);
	set_pair(&pair);
	for (i = 1; i <= pair; i++)
	{
		wattron(info_wnd, COLOR_PAIR(i));
        wattron(info_wnd, A_BOLD);
        waddstr(info_wnd, "/////");
        wattroff(info_wnd, A_BOLD);
        wattroff(info_wnd, COLOR_PAIR(i));
	}
	mvwprintw(info_wnd, 2, 0, ENERGY);
	wattron(info_wnd, A_BOLD);
	wattron(info_wnd, COLOR_PAIR(pair));
	mvwprintw(info_wnd, 2, 18, "%d%%", energy);
	wattroff(info_wnd, COLOR_PAIR(pair));
	wattroff(info_wnd, A_BOLD);
	mvwprintw(info_wnd, 3, 0, "LEVEL: %d", level);
	wrefresh(info_wnd);
}

void	*stars_update(void *data)
{
	t_objects	*tmp;
	t_objects	*stars = NULL;

	while (energy != 0 && exit_request == false)
	{
		tmp = stars;

		/* DEKKER ALGORITHM */
		thread1wantstoenter = true;
		while (thread2wantstoenter == true)
		{
			if (favouredthread == 2)
			{
				usleep(10000 - (level * 1000)); // 10 ms
				thread1wantstoenter = false;
				while (favouredthread == 2) ;
				thread1wantstoenter = true;
			}
		}
		/* DEKKER ALGORITHM */
		/* CRITICAL SECTION */
		pthread_mutex_lock(&mutex);
		while (tmp)
		{
			tmp->y++;
			if (!(((tmp->y - 1) == player_y && tmp->x == player_x)
				||((tmp->y - 1) == player_y && tmp->x == player_x + 1)
				||((tmp->y - 1) == player_y && tmp->x == player_x - 1)
				||((tmp->y - 1) == player_y + 1 && tmp->x == player_x)))
				mvwaddch(game_wnd, tmp->y - 1, tmp->x, EMPTY);
			if (!((tmp->y == player_y && tmp->x == player_x)
				||(tmp->y == player_y && tmp->x == player_x + 1)
				||(tmp->y == player_y && tmp->x == player_x - 1)
				||(tmp->y == player_y + 1 && tmp->x == player_x)))
				mvwaddch(game_wnd, tmp->y, tmp->x, STAR);
			wrefresh(game_wnd);
			if (tmp->y == LINES - info_height)
			{
				erase_element(&stars);
				tmp = stars;
			}
			else
				tmp = tmp->next;
		}
		draw_info();
		pthread_mutex_unlock(&mutex);
		/* CRITICAL SECTION */
		favouredthread = 2;
		thread1wantstoenter = false;
		push_back(&stars, 0, rand() % COLS);
	}
	clear_list(&stars);
	return (NULL);
}

void    *asteroids_update(void *data)
{
    t_objects	*tmp;
	t_objects	*asteroids;
	int			tick = 1;

	asteroids = NULL;
	while (energy != 0 && exit_request == false)
	{
		tick++;
		tmp = asteroids;
		/* DEKKER ALGORITHM */
		thread2wantstoenter = true;
		while (thread1wantstoenter == true)
		{
			if (favouredthread == 1)
			{
				usleep(10000 - (level * 1000)); // 10 ms
				thread2wantstoenter = false;
				while (favouredthread == 1) ;
				thread2wantstoenter = true;
			}
		}
		/* DEKKER ALGORITHM */
		/* BEGIN CRITICAL SECTION */
		pthread_mutex_lock(&mutex);
		while (tmp)
		{
			if (tick % 7 == 0)
				tmp->y++;
			mvwaddch(game_wnd, tmp->y - 1, tmp->x, EMPTY);
			wattron(game_wnd, A_BOLD);
			mvwaddch(game_wnd, tmp->y, tmp->x, ASTEROID);
			wattroff(game_wnd, A_BOLD);
			wrefresh(game_wnd);
			if ((tmp->y == player_y && tmp->x == player_x)
				||(tmp->y == player_y && tmp->x == player_x + 1)
				||(tmp->y == player_y && tmp->x == player_x - 1))
				energy -= 25;
			if (tmp->y == LINES - info_height)
			{
				erase_element(&asteroids);
				tmp = asteroids;
			}
			else
				tmp = tmp->next;
		}
		pthread_mutex_unlock(&mutex);
		/* END CRITICAL SECTION */
		favouredthread = 1;
		thread2wantstoenter = false;
		if (tick % 7 == 0)
		{
			tick = 1;
			push_back(&asteroids, 0, rand() % COLS);
		}
	}
	clear_list(&asteroids);
	return (NULL);
}

void	player_clr(int y, int x)
{
	mvwaddch(game_wnd, y, x + 1, EMPTY);
	mvwaddch(game_wnd, y, x - 1, EMPTY);
	mvwaddch(game_wnd, y, x, EMPTY);
	mvwaddch(game_wnd, y + 1, x, EMPTY);
}

void	direction(int *y, int *x, int ch)
{
	if (ch == KEY_UP)
	{
		player_clr(*y, *x);
		(*y)--;
		if (*y < 0)
			(*y)++;
	}
	else if (ch == KEY_RIGHT)
	{
		player_clr(*y, *x);
		(*x)++;
		if (*x > COLS - 2)
			(*x)--;
	}
	else if (ch == KEY_LEFT )
	{
		player_clr(*y, *x);
		(*x)--;
		if (*x < 1)
			(*x)++;
	}
	else if (ch == KEY_DOWN)
	{
		player_clr(*y, *x);
		(*y)++;
		if (*y > LINES - info_height - 2)
			(*y)--;
	}
	else if (ch == 'q' || ch == 'Q')
		exit_request = true;
}

void	_game_over_()
{
	clear();
	attron(A_BOLD);
	mvaddstr(LINES / 2, COLS / 2 - strlen(GAME_OVER) / 2, GAME_OVER);
	mvprintw(LINES / 2 + 2, COLS / 2 - strlen(LVL) / 2, LVL, level);
	mvaddstr(LINES / 2 + 4, COLS / 2 - strlen(EXIT) / 2, EXIT);
	attroff(A_BOLD);
	refresh();
	getch();
}

int		main(void)
{
	pthread_t		pid1;
	pthread_t		pid2;
	pthread_t		pid3;
	int				ch;
	long long int	tick = 0;


	init();
	player_y = LINES - info_height - 2;
    player_x = 1;
	clear();
	srand(time(0));
	pthread_create(&pid1, NULL, stars_update, NULL);
	pthread_create(&pid2, NULL, asteroids_update, NULL);
	pthread_create(&pid3, NULL, set_level, NULL);
	while (energy != 0 && exit_request == false)
	{
		tick++;
		/* BEGIN CRITICAL SECTION */
		pthread_mutex_lock(&mutex);
		/* draw < > and engine */
		wattron(game_wnd, A_ALTCHARSET);
		mvwaddch(game_wnd, player_y, player_x - 1, ACS_LARROW);
		mvwaddch(game_wnd, player_y, player_x + 1, ACS_RARROW);
		wattron(game_wnd, COLOR_PAIR((tick % 10 / 3 % 2) ? 1 : 2));
		mvwaddch(game_wnd, player_y + 1, player_x, ACS_UARROW);
		wattroff(game_wnd, COLOR_PAIR((tick % 10 / 3 % 2) ? 1 : 2));
		wattroff(game_wnd, A_ALTCHARSET);
		/* draw player body */
		wattron(game_wnd, A_BOLD);
		mvwaddch(game_wnd, player_y, player_x, PLAYER);
		wattroff(game_wnd, A_BOLD);
		wrefresh(game_wnd);
		/* END CRITICAL SECTION */
		pthread_mutex_unlock(&mutex);
		ch = wgetch(game_wnd);
		pthread_mutex_lock(&mutex);
		direction(&player_y, &player_x, ch);
		pthread_mutex_unlock(&mutex);
	}
	pthread_join(pid1, NULL);
	pthread_join(pid2, NULL);
	pthread_join(pid3, NULL);
	if (exit_request == false)
		_game_over_();
	delwin(game_wnd);
	delwin(info_wnd);
	endwin();
	exit (0);
}