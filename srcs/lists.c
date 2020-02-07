#include "game.h"

void	clear_list(t_objects **objects)
{
	t_objects	*tmp;

	while (*objects)
	{
		tmp = *objects;
		*objects = (*objects)->next;
		free(tmp);
	}
	(*objects) = NULL;
}

void	erase_element(t_objects **stars)
{
	t_objects *clear;

	clear = *stars;
	(*stars) = (*stars)->next;
	free(clear);
	clear = NULL;
}

t_objects  *lstnew(int y_cord, int x_cord)
{
    t_objects  *new;

    if (!(new = (t_objects*)malloc(sizeof(t_objects))))
        return (NULL);
    new->y = y_cord;
    new->x = x_cord;
    new->next = NULL;
    return (new);
}

void    push_back(t_objects **objects, int y_cord, int x_cord)
{
    t_objects  *tmp;

    if (*objects)
    {
        tmp = *objects;
        while (tmp->next)
            tmp = tmp->next;
        tmp->next = lstnew(y_cord, x_cord);
    }
    else
        *objects = lstnew(y_cord, x_cord);
}