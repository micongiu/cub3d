/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_render_sky_floor.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: micongiu <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/25 14:15:47 by micongiu          #+#    #+#             */
/*   Updated: 2025/03/25 14:15:48 by micongiu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../cub3d.h"

void	render_floor(t_render_data *r_vars, t_data *data)
{
	t_var_count	var_count;

	var_count.i = 0;
	while (var_count.i < r_vars->screen_width)
	{
		var_count.j = r_vars->screen_height / 2;
		while (var_count.j < r_vars->screen_height)
		{
			r_vars->tex_x = (var_count.i * data->texture_grass.width)
				/ r_vars->screen_width;
			r_vars->tex_y = ((var_count.j - r_vars->screen_height / 2)
					* data->texture_grass.height) / (r_vars->screen_height / 2);
			if (r_vars->tex_x >= 0 && r_vars->tex_x < data->texture_grass.width
				&& r_vars->tex_y >= 0
				&& r_vars->tex_y < data->texture_grass.height)
			{
				r_vars->color = data->texture_grass.data[r_vars->tex_y
					* data->texture_grass.width + r_vars->tex_x];
				my_mlx_pixel_put(data, var_count.i,
					var_count.j, r_vars->color);
			}
			var_count.j++;
		}
		var_count.i++;
	}
}

void	render_floor_and_sky(t_render_data *r_vars, t_data *data)
{
	t_var_count	var_count;

	var_count.i = 0;
	while (var_count.i < r_vars->screen_width)
	{
		var_count.j = 0;
		while (var_count.j < r_vars->screen_height / 2)
		{
			r_vars->tex_x = (var_count.i
					* data->texture_sky.width) / r_vars->screen_width;
			r_vars->tex_y = (var_count.j
					* data->texture_sky.height) / (r_vars->screen_height / 2);
			if (r_vars->tex_x >= 0 && r_vars->tex_x < data->texture_sky.width
				&& r_vars->tex_y >= 0
				&& r_vars->tex_y < data->texture_sky.height)
			{
				r_vars->color = data->texture_sky.data[r_vars->tex_y
					* data->texture_sky.width + r_vars->tex_x];
				my_mlx_pixel_put(data, var_count.i, var_count.j, r_vars->color);
			}
			var_count.j++;
		}
		var_count.i++;
	}
	render_floor(r_vars, data);
}
