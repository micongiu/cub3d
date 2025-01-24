#include "../cub3d.h"

void rotate_view(float *dx, float *dy, float angle)
{
	float new_dx = (*dx) * cos(angle) - (*dy) * sin(angle);
	float new_dy = (*dx) * sin(angle) + (*dy) * cos(angle);

	*dx = new_dx;
	*dy = new_dy;
}
void draw_line_sopra(t_data *data, int start_x, int start_y, float dx, float dy, int color)
{
	float pixel_x = start_x * TILE_SIZE + TILE_SIZE / 2;
	float pixel_y = start_y * TILE_SIZE + TILE_SIZE / 2;
	int map_x, map_y;

	while (pixel_y >= 0 && pixel_y < data->map_height * TILE_SIZE &&
		pixel_x >= 0 && pixel_x < data->map_width * TILE_SIZE)
	{
		map_x = pixel_x / TILE_SIZE;
		map_y = pixel_y / TILE_SIZE;

		if (data->map[map_y][map_x] == '1')
			break;

		mlx_pixel_put(data->mlx, data->win, pixel_x, pixel_y, color);
		pixel_x += dx;
		pixel_y += dy;
	}
}
void draw_cono(t_data *data, int start_x, int start_y, float dx, float dy, int color)
{
	float fov = 1.162;
	int ray_n = 100;
	float angle_steps = fov / ray_n; // Angolo tra ciascun raggio
	for (int i = 0; i < ray_n; i++)
	{
		float angle = -fov / 2 + i * angle_steps;
		float ray_dx = dx * cos(angle) - dy * sin(angle);
		float ray_dy = dx * sin(angle) + dy * cos(angle);
		draw_line_sopra(data, start_x, start_y, ray_dx, ray_dy, color);
	}
}

void render_map(t_data *data)
{
	for (int y = 0; data->map[y] != NULL; y++)
	{
		int row_length = ft_strlen_lib(data->map[y]);
		for (int x = 0; x < row_length; x++)
		{
			if (data->map[y][x] == '1')
				draw_square(data, x, y, 0xFF0000);
			else if (data->map[y][x] == '0')
				draw_square(data, x, y, 0x00FF00);
			else if (data->map[y][x] == 'P')
				draw_square(data, x, y, 0x0000FF);
		}
	}
}
void move_p(t_data *data, char direc)
{
	float new_x = data->float_x;
	float new_y = data->float_y;

		// Spostamento basato sull'orientamento
	if (direc == 'w') { // Avanti
		new_x += data->dx * 0.1;
		new_y += data->dy * 0.1;
	}
	if (direc == 's') { // Indietro
		new_x -= data->dx * 0.1;
		new_y -= data->dy * 0.1;
	}
	if (direc == 'a') { // Sinistra (laterale)
		new_x += data->dy * 0.1;
		new_y -= data->dx * 0.1;
	}
	if (direc == 'd') { // Destra (laterale)
		new_x -= data->dy * 0.1;
		new_y += data->dx * 0.1;
	}
	printf("new_x = %f\n", new_x);
	printf("new_y = %f\n", new_y);

	if (direc == 'l') // Sinistra
		rotate_view(&data->dx, &data->dy, -0.1);
	if (direc == 'r') // Destra
		rotate_view(&data->dx, &data->dy, 0.1);

	if (new_x > 0 && new_y > 0 && new_y < data->map_height && new_x < (data->map_width - 1)) {
		if (data->map[(int)new_y][(int)new_x] != '1') {
			data->float_x = new_x;
			data->float_y = new_y;
			mlx_clear_window(data->mlx, data->win);
			data->map[data->y_player][data->x_player] = '0';
			data->x_player = (int)data->float_x;
			data->y_player = (int)data->float_y;
			printf("x_p = %i\n", data->x_player);
			printf("y_p = %i\n", data->y_player);
			data->map[data->y_player][data->x_player] = 'P';
			render_map(data);
			draw_cono(data, data->x_player, data->y_player, data->dx, data->dy, 0x800080);
		} else {
			printf("Wall!!!\n");
		}
	} else {
		printf("Wall!!!\n");
	}
}

void ft_init_data(t_data *data, char *argv)
{
	data->map = open_file(argv);
	data->dx = 0; // Direzione iniziale: "sopra"
	data->dy = -1;

	for (int i = 0; data->map[i]; i++)
		printf("%s", data->map[i]);
	printf("\nmap[0][0] = %c\n", data->map[0][0]);
	calculate_map_dimensions(data);
	printf("width = %i\n", data->map_width);
	printf("width = %i\n", data->map_height);
	data->mlx = mlx_init();
	data->win = mlx_new_window(data->mlx, data->map_width * TILE_SIZE, data->map_height * TILE_SIZE, "Cub3D Map");
	render_map(data);
	mlx_hook(data->win, 17, 0, (int (*)())ft_close, data);
	mlx_hook(data->win, 2, 1L << 0, (int (*)())handle_keypress, data);
	mlx_loop(data->mlx);
}
