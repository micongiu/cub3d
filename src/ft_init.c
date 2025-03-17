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
	int screen_width = data->map_width * TILE_SIZE;
	int screen_height = data->map_height * TILE_SIZE;

	// Renderizza il cielo (metà superiore dello schermo)
	for (int i = 0; i < screen_width; i++) {
		for (int y = 0; y < screen_height / 2; y++) {
			mlx_pixel_put(data->mlx, data->win, i, y, 0xA020F0); // Viola per il cielo
		}
	}

	// Renderizza il pavimento (metà inferiore dello schermo)
	for (int i = 0; i < screen_width; i++) {
		for (int y = screen_height / 2; y < screen_height; y++) {
			mlx_pixel_put(data->mlx, data->win, i, y, 0x83f52c); // Verde per il pavimento
		}
	}

	// Configurazione del campo visivo e del raycasting
	float fov = 1.0472; // 60 gradi in radianti
	int ray_n = screen_width; // Un raggio per ogni colonna dello schermo
	float angle_step = fov / ray_n;

	// Calcola l'angolo del giocatore dai vettori di direzione
	float player_angle = atan2(data->dy, data->dx);
	float ray_angle = player_angle - (fov / 2);

	// Esegui il raycasting per ogni colonna dello schermo
	for (int x = 0; x < ray_n; x++) {
		// Calcola la direzione del raggio
		float ray_dx = cos(ray_angle);
		float ray_dy = sin(ray_angle);

		// Posizione iniziale del raggio (posizione del giocatore)
		float ray_x = data->float_x;
		float ray_y = data->float_y;

		// Distanza percorsa dal raggio
		float distance = 0.0;
		int hit_wall = 0;
		int hit_side = 0;

		// DDA (Digital Differential Analysis) per il raycasting
		float delta_dist_x = fabs(1.0 / ray_dx);
		float delta_dist_y = fabs(1.0 / ray_dy);

		// Calcola la distanza fino al prossimo bordo della griglia
		int map_x = (int)ray_x;
		int map_y = (int)ray_y;

		float side_dist_x, side_dist_y;
		int step_x, step_y;

		// Determina il passo e la distanza laterale iniziale
		if (ray_dx < 0) {
			step_x = -1;
			side_dist_x = (ray_x - map_x) * delta_dist_x;
		} else {
			step_x = 1;
			side_dist_x = (map_x + 1.0 - ray_x) * delta_dist_x;
		}

		if (ray_dy < 0) {
			step_y = -1;
			side_dist_y = (ray_y - map_y) * delta_dist_y;
		} else {
			step_y = 1;
			side_dist_y = (map_y + 1.0 - ray_y) * delta_dist_y;
		}

		// Esegui DDA
		float max_distance = 20.0; // Distanza massima di ricerca
		while (!hit_wall && distance < max_distance) {
			// Salta al prossimo quadrato della mappa
			if (side_dist_x < side_dist_y) {
				side_dist_x += delta_dist_x;
				map_x += step_x;
				hit_side = 0; // Colpito un lato verticale (est/ovest)
				distance = side_dist_x - delta_dist_x;
			} else {
				side_dist_y += delta_dist_y;
				map_y += step_y;
				hit_side = 1; // Colpito un lato orizzontale (nord/sud)
				distance = side_dist_y - delta_dist_y;
			}

			// Verifica se abbiamo colpito un muro
			if (map_y >= 0 && map_x >= 0 && map_y < data->map_height && map_x < data->map_width) {
				if (data->map[map_y][map_x] == '1') {
					hit_wall = 1;
				}
			} else {
				break; // Siamo usciti dalla mappa
			}
		}

		// Calcola l'altezza della linea da disegnare sullo schermo
		int line_height;
		if (hit_wall) {
			// Correggi l'effetto fisheye moltiplicando per cos(angolo_raggio - angolo_giocatore)
			float corrected_distance = distance * cos(ray_angle - player_angle);
			line_height = (int)(screen_height / corrected_distance);
		} else {
			line_height = 0;
		}

		// Determina il colore del muro in base alla direzione
		int color;
		if (hit_side == 1) { // Nord o Sud (muri orizzontali)
			color = (step_y > 0) ? 0xFF0000 : 0x00FF00; // Sud (rosso) / Nord (verde)
		} else { // Est o Ovest (muri verticali)
			color = (step_x > 0) ? 0x0000FF : 0xFFFF00; // Est (blu) / Ovest (giallo)
		}

		// Calcola dove iniziare e finire di disegnare la linea verticale
		int draw_start = (screen_height - line_height) / 2;
		if (draw_start < 0) draw_start = 0;

		int draw_end = (screen_height + line_height) / 2;
		if (draw_end >= screen_height) draw_end = screen_height - 1;

		// Disegna la linea verticale
		for (int y = draw_start; y <= draw_end; y++) {
			mlx_pixel_put(data->mlx, data->win, x, y, color);
		}

		// Incrementa l'angolo per il prossimo raggio
		ray_angle += angle_step;
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

unsigned int	get_color_5(int door_line_length, void* door_adr, int x, int y)
{
	char	*ptr;
	int		pixel;

	pixel = y * door_line_length + x * 4;
	ptr = door_adr + pixel;
	return ((((unsigned char)ptr[2]) << 16)
		+ (((unsigned char)ptr[1]) << 8) + ((unsigned char)ptr[0]));
}

void ft_init_data(t_data *data, char *argv)
{
	data->map = open_file(argv);
	data->dx = 0;
	data->dy = -1;

	calculate_map_dimensions(data);
	data->mlx = mlx_init();
	data->win = mlx_new_window(data->mlx, data->map_width * TILE_SIZE, data->map_height * TILE_SIZE, "Cub3D Map");
	int		door_w;
	int		door_h;
	int		door_endian;
	int		door_line_length;
	int		door_bits_per_pixel;
	char	*path = "./test.xpm/my_img.xmp";
	data->img = mlx_xpm_file_to_image(data->mlx, path, &door_w, &door_h);
	if (!data->img) {
		perror("Error loading XPM image");
		return ;
	}
	void* door_adr = mlx_get_data_addr(data->img, &door_bits_per_pixel, &door_line_length, &door_endian);
	int *door_buff = ft_calloc(4, (door_h * door_w));
	int i = -1;
	int j = 0;
	int	k = 0;
	while (++i < door_h)
	{
		j = 0;
		while (j < door_w)
		{
			door_buff[k] = get_color_5(door_line_length, door_adr, j, i);
			j++;
			k++;
		}
	}
	for (size_t d = 20; d < 600; d++)
	{
		mlx_pixel_put(data->mlx, data->win, d, 50, door_buff[0]);
		mlx_pixel_put(data->mlx, data->win, d, 51, door_buff[1]);
		mlx_pixel_put(data->mlx, data->win, d, 52, door_buff[2]);
		mlx_pixel_put(data->mlx, data->win, d, 53, door_buff[3]);
		mlx_pixel_put(data->mlx, data->win, d, 54, door_buff[4]);
		mlx_pixel_put(data->mlx, data->win, d, 55, door_buff[5]);
		mlx_pixel_put(data->mlx, data->win, d, 56, door_buff[6]);
		mlx_pixel_put(data->mlx, data->win, d, 75, door_buff[137]);
		mlx_pixel_put(data->mlx, data->win, d, 76, door_buff[137]);
		mlx_pixel_put(data->mlx, data->win, d, 77, door_buff[137]);
	}

	mlx_put_image_to_window(data->mlx, data->win, data->img, 0, 0);
	mlx_hook(data->win, 17, 0, (int (*)())ft_close, data);
	mlx_hook(data->win, 2, 1L << 0, (int (*)())handle_keypress, data);
	mlx_loop(data->mlx);

	// Display the image in the window.
	render_3d(data);
}
