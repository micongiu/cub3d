/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_init.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: micongiu <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/25 14:15:11 by micongiu          #+#    #+#             */
/*   Updated: 2025/03/25 14:15:13 by micongiu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../cub3d.h"

// Funzione per mettere un pixel nel buffer dell'immagine
void	my_mlx_pixel_put(t_data *data, int x, int y, int color)
{
	char	*dst;

	dst = data->img_addr + (y * data->line_length
			+ x * (data->bits_per_pixel / 8));
	*(unsigned int *)dst = color;
}

void	ft_init_mlx_2(t_data *data)
{
	if (!data->texture_north.img || !data->texture_south.img
		|| !data->texture_east.img || !data->texture_west.img)
		ft_error("Error loading XPM images");
	data->texture_sky.img = mlx_xpm_file_to_image(data->mlx,
			"./test.xpm/sky.xpm", &data->texture_sky.width,
			&data->texture_sky.height);
	data->texture_grass.img = mlx_xpm_file_to_image(data->mlx,
			"./test.xpm/grass.xpm", &data->texture_grass.width,
			&data->texture_grass.height);
}

void	ft_init_mlx(t_data *data)
{
	data->mlx = mlx_init();
	data->win = mlx_new_window(data->mlx, data->map_width * TILE_SIZE,
			data->map_height * TILE_SIZE, "Cub3D Map");
	data->img_buffer = mlx_new_image(data->mlx, data->map_width * TILE_SIZE,
			data->map_height * TILE_SIZE);
	data->img_addr = mlx_get_data_addr(data->img_buffer, &data->bits_per_pixel,
			&data->line_length, &data->endian);
	data->texture_north.img = mlx_xpm_file_to_image(data->mlx,
			"./test.xpm/b_w_n.xpm", &data->texture_north.width,
			&data->texture_north.height);
	data->texture_south.img = mlx_xpm_file_to_image(data->mlx,
			"./test.xpm/b_w_s.xpm", &data->texture_south.width,
			&data->texture_south.height);
	data->texture_east.img = mlx_xpm_file_to_image(data->mlx,
			"./test.xpm/b_w_o.xpm", &data->texture_east.width,
			&data->texture_east.height);
	data->texture_west.img = mlx_xpm_file_to_image(data->mlx,
			"./test.xpm/b_w_w.xpm", &data->texture_west.width,
			&data->texture_west.height);
	ft_init_mlx_2(data);
}

void	ft_init_data(t_data *data, char *argv)
{
	data->map = open_file(argv);
	calculate_map_dimensions(data);
	if (parser_map(data) == 1)
		ft_error("Error in the parser\n");
	find_player_orientation(data);
	ft_init_mlx(data);
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
