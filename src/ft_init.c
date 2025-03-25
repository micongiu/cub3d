#include "../cub3d.h"

// Funzione per mettere un pixel nel buffer dell'immagine
void my_mlx_pixel_put(t_data *data, int x, int y, int color)
{
	char *dst = data->img_addr + (y * data->line_length
		+ x * (data->bits_per_pixel / 8));
	*(unsigned int*)dst = color;
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
			if (data->map[i][j] != '1' && data->map[i][j] != '0' &&
				data->map[i][j] != 'N' && data->map[i][j] != 'S' &&
				data->map[i][j] != 'E' && data->map[i][j] != 'W')
			return 1;
			j++;
		}
		i++;
	}
	return 0;
}
void find_player_orientation(t_data *data)
{
	int i = 0;
	while (i < data->map_height)
	{
		int j = 0;
		while (j < data->map_width)
		{
			if (data->map[i][j] == 'N' || data->map[i][j] == 'S' ||
				data->map[i][j] == 'E' || data->map[i][j] == 'W')
			{
				data->x_player = j;
				data->y_player = i;
				if (data->map[i][j] == 'N')
				{
					data->player_angle = M_PI * 2; // 90 gradi
					data->dx = 0;
					data->dy = -1;
				}
				else if (data->map[i][j] == 'S')
				{
					data->player_angle = 3 * (M_PI * 2); // 270 gradi
					data->dx = 0;
					data->dy = 1;
				}
				else if (data->map[i][j] == 'E')
				{
					data->player_angle = 0; // 0 gradi
					data->dx = 1;
					data->dy = 0;
				}
				else if (data->map[i][j] == 'W')
				{
					data->player_angle = M_PI; // 180 gradi
					data->dx = -1;
					data->dy = 0;
				}
				data->map[i][j] = '0'; // Sostituisci la posizione con '0'
				return;
			}
			j++;
		}
		i++;
	}
	ft_error("Player starting position not found");
}

void ft_init_data(t_data *data, char *argv)
{
	data->map = open_file(argv);

	calculate_map_dimensions(data);
	if(parser_map(data) == 1)
		ft_error("Error in the parser\n");
	find_player_orientation(data);
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
