#include "../cub3d.h"


void find_player(t_data *data)
{
	int i = 0;
	int j = 0;
	while (data->map[i] != NULL)
	{
		j = 0;
		while (data->map[i][j] != '\0')
		{
			if (data->map[i][j] == 'P')
			{
				data->x_player = j;
				data->float_x = i + 0.5;
				data->float_y = j + 0.5;
				data->y_player = i;
				return;
			}
			j++;
		}
		i++;
	}
	printf("no player\n");
	free_matrix((void **)data->map);
	free(data);
	exit(0);
}
void calculate_map_dimensions(t_data *data)
{
	int row_length;
	data->map_width = 0;
	data->map_height = 0;

	row_length = 0;
	for (int y = 0; data->map[y] != NULL; y++)
	{
		row_length = ft_strlen_lib(data->map[y]);
		if (row_length > data->map_width)
			data->map_width = row_length;
		data->map_height++;
	}
	data->map_width--;
	// data->map_height--;
	find_player(data);
	printf("x_p = %i\n", data->x_player);
	printf("y_p = %i\n", data->y_player);
}

int ft_close(t_data *data)
{
	printf("ESC pressed. Exiting...\n");
	mlx_destroy_window(data->mlx, data->win);
	mlx_destroy_display(data->mlx);
	free(data->mlx);
	free_matrix((void **)data->map);
	free(data);
	exit(0);
}
int handle_keypress(int keycode, t_data *data)
{
	if (keycode == 65307)
		ft_close(data);
	if (keycode == 119)
		move_p(data, 'w');
	else if (keycode == 115)
		move_p(data, 's');
	else if (keycode == 97)
		move_p(data, 'a');
	else if (keycode == 100)
		move_p(data, 'd');
	else if (keycode == 65361)
		move_p(data, 'l');
	else if (keycode == 65363)
		move_p(data, 'r');
	return (0);
}

void draw_square(t_data *data, int x, int y, int color)
{
	for (int i = 0; i < TILE_SIZE; i++)
	{
		for (int j = 0; j < TILE_SIZE; j++)
			mlx_pixel_put(data->mlx, data->win, x * TILE_SIZE + j, y * TILE_SIZE + i, color);
	}
}
