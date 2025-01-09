#include "../cub3d.h"

void draw_square(t_data *data, int x, int y, int color)
{
	for (int i = 0; i < TILE_SIZE; i++)
		mlx_pixel_put(data->mlx, data->win, x * TILE_SIZE, y * TILE_SIZE + i, color);
	for (int j = 0; j < TILE_SIZE; j++)
			mlx_pixel_put(data->mlx, data->win, x * TILE_SIZE + j, y * TILE_SIZE, color);
}

void render_map(t_data *data) {
	for (int y = 0; data->map[y] != NULL; y++) {
		int row_length = strlen(data->map[y]);
		for (int x = 0; x < row_length; x++) {
			if (data->map[y][x] == '1')
				draw_square(data, x, y, 0xFF0000);
			else if (data->map[y][x] == '0')
				draw_square(data, x, y, 0x00FF00);
			else if (data->map[y][x] == 'P')
				draw_square(data, x, y, 0x0000FF);
		}
	}
}

void calculate_map_dimensions(t_data *data) {
	data->map_width = 0;
	data->map_height = 0;
	int row_length;

	row_length = 0;
	for	(int y = 0; data->map[y] != NULL; y++) {
		row_length = strlen(data->map[y]);
		if (row_length > data->map_width)
			data->map_width = row_length;
		data->map_height++;
	}
	data->map_width--;
}

t_data *ft_init_data(char **map)
{
	t_data *data = malloc(sizeof(t_data));
	data->map = map;
	printf("\nmap[0][0] = %c\n", map[0][0]);
	calculate_map_dimensions(data);
	data->mlx = mlx_init();
	data->win = mlx_new_window(data->mlx, data->map_width * TILE_SIZE, data->map_height * TILE_SIZE, "Cub3D Map");
	render_map(data);
	mlx_loop(data->mlx);
	return (data);
}
