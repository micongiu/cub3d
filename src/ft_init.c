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
	texture->data = (int *)ft_calloc(sizeof(int), texture->width * texture->height);
	if (!texture->data)
		ft_error("no texture");
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
	t_render_data render_vars;

	render_vars.screen_width = data->map_width * TILE_SIZE;
	render_vars.screen_height = data->map_height * TILE_SIZE;

	// Pulisci il buffer dell'immagine
	ft_memset(data->img_addr, 0, render_vars.screen_width * render_vars.screen_height * (data->bits_per_pixel / 8));

	// Renderizza il cielo (metà superiore dello schermo)
	// Renderizza il cielo
	for (int i = 0; i < render_vars.screen_width; i++) {
		for (int y = 0; y < render_vars.screen_height / 2; y++) {
			int tex_x = (i * data->texture_sky.width) / render_vars.screen_width;
			int tex_y = (y * data->texture_sky.height) / (render_vars.screen_height / 2);
			int color = data->texture_sky.data[tex_y * data->texture_sky.width + tex_x];
			my_mlx_pixel_put(data, i, y, color);
		}
	}

	// Renderizza il pavimento
	for (int i = 0; i < render_vars.screen_width; i++) {
		for (int y = render_vars.screen_height / 2; y < render_vars.screen_height; y++) {
			int tex_x = (i * data->texture_grass.width) / render_vars.screen_width;
			int tex_y = ((y - render_vars.screen_height / 2) * data->texture_grass.height) / (render_vars.screen_height / 2);
			int color = data->texture_grass.data[tex_y * data->texture_grass.width + tex_x];
			my_mlx_pixel_put(data, i, y, color);
		}
	}

	// Configurazione del campo visivo e del raycasting
	render_vars.fov = 1.0472; // 60 gradi in radianti
	render_vars.ray_n = render_vars.screen_width; // Un raggio per ogni colonna dello schermo
	render_vars.angle_step = render_vars.fov / render_vars.ray_n;

	// Calcola l'angolo del giocatore dai vettori di direzione
	render_vars.player_angle = atan2(data->dy, data->dx);
	render_vars.ray_angle = render_vars.player_angle - (render_vars.fov / 2);

	// Esegui il raycasting per ogni colonna dello schermo
	for (int x = 0; x < render_vars.ray_n; x++)
	{
		// Calcola la direzione del raggio
		render_vars.ray_dx = cos(render_vars.ray_angle);
		render_vars.ray_dy = sin(render_vars.ray_angle);

		// Posizione iniziale del raggio (posizione del giocatore)
		render_vars.ray_x = data->float_x;
		render_vars.ray_y = data->float_y;

		// Distanza percorsa dal raggio
		render_vars.distance = 0.0;
		render_vars.hit_wall = 0;
		render_vars.hit_side = 0;

		// DDA (Digital Differential Analysis) per il raycasting
		render_vars.delta_dist_x = fabs(1.0 / render_vars.ray_dx);
		render_vars.delta_dist_y = fabs(1.0 / render_vars.ray_dy);

		// Calcola la distanza fino al prossimo bordo della griglia
		render_vars.map_x = (int)render_vars.ray_x;
		render_vars.map_y = (int)render_vars.ray_y;

		// Calcola la distanza laterale iniziale
		if (render_vars.ray_dx < 0) {
			render_vars.step_x = -1;
			render_vars.side_dist_x = (render_vars.ray_x - render_vars.map_x) * render_vars.delta_dist_x;
		} else {
			render_vars.step_x = 1;
			render_vars.side_dist_x = (render_vars.map_x + 1.0 - render_vars.ray_x) * render_vars.delta_dist_x;
		}

		if (render_vars.ray_dy < 0) {
			render_vars.step_y = -1;
			render_vars.side_dist_y = (render_vars.ray_y - render_vars.map_y) * render_vars.delta_dist_y;
		} else {
			render_vars.step_y = 1;
			render_vars.side_dist_y = (render_vars.map_y + 1.0 - render_vars.ray_y) * render_vars.delta_dist_y;
		}

		// Esegui DDA
		render_vars.max_distance = 30.0; // Distanza massima di ricerca
		while (!render_vars.hit_wall && render_vars.distance < render_vars.max_distance) {
			// Salta al prossimo quadrato della mappa
			if (render_vars.side_dist_x < render_vars.side_dist_y) {
				render_vars.side_dist_x += render_vars.delta_dist_x;
				render_vars.map_x += render_vars.step_x;
				render_vars.hit_side = 0; // Colpito un lato verticale (est/ovest)
				render_vars.distance = render_vars.side_dist_x - render_vars.delta_dist_x;
			} else {
				render_vars.side_dist_y += render_vars.delta_dist_y;
				render_vars.map_y += render_vars.step_y;
				render_vars.hit_side = 1; // Colpito un lato orizzontale (nord/sud)
				render_vars.distance = render_vars.side_dist_y - render_vars.delta_dist_y;
			}

			// Verifica se abbiamo colpito un muro
			if (render_vars.map_y >= 0 && render_vars.map_x >= 0 && render_vars.map_y < data->map_height && render_vars.map_x < data->map_width) {
				if (data->map[render_vars.map_y][render_vars.map_x] == '1') {
					render_vars.hit_wall = 1;
				}
			} else {
				break; // Siamo usciti dalla mappa
			}
		}

		// Calcola l'altezza della linea da disegnare sullo schermo
		int line_height;
		if (render_vars.hit_wall) {
			// Correggi l'effetto fisheye moltiplicando per cos(angolo_raggio - angolo_giocatore)
			render_vars.corrected_distance = render_vars.distance * cos(render_vars.ray_angle - render_vars.player_angle);
			line_height = (int)(render_vars.screen_height / render_vars.corrected_distance);
		} else {
			line_height = 0;
		}

		// Calcola la coordinata esatta del punto di impatto sul muro
		float wall_x;
		if (render_vars.hit_side == 0) {
			wall_x = render_vars.ray_y + render_vars.distance * render_vars.ray_dy;
		} else {
			wall_x = render_vars.ray_x + render_vars.distance * render_vars.ray_dx;
		}
		wall_x -= floor(wall_x); // Mantieni solo la parte frazionaria (0-1)

		// Calcola la texture da utilizzare
		t_texture *current_texture;
		if (render_vars.hit_side == 0) { // Muro verticale (Est/Ovest)
			if (render_vars.step_x > 0)
				current_texture = &data->texture_east; // Est
			else
				current_texture = &data->texture_west; // Ovest
		} else { // Muro orizzontale (Nord/Sud)
			if (render_vars.step_y > 0)
				current_texture = &data->texture_south; // Sud
			else
				current_texture = &data->texture_north; // Nord
		}
		render_vars.current_texture = current_texture;

		// Calcola la coordinata x della texture
		render_vars.tex_x = (int)(wall_x * render_vars.current_texture->width);
		if ((render_vars.hit_side == 0 && render_vars.ray_dx > 0) || (render_vars.hit_side == 1 && render_vars.ray_dy < 0)) {
			render_vars.tex_x = render_vars.current_texture->width - render_vars.tex_x - 1;
		}

		// Calcola dove iniziare e finire di disegnare la linea verticale
		render_vars.draw_start = (render_vars.screen_height - line_height) / 2;
		if (render_vars.draw_start < 0) render_vars.draw_start = 0;

		render_vars.draw_end = (render_vars.screen_height + line_height) / 2;
		if (render_vars.draw_end >= render_vars.screen_height) render_vars.draw_end = render_vars.screen_height - 1;

		// Disegna la linea verticale con la texture selezionata
		for (int y = render_vars.draw_start; y <= render_vars.draw_end; y++) {
			render_vars.tex_y = (int)((y - render_vars.draw_start) * render_vars.current_texture->height / line_height);
			render_vars.color = render_vars.current_texture->data[render_vars.tex_y * render_vars.current_texture->width + render_vars.tex_x];

			// Applica ombreggiatura per l'effetto di profondità
			render_vars.shade = 1.0 / (1.0 + render_vars.distance * 0.1);
			if (render_vars.shade > 1.0) render_vars.shade = 1.0;

			render_vars.r = (int)(((render_vars.color >> 16) & 0xFF) * render_vars.shade);
			render_vars.g = (int)(((render_vars.color >> 8) & 0xFF) * render_vars.shade);
			render_vars.b = (int)((render_vars.color & 0xFF) * render_vars.shade);

			render_vars.color = (render_vars.r << 16) | (render_vars.g << 8) | render_vars.b;

			// Disegna il pixel
			my_mlx_pixel_put(data, x, y, render_vars.color);
		}

		// Incrementa l'angolo per il prossimo raggio
		render_vars.ray_angle += render_vars.angle_step;
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

int parser_map(t_data *data)
{
	int i = 0;
	int j = 0;

	while (j < data->map_width - 1)
	{
		if (data->map[0][j] != '1')
			return 1;
		j++;
	}
	j = 0;
	while (j < data->map_width - 1)
	{
		if (data->map[data->map_height -1][j] != '1')
		return 1;
		j++;
	}
	while (i < data->map_height - 1)
	{
		if (data->map[i][0] != '1' || data->map[i][data->map_width - 1] != '1')
		return 1;

		j = 0;
		while (j < data->map_width - 1)
		{
			if (data->map[i][j] != '1' && data->map[i][j] != '0' && data->map[i][j] != 'P')
			return 1;
			j++;
		}
		i++;
	}
	return 0;
}

void ft_init_data(t_data *data, char *argv)
{
	data->map = open_file(argv);
	data->dx = 0;
	data->dy = -1;
	calculate_map_dimensions(data);
	if(parser_map(data) == 1)
		ft_error("Error in the parser\n");
	data->mlx = mlx_init();
	data->win = mlx_new_window(data->mlx, data->map_width * TILE_SIZE, data->map_height * TILE_SIZE, "Cub3D Map");
	data->img_buffer = mlx_new_image(data->mlx, data->map_width * TILE_SIZE, data->map_height * TILE_SIZE);
	data->img_addr = mlx_get_data_addr(data->img_buffer, &data->bits_per_pixel, &data->line_length, &data->endian);
	data->texture_north.img = mlx_xpm_file_to_image(data->mlx, "./test.xpm/b_w_n.xpm", &data->texture_north.width, &data->texture_north.height);
	data->texture_south.img = mlx_xpm_file_to_image(data->mlx, "./test.xpm/b_w_s.xpm", &data->texture_south.width, &data->texture_south.height);
	data->texture_east.img = mlx_xpm_file_to_image(data->mlx, "./test.xpm/b_w_o.xpm", &data->texture_east.width, &data->texture_east.height);
	data->texture_west.img = mlx_xpm_file_to_image(data->mlx, "./test.xpm/b_w_w.xpm", &data->texture_west.width, &data->texture_west.height);
	if (!data->texture_north.img || !data->texture_south.img || !data->texture_east.img || !data->texture_west.img)
		ft_error("Error loading XPM images");
	data->texture_sky.img = mlx_xpm_file_to_image(data->mlx, "./test.xpm/sky.xpm", &data->texture_sky.width, &data->texture_sky.height);
	data->texture_grass.img = mlx_xpm_file_to_image(data->mlx, "./test.xpm/grass.xpm", &data->texture_grass.width, &data->texture_grass.height);
	if (!data->texture_sky.img || !data->texture_grass.img)
		ft_error("Error loading sky or grass XPM images");
	convert_texture_data(data, &data->texture_sky);
	convert_texture_data(data, &data->texture_grass);
	convert_texture_data(data, &data->texture_north);
	convert_texture_data(data, &data->texture_south);
	convert_texture_data(data, &data->texture_east);
	convert_texture_data(data, &data->texture_west);
	mlx_hook(data->win, 17, 0, (int (*)())ft_close, data);
	mlx_hook(data->win, 2, 1L << 0, (int (*)())handle_keypress, data);
	render_3d(data);
	mlx_loop(data->mlx);
}
