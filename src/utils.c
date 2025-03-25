#include "../cub3d.h"

void	find_player(t_data *data)
{
	int	i;
	int	j;

	i = 0;
	j = 0;
	while (data->map[i] != NULL)
	{
		j = 0;
		while (data->map[i][j] != '\0')
		{
			if (data->map[i][j] == 'N' || data->map[i][j] == 'S' ||
				data->map[i][j] == 'E' || data->map[i][j] == 'W')
			{
				data->x_player = j;
				data->float_x = i + 0.5;
				data->float_y = j + 0.5;
				data->y_player = i;
				return ;
			}
			j++;
		}
		i++;
	}
	free_matrix((void **)data->map);
	free(data);
	ft_error("No player");
}

void	calculate_map_dimensions(t_data *data)
{
	int	row_length;
	int	y;

	data->map_width = 0;
	data->map_height = 0;
	row_length = 0;
	y = 0;
	while (data->map[y] != NULL)
	{
		row_length = ft_strlen_lib(data->map[y]);
		if (row_length > data->map_width)
			data->map_width = row_length;
		data->map_height++;
		y++;
	}
	data->map_width--;
	find_player(data);
}

int	ft_close(t_data *data)
{
	printf("ESC pressed. Exiting...\n");
	mlx_destroy_image(data->mlx, data->img_buffer);
	mlx_destroy_image(data->mlx, data->texture_north.img);
	mlx_destroy_image(data->mlx, data->texture_south.img);
	mlx_destroy_image(data->mlx, data->texture_east.img);
	mlx_destroy_image(data->mlx, data->texture_west.img);
	mlx_destroy_image(data->mlx, data->texture_sky.img);
	mlx_destroy_image(data->mlx, data->texture_grass.img);
	mlx_destroy_window(data->mlx, data->win);
	mlx_destroy_display(data->mlx);
	free(data->texture_north.data);
	free(data->texture_south.data);
	free(data->texture_east.data);
	free(data->texture_west.data);
	free(data->texture_sky.data);
	free(data->texture_grass.data);
	free(data->mlx);
	free_matrix((void **)data->map);
	free(data);
	exit(0);
}

int	handle_keypress(int keycode, t_data *data)
{
	if (keycode == 65307)
		ft_close(data);
	if (keycode == 119)
		move_player(data, 'w');
	else if (keycode == 115)
		move_player(data, 's');
	else if (keycode == 97)
		move_player(data, 'a');
	else if (keycode == 100)
		move_player(data, 'd');
	else if (keycode == 65361)
		move_player(data, 'l');
	else if (keycode == 65363)
		move_player(data, 'r');
	return (0);
}

void	draw_square(t_data *data, int x, int y, int color)
{
	int	i;
	int	j;

	i = 0;
	while (i < TILE_SIZE)
	{
		j = 0;
		while (j < TILE_SIZE)
		{
			mlx_pixel_put(data->mlx, data->win, x * TILE_SIZE
				+ j, y * TILE_SIZE + i, color);
			j++;
		}
		i++;
	}
}
