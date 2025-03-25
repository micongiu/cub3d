#include "../cub3d.h"

void	render_floor(t_var_count var_count,
	t_render_data *render_vars, t_data *data, t_render	var_r)
{
	var_count.i = 0;
	while (var_count.i < render_vars->screen_width)
	{
		var_count.j = render_vars->screen_height / 2;
		while (var_count.j < render_vars->screen_height)
		{
			var_r.tex_x = (var_count.i * data->texture_grass.width)
				/ render_vars->screen_width;
			var_r.tex_y = ((var_count.j - render_vars->screen_height / 2)
					* data->texture_grass.height)
				/ (render_vars->screen_height / 2);
			if (var_r.tex_x >= 0 && var_r.tex_x < data->texture_grass.width
				&& var_r.tex_y >= 0 && var_r.tex_y < data->texture_grass.height)
			{
				var_r.color = data->texture_grass.data[var_r.tex_y
					* data->texture_grass.width + var_r.tex_x];
				my_mlx_pixel_put(data, var_count.i, var_count.j, var_r.color);
			}
			var_count.j++;
		}
		var_count.i++;
	}
}

void	render_floor_and_sky(t_render_data *render_vars, t_data *data)
{
	t_var_count	var_count;
	t_render	var_r;

	var_count.i = 0;
	while (var_count.i < render_vars->screen_width)
	{
		var_count.j = 0;
		while (var_count.j < render_vars->screen_height / 2)
		{
			var_r.tex_x = (var_count.i * data->texture_sky.width)
				/ render_vars->screen_width;
			var_r.tex_y = (var_count.j * data->texture_sky.height)
				/ (render_vars->screen_height / 2);
			if (var_r.tex_x >= 0 && var_r.tex_x < data->texture_sky.width
				&& var_r.tex_y >= 0 && var_r.tex_y < data->texture_sky.height)
			{
				var_r.color = data->texture_sky.data[var_r.tex_y
					* data->texture_sky.width + var_r.tex_x];
				my_mlx_pixel_put(data, var_count.i, var_count.j, var_r.color);
			}
			var_count.j++;
		}
		var_count.i++;
	}
	render_floor(var_count, render_vars, data, var_r);
}

void	dda(t_render_data *r_vars, t_data *data)
{
	while (!r_vars->hit_wall && r_vars->distance < r_vars->max_distance)
	{
		if (r_vars->side_dist_x < r_vars->side_dist_y)
		{
			r_vars->side_dist_x += r_vars->delta_dist_x;
			r_vars->map_x += r_vars->step_x;
			r_vars->hit_side = 0;
			r_vars->distance = r_vars->side_dist_x - r_vars->delta_dist_x;
		}
		else
		{
			r_vars->side_dist_y += r_vars->delta_dist_y;
			r_vars->map_y += r_vars->step_y;
			r_vars->hit_side = 1;
			r_vars->distance = r_vars->side_dist_y - r_vars->delta_dist_y;
		}
		if (r_vars->map_y >= 0 && r_vars->map_x >= 0 && r_vars->map_y
			< data->map_height && r_vars->map_x < data->map_width)
		{
			if (data->map[r_vars->map_y][r_vars->map_x] == '1')
				r_vars->hit_wall = 1;
		}
		else
			break ;
	}
}

void	render_int(t_data *data, t_render_data	*render_vars)
{
	render_vars->screen_width = data->map_width * TILE_SIZE;
	render_vars->screen_height = data->map_height * TILE_SIZE;
	ft_memset(data->img_addr, 0, render_vars->screen_width
		* render_vars->screen_height * (data->bits_per_pixel / 8));
	render_floor_and_sky(render_vars, data);
	render_vars->fov = 1.0472;
	render_vars->ray_n = render_vars->screen_width;
	render_vars->angle_step = render_vars->fov / render_vars->ray_n;
	render_vars->player_angle = atan2(data->dy, data->dx);
	render_vars->ray_angle = render_vars->player_angle - (render_vars->fov / 2);
}

void	render_3d_loop(t_render_data *r_vars, t_data *data, int x, int y)
{
	r_vars->draw_start = (r_vars->screen_height - r_vars->line_height) / 2;
	if (r_vars->draw_start < 0)
		r_vars->draw_start = 0;
	r_vars->draw_end = (r_vars->screen_height + r_vars->line_height) / 2;
	if (r_vars->draw_end >= r_vars->screen_height)
		r_vars->draw_end = r_vars->screen_height - 1;
	while (y < r_vars->draw_end)
	{
		r_vars->tex_y = (int)((y - r_vars->draw_start)
				* r_vars->current_texture->height / r_vars->line_height);
		r_vars->color = r_vars->current_texture->data[r_vars->tex_y
			* r_vars->current_texture->width + r_vars->tex_x];
		my_mlx_pixel_put(data, x, y, r_vars->color);
		y++;
	}
}

void	render_3d(t_data *data)
{
	t_render_data	r_vars;
	int				x;
	int				y;

	render_int(data, &r_vars);
	x = 0;
	y = 0;
	while (x < r_vars.ray_n)
	{
		set_up_3d_render(&r_vars, data);
		calculate_initial_lateral_distance(&r_vars);
		r_vars.max_distance = 30.0;
		dda(&r_vars, data);
		calculate_the_exact_coordinate_impact_point(&r_vars);
		r_vars.current_texture = calculate_the_texture_to_use(&r_vars, data);
		r_vars.tex_x = (int)(r_vars.wall_x * r_vars.current_texture->width);
		if ((r_vars.hit_side == 0 && r_vars.ray_dx > 0)
			|| (r_vars.hit_side == 1 && r_vars.ray_dy < 0))
			r_vars.tex_x = r_vars.current_texture->width - r_vars.tex_x - 1;
		calculate_the_height_line_to_be_drawn_on_screen(&r_vars);
		render_3d_loop(&r_vars, data, x, y);
		r_vars.ray_angle += r_vars.angle_step;
		x++;
	}
	mlx_put_image_to_window(data->mlx, data->win, data->img_buffer, 0, 0);
}
