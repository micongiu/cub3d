#include "../cub3d.h"

void	rotate_view(float *dx, float *dy, float angle)
{
	float	new_dx;
	float	new_dy;

	new_dx = (*dx) * cos(angle) - (*dy) * sin(angle);
	new_dy = (*dx) * sin(angle) + (*dy) * cos(angle);
	*dx = new_dx;
	*dy = new_dy;
}

void	move_player_2(t_data *data, char direc, float new_x, float new_y)
{
	if (direc == 'd')
	{
		new_x -= data->dy * 0.1;
		new_y += data->dx * 0.1;
	}
	if (direc == 'l')
		rotate_view(&data->dx, &data->dy, -0.1);
	if (direc == 'r')
		rotate_view(&data->dx, &data->dy, 0.1);
	if (new_x > 0 && new_y > 0 && new_y < data->map_height
		&& new_x < (data->map_width - 1))
	{
		if (data->map[(int)new_y][(int)new_x] != '1')
		{
			data->float_x = new_x;
			data->float_y = new_y;
			data->map[data->y_player][data->x_player] = '0';
			data->x_player = (int)data->float_x;
			data->y_player = (int)data->float_y;
			data->map[data->y_player][data->x_player] = 'P';
			mlx_clear_window(data->mlx, data->win);
			render_3d(data);
		}
	}
}

void	move_player(t_data *data, char direc)
{
	float	new_x;
	float	new_y;

	new_x = data->float_x;
	new_y = data->float_y;
	if (direc == 'w')
	{
		new_x += data->dx * 0.1;
		new_y += data->dy * 0.1;
	}
	if (direc == 's')
	{
		new_x -= data->dx * 0.1;
		new_y -= data->dy * 0.1;
	}
	if (direc == 'a')
	{
		new_x += data->dy * 0.1;
		new_y -= data->dx * 0.1;
	}
	move_player_2(data, direc, new_x, new_y);
}
