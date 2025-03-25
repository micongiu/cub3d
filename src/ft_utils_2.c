#include "../cub3d.h"

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

