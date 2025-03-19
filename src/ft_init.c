#include "../cub3d.h"

// Funzione per mettere un pixel nel buffer dell'immagine
void my_mlx_pixel_put(t_data *data, int x, int y, int color)
{
	char *dst = data->img_addr + (y * data->line_length + x * (data->bits_per_pixel / 8));
	*(unsigned int*)dst = color;
}

void rotate_view(float *dx, float *dy, float angle)
{
	float new_dx = (*dx) * cos(angle) - (*dy) * sin(angle);
	float new_dy = (*dx) * sin(angle) + (*dy) * cos(angle);

	*dx = new_dx;
	*dy = new_dy;
}

// Funzione per convertire i dati della texture in un array di interi
void convert_texture_data(t_data *data, t_texture *texture)
{
	(void)data;
	texture->data = (int *)malloc(sizeof(int) * texture->width * texture->height);
	if (!texture->data)
		return;

	void *addr = mlx_get_data_addr(texture->img, &texture->bits_per_pixel,
								&texture->line_length, &texture->endian);

	for (int y = 0; y < texture->height; y++)
	{
		for (int x = 0; x < texture->width; x++)
		{
			int pixel = y * texture->line_length + x * 4;
			char *ptr = addr + pixel;
			texture->data[y * texture->width + x] = ((unsigned char)ptr[2] << 16) +
												((unsigned char)ptr[1] << 8) +
												((unsigned char)ptr[0]);
		}
	}
}

void render_3d(t_data *data)
{
	int screen_width = data->map_width * TILE_SIZE;
	int screen_height = data->map_height * TILE_SIZE;

	// Pulisci il buffer dell'immagine
	ft_memset(data->img_addr, 0, screen_width * screen_height * (data->bits_per_pixel / 8));

	// Renderizza il cielo (metà superiore dello schermo)
	for (int i = 0; i < screen_width; i++) {
		for (int y = 0; y < screen_height / 2; y++) {
			my_mlx_pixel_put(data, i, y, 0xA020F0); // Viola per il cielo
		}
	}

	// Renderizza il pavimento (metà inferiore dello schermo)
	for (int i = 0; i < screen_width; i++) {
		for (int y = screen_height / 2; y < screen_height; y++) {
			my_mlx_pixel_put(data, i, y, 0x83f52c); // Verde per il pavimento
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
	for (int x = 0; x < ray_n; x++)
	{
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
		float max_distance = 30.0; // Distanza massima di ricerca
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

		// Calcola la coordinata esatta del punto di impatto sul muro
		float wall_x;
		if (hit_side == 0) {
			wall_x = ray_y + distance * ray_dy;
		} else {
			wall_x = ray_x + distance * ray_dx;
		}
		wall_x -= floor(wall_x); // Mantieni solo la parte frazionaria (0-1)

		// Calcola la texture da utilizzare
		t_texture *current_texture;
		if (hit_side == 0) { // Muro verticale (Est/Ovest)
			if (step_x > 0)
				current_texture = &data->texture_east; // Est
			else
				current_texture = &data->texture_west; // Ovest
		} else { // Muro orizzontale (Nord/Sud)
			if (step_y > 0)
				current_texture = &data->texture_south; // Sud
			else
				current_texture = &data->texture_north; // Nord
		}

		// Calcola la coordinata x della texture
		int tex_x = (int)(wall_x * current_texture->width);
		if ((hit_side == 0 && ray_dx > 0) || (hit_side == 1 && ray_dy < 0)) {
			tex_x = current_texture->width - tex_x - 1;
		}

		// Calcola dove iniziare e finire di disegnare la linea verticale
		int draw_start = (screen_height - line_height) / 2;
		if (draw_start < 0) draw_start = 0;

		int draw_end = (screen_height + line_height) / 2;
		if (draw_end >= screen_height) draw_end = screen_height - 1;

		// Disegna la linea verticale con la texture selezionata
		for (int y = draw_start; y <= draw_end; y++) {
			int tex_y = (int)((y - draw_start) * current_texture->height / line_height);
			int color = current_texture->data[tex_y * current_texture->width + tex_x];

			// Applica ombreggiatura per l'effetto di profondità
			float shade = 1.0 / (1.0 + distance * 0.1);
			if (shade > 1.0) shade = 1.0;

			int r = (int)(((color >> 16) & 0xFF) * shade);
			int g = (int)(((color >> 8) & 0xFF) * shade);
			int b = (int)((color & 0xFF) * shade);

			color = (r << 16) | (g << 8) | b;

			// Disegna il pixel
			my_mlx_pixel_put(data, x, y, color);
		}

		// Incrementa l'angolo per il prossimo raggio
		ray_angle += angle_step;
	}

	// Alla fine del rendering, visualizza il buffer
	mlx_put_image_to_window(data->mlx, data->win, data->img_buffer, 0, 0);
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
	// t_texture	texture;
	data->map = open_file(argv);
	data->dx = 0;
	data->dy = -1;

	calculate_map_dimensions(data);
	data->mlx = mlx_init();

	// Crea la finestra
	data->win = mlx_new_window(data->mlx, data->map_width * TILE_SIZE, data->map_height * TILE_SIZE, "Cub3D Map");

	// Crea il buffer dell'immagine
	data->img_buffer = mlx_new_image(data->mlx, data->map_width * TILE_SIZE, data->map_height * TILE_SIZE);
	data->img_addr = mlx_get_data_addr(data->img_buffer, &data->bits_per_pixel, &data->line_length, &data->endian);

	// Carica le texture per ogni direzione
	data->texture_north.img = mlx_xpm_file_to_image(data->mlx, "./test.xpm/b_w_n.xpm", &data->texture_north.width, &data->texture_north.height);
	data->texture_south.img = mlx_xpm_file_to_image(data->mlx, "./test.xpm/b_w_s.xpm", &data->texture_south.width, &data->texture_south.height);
	data->texture_east.img = mlx_xpm_file_to_image(data->mlx, "./test.xpm/b_w_o.xpm", &data->texture_east.width, &data->texture_east.height);
	data->texture_west.img = mlx_xpm_file_to_image(data->mlx, "./test.xpm/b_w_w.xpm", &data->texture_west.width, &data->texture_west.height);

	// Controlla se le texture sono state caricate correttamente
	if (!data->texture_north.img || !data->texture_south.img || !data->texture_east.img || !data->texture_west.img) {
		perror("Error loading XPM images");
		return;
	}

	// Converti i dati delle texture
	convert_texture_data(data, &data->texture_north);
	convert_texture_data(data, &data->texture_south);
	convert_texture_data(data, &data->texture_east);
	convert_texture_data(data, &data->texture_west);

	// Configura gli hook per gli eventi
	mlx_hook(data->win, 17, 0, (int (*)())ft_close, data);
	mlx_hook(data->win, 2, 1L << 0, (int (*)())handle_keypress, data);

	// Renderizza la scena iniziale
	render_3d(data);
	// Avvia il loop degli eventi
	mlx_loop(data->mlx);
}
