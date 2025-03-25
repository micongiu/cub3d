#include "../cub3d.h"

// t_var_count var_count;
// int	tex_x;
// int	tex_y;
// int	color;

// var_count.i = 0;
// while (var_count.i < render_vars->screen_width)
// {
// 	var_count.j = 0;
// 	while (var_count.j < render_vars->screen_height / 2)
// 	{
// 		tex_x = (var_count.i * data->texture_sky.width) / render_vars->screen_width;
// 		tex_y = (var_count.j * data->texture_sky.height) / (render_vars->screen_height / 2);
// 		color = data->texture_sky.data[tex_y * data->texture_sky.width + tex_x];
// 		my_mlx_pixel_put(data, var_count.i, var_count.j, color);
// 		var_count.j++;
// 	}
// 	var_count.i++;
// }
// var_count.i = 0;
// while (var_count.i < render_vars->screen_width)
// {
// 	var_count.j = 0;
// 	while (var_count.j < render_vars->screen_height / 2)
// 	{
// 		tex_x = (var_count.i * data->texture_grass.width) / render_vars->screen_width;
// 		tex_y = ((var_count.j - render_vars->screen_height / 2) * data->texture_grass.height) / (render_vars->screen_height / 2);
// 		color = data->texture_grass.data[tex_y * data->texture_grass.width + tex_x];
// 		my_mlx_pixel_put(data, var_count.i, var_count.j, color);
// 		var_count.j++;
// 	}
// 	var_count.i++;
// }
void	render_floor_and_sky(t_render_data *render_vars, t_data *data)
{
	for (int i = 0; i < render_vars->screen_width; i++) {
		for (int y = 0; y < render_vars->screen_height / 2; y++) {
			int tex_x = (i * data->texture_sky.width) / render_vars->screen_width;
			int tex_y = (y * data->texture_sky.height) / (render_vars->screen_height / 2);
			int color = data->texture_sky.data[tex_y * data->texture_sky.width + tex_x];
			my_mlx_pixel_put(data, i, y, color);
		}
	}
	for (int i = 0; i < render_vars->screen_width; i++) {
		for (int y = render_vars->screen_height / 2; y < render_vars->screen_height; y++) {
			int tex_x = (i * data->texture_grass.width) / render_vars->screen_width;
			int tex_y = ((y - render_vars->screen_height / 2) * data->texture_grass.height) / (render_vars->screen_height / 2);
			int color = data->texture_grass.data[tex_y * data->texture_grass.width + tex_x];
			my_mlx_pixel_put(data, i, y, color);
		}
	}
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

void	calculate_initial_lateral_distance(t_render_data *r_vars)
{
	if (r_vars->ray_dx < 0)
	{
		r_vars->step_x = -1;
		r_vars->side_dist_x = (r_vars->ray_x - r_vars->map_x)
			* r_vars->delta_dist_x;
	}
	else
	{
		r_vars->step_x = 1;
		r_vars->side_dist_x = (r_vars->map_x + 1.0 - r_vars->ray_x)
			* r_vars->delta_dist_x;
	}
	if (r_vars->ray_dy < 0)
	{
		r_vars->step_y = -1;
		r_vars->side_dist_y = (r_vars->ray_y - r_vars->map_y)
			* r_vars->delta_dist_y;
	}
	else
	{
		r_vars->step_y = 1;
		r_vars->side_dist_y = (r_vars->map_y + 1.0 - r_vars->ray_y)
			* r_vars->delta_dist_y;
	}
}

t_texture	*calculate_the_texture_to_use(t_render_data *r_vars, t_data *data)
{
	t_texture	*current_texture;

	if (r_vars->hit_side == 0)
	{
		if (r_vars->step_x > 0)
			current_texture = &data->texture_east;
		else
			current_texture = &data->texture_west;
	}
	else
	{
		if (r_vars->step_y > 0)
			current_texture = &data->texture_south;
		else
			current_texture = &data->texture_north;
	}
	return (current_texture);
}

void	set_up_3d_render(t_render_data *r_vars, t_data *data)
{
	r_vars->ray_dx = cos(r_vars->ray_angle);
	r_vars->ray_dy = sin(r_vars->ray_angle);
	r_vars->ray_x = data->float_x;
	r_vars->ray_y = data->float_y;
	r_vars->distance = 0.0;
	r_vars->hit_wall = 0;
	r_vars->hit_side = 0;
	r_vars->delta_dist_x = fabs(1.0 / r_vars->ray_dx);
	r_vars->delta_dist_y = fabs(1.0 / r_vars->ray_dy);
	r_vars->map_x = (int)r_vars->ray_x;
	r_vars->map_y = (int)r_vars->ray_y;
}

void	calculate_the_height_line_to_be_drawn_on_screen(t_render_data *r_vars)
{
	r_vars->line_height = 0;
	if (r_vars->hit_wall)
	{
		r_vars->corrected_distance = r_vars->distance
			* cos(r_vars->ray_angle - r_vars->player_angle);
		r_vars->line_height = (int)(r_vars->screen_height
				/ r_vars->corrected_distance);
	}
}
void	calculate_the_exact_coordinate_impact_point(t_render_data *r_vars)
{
	if (r_vars->hit_side == 0)
			r_vars->wall_x = r_vars->ray_y + r_vars->distance * r_vars->ray_dy;
	else
		r_vars->wall_x = r_vars->ray_x + r_vars->distance * r_vars->ray_dx;
	r_vars->wall_x -= floor(r_vars->wall_x);
}

void	render_3d(t_data *data)
{
	t_render_data	render_vars;
	int				x;

	render_vars.screen_width = data->map_width * TILE_SIZE;
	render_vars.screen_height = data->map_height * TILE_SIZE;
	ft_memset(data->img_addr, 0, render_vars.screen_width
		* render_vars.screen_height * (data->bits_per_pixel / 8));
	render_floor_and_sky(&render_vars, data);
	render_vars.fov = 1.0472;
	render_vars.ray_n = render_vars.screen_width;
	render_vars.angle_step = render_vars.fov / render_vars.ray_n;
	render_vars.player_angle = atan2(data->dy, data->dx);
	render_vars.ray_angle = render_vars.player_angle - (render_vars.fov / 2);
	x = 0;
	while (x < render_vars.ray_n)
	{
		set_up_3d_render(&render_vars, data);
		calculate_initial_lateral_distance(&render_vars);
		render_vars.max_distance = 30.0;
		dda(&render_vars, data);
		calculate_the_exact_coordinate_impact_point(&render_vars);
		render_vars.current_texture = calculate_the_texture_to_use(&render_vars, data);
		render_vars.tex_x = (int)(render_vars.wall_x * render_vars.current_texture->width);
		if ((render_vars.hit_side == 0 && render_vars.ray_dx > 0) || (render_vars.hit_side == 1 && render_vars.ray_dy < 0))
			render_vars.tex_x = render_vars.current_texture->width - render_vars.tex_x - 1;
		calculate_the_height_line_to_be_drawn_on_screen(&render_vars);
		render_vars.draw_start = (render_vars.screen_height - render_vars.line_height) / 2;
		if (render_vars.draw_start < 0)
			render_vars.draw_start = 0;
		render_vars.draw_end = (render_vars.screen_height + render_vars.line_height) / 2;
		if (render_vars.draw_end >= render_vars.screen_height)
			render_vars.draw_end = render_vars.screen_height - 1;
		for (int y = render_vars.draw_start; y < render_vars.draw_end; y++) {
			render_vars.tex_y = (int)((y - render_vars.draw_start) * render_vars.current_texture->height / render_vars.line_height);
			render_vars.color = render_vars.current_texture->data[render_vars.tex_y * render_vars.current_texture->width + render_vars.tex_x];
			my_mlx_pixel_put(data, x, y, render_vars.color);
		}
		render_vars.ray_angle += render_vars.angle_step;
		x++;
	}
	mlx_put_image_to_window(data->mlx, data->win, data->img_buffer, 0, 0);
}
