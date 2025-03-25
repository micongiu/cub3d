#include "../cub3d.h"

void	render_floor_and_sky(t_render_data *render_vars, t_data *data)
{
	t_var_count var_count;
	int	tex_x;
	int	tex_y;
	int	color;

	var_count.i = 0;
	while (var_count.i < render_vars->screen_width)
	{
		var_count.j = 0;
		while (var_count.j < render_vars->screen_height / 2)
		{
			tex_x = (var_count.i * data->texture_sky.width) / render_vars->screen_width;
			tex_y = (var_count.j * data->texture_sky.height) / (render_vars->screen_height / 2);
			if (tex_x >= 0 && tex_x < data->texture_sky.width &&
				tex_y >= 0 && tex_y < data->texture_sky.height)
			{
				color = data->texture_sky.data[tex_y * data->texture_sky.width + tex_x];
				my_mlx_pixel_put(data, var_count.i, var_count.j, color);
			}
			var_count.j++;
		}
		var_count.i++;
	}
	var_count.i = 0;
	while (var_count.i < render_vars->screen_width)
	{
		var_count.j = render_vars->screen_height / 2;
		while (var_count.j < render_vars->screen_height)
		{
			tex_x = (var_count.i * data->texture_grass.width) / render_vars->screen_width;
			tex_y = ((var_count.j - render_vars->screen_height / 2) * data->texture_grass.height) / (render_vars->screen_height / 2);
			if (tex_x >= 0 && tex_x < data->texture_grass.width &&
				tex_y >= 0 && tex_y < data->texture_grass.height)
			{
				color = data->texture_grass.data[tex_y * data->texture_grass.width + tex_x];
				my_mlx_pixel_put(data, var_count.i, var_count.j, color);
			}
			var_count.j++;
		}
		var_count.i++;
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

static void	render_wall_slice(t_render_data *r_vars, t_data *data, int x)
{
	int	y;

	y = r_vars->draw_start;
	while (y < r_vars->draw_end)
	{
		r_vars->tex_y = (int)((y - r_vars->draw_start) *
			r_vars->current_texture->height / r_vars->line_height);
		r_vars->color = r_vars->current_texture->data[
			r_vars->tex_y * r_vars->current_texture->width + r_vars->tex_x];
		my_mlx_pixel_put(data, x, y, r_vars->color);
		y++;
	}
}

static void	calculate_wall_dimensions(t_render_data *r_vars)
{
	r_vars->draw_start = (r_vars->screen_height - r_vars->line_height) / 2;
	if (r_vars->draw_start < 0)
		r_vars->draw_start = 0;
	r_vars->draw_end = (r_vars->screen_height + r_vars->line_height) / 2;
	if (r_vars->draw_end >= r_vars->screen_height)
		r_vars->draw_end = r_vars->screen_height - 1;
}

static void	process_ray(t_render_data *r_vars, t_data *data, int x)
{
	set_up_3d_render(r_vars, data);
	calculate_initial_lateral_distance(r_vars);
	r_vars->max_distance = 30.0;
	dda(r_vars, data);
	calculate_the_exact_coordinate_impact_point(r_vars);
	r_vars->current_texture = calculate_the_texture_to_use(r_vars, data);
	r_vars->tex_x = (int)(r_vars->wall_x * r_vars->current_texture->width);
	if ((r_vars->hit_side == 0 && r_vars->ray_dx > 0)
		|| (r_vars->hit_side == 1 && r_vars->ray_dy < 0))
		r_vars->tex_x = r_vars->current_texture->width - r_vars->tex_x - 1;
	calculate_the_height_line_to_be_drawn_on_screen(r_vars);
	calculate_wall_dimensions(r_vars);
	render_wall_slice(r_vars, data, x);
}

void	render_3d(t_data *data)
{
	t_render_data	r_vars;
	int				x;

	r_vars.screen_width = data->map_width * TILE_SIZE;
	r_vars.screen_height = data->map_height * TILE_SIZE;
	ft_memset(data->img_addr, 0, r_vars.screen_width *
		r_vars.screen_height * (data->bits_per_pixel / 8));
	render_floor_and_sky(&r_vars, data);
	r_vars.fov = 1.0472;
	r_vars.ray_n = r_vars.screen_width;
	r_vars.angle_step = r_vars.fov / r_vars.ray_n;
	r_vars.player_angle = atan2(data->dy, data->dx);
	r_vars.ray_angle = r_vars.player_angle - (r_vars.fov / 2);
	x = 0;
	while (x < r_vars.ray_n)
	{
		process_ray(&r_vars, data, x);
		r_vars.ray_angle += r_vars.angle_step;
		x++;
	}
	mlx_put_image_to_window(data->mlx, data->win, data->img_buffer, 0, 0);
}
