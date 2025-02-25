#include "../cub3d.h"

// void xmp_init(t_data *data)
// {
// 	data
// }


void rotate_view(float *dx, float *dy, float angle)
{
	float new_dx = (*dx) * cos(angle) - (*dy) * sin(angle);
	float new_dy = (*dx) * sin(angle) + (*dy) * cos(angle);

	*dx = new_dx;
	*dy = new_dy;
}

void render_3d(t_data *data)
{
	int i = 0;
	int y = 0;
	while (i <= data->map_width * TILE_SIZE) {
		y = 0;
		while (y <= data->map_height * TILE_SIZE / 2)
		{
			mlx_pixel_put(data->mlx, data->win, i, y, 0xA020F0);
			y++;
		}
		i++;
	}
	i = 0;
	while (i <= data->map_width * TILE_SIZE) {
		y = data->map_height * TILE_SIZE / 2;
		while (y <= data->map_height * TILE_SIZE)
		{
			mlx_pixel_put(data->mlx, data->win, i, y, 0x83f52c);
			y++;
		}
		i++;
	}
	int screen_width = data->map_width * TILE_SIZE;
	int screen_height = data->map_height * TILE_SIZE;
	float fov = 1.484; // Circa 66.5 gradi
	int ray_n = screen_width;
	float angle_step = fov / ray_n;

	for (int x = 0; x < ray_n; x++)
	{
		float angle = -fov / 2 + x * angle_step;
		float ray_dx = data->dx * cos(angle) - data->dy * sin(angle);
		float ray_dy = data->dx * sin(angle) + data->dy * cos(angle);

		float pixel_x = data->float_x;
		float pixel_y = data->float_y;
		float distance = 0;
		int hit_side = 0;

		// Lancia il raggio e trova il muro
		while (distance < 50)
		{
			int map_x = (int)pixel_x;
			int map_y = (int)pixel_y;
			if (data->map[map_y][map_x] == '1')
			{
				// Determina se è un muro verticale o orizzontale
				float diff_x = fabs(pixel_x - map_x);
				float diff_y = fabs(pixel_y - map_y);
				if (diff_x > diff_y)
					hit_side = 1; // Orizzontale (Nord/Sud)
				else
					hit_side = 0; // Verticale (Est/Ovest)
				break;
			}
			pixel_x += ray_dx * 0.05;
			pixel_y += ray_dy * 0.05;
			distance += 0.05;
		}

		// Evita divisione per 0
		if (distance < 0.01)
			distance = 0.01;

		// Calcola l'altezza della colonna per effetto prospettico
		int wall_height = (int)(screen_height / (distance * cos(angle)));

		// Determina il colore del muro in base alla direzione
		int color;
		if (hit_side == 1) // Nord o Sud (muri orizzontali)
			color = (ray_dy > 0) ? 0xFF0000 : 0x00FF00; // Sud (rosso) / Nord (verde)
		else // Est o Ovest (muri verticali)
			color = (ray_dx > 0) ? 0x0000FF : 0xFFFF00; // Est (blu) / Ovest (giallo)

		// Disegna colonna verticale (linea) per rappresentare il muro
		int start = (screen_height / 2) - (wall_height / 2);
		int end = (screen_height / 2) + (wall_height / 2);
		if (start < 0) start = 0;
		if (end >= screen_height) end = screen_height - 1;

		for (int y = start; y < end; y++)
		{
			mlx_pixel_put(data->mlx, data->win, x, y, color);
		}
	}
}


void move_p(t_data *data, char direc)
{
	float new_x = data->float_x;
	float new_y = data->float_y;

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
	if (direc == 'l') // Sinistra
		rotate_view(&data->dx, &data->dy, -0.1);
	if (direc == 'r') // Destra
		rotate_view(&data->dx, &data->dy, 0.1);
	if (new_x > 0 && new_y > 0 && new_y < data->map_height && new_x < (data->map_width - 1)) {
		if (data->map[(int)new_y][(int)new_x] != '1') {
			data->float_x = new_x;
			data->float_y = new_y;
			data->map[data->y_player][data->x_player] = '0';
			data->x_player = (int)data->float_x;
			data->y_player = (int)data->float_y;
			data->map[data->y_player][data->x_player] = 'P';
			mlx_clear_window(data->mlx, data->win);
			render_3d(data);
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
	data->dx = 0;
	data->dy = -1;

	for (int i = 0; data->map[i]; i++)
		printf("%s", data->map[i]);
	calculate_map_dimensions(data);
	// xmp_init(data);
	data->mlx = mlx_init();
	data->win = mlx_new_window(data->mlx, data->map_width * TILE_SIZE, data->map_height * TILE_SIZE, "Cub3D Map");
	int		bits_per_pixel;
	int		line_length;
	int endian;
	char	*path = "./test.xpm/my_img.xmp";
	data->img = mlx_xpm_file_to_image(data->mlx, path, &line_length, &bits_per_pixel);
	if (!data->img) {
        perror("Error loading XPM image");
        return ;
    }
	mlx_get_data_addr(data->img, &bits_per_pixel, &line_length, &endian);
    mlx_put_image_to_window(data->mlx, data->win, data->img, 0, 0);
	mlx_hook(data->win, 17, 0, (int (*)())ft_close, data);
	mlx_hook(data->win, 2, 1L << 0, (int (*)())handle_keypress, data);
	mlx_loop(data->mlx);

    // Display the image in the window.
	render_3d(data);
}
