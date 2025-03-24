#ifndef CUB3D_H
# define CUB3D_H

# include "libft/libft.h"
# include "gnl/get_next_line.h"
# include "minilibx-linux/mlx.h"
# include <stdlib.h>
# include <unistd.h>
# include <stdio.h>
#include <string.h>
#include <math.h>

#define WIDTH 800
#define HEIGHT 600
#define TILE_SIZE 64

// Structures

typedef struct s_texture {
	void	*img;
	int		*data;
	int		width;
	int		height;
	int		bits_per_pixel;
	int		line_length;
	int		endian;
}	t_texture;

typedef struct s_render_data {
	int			screen_width;
	int			screen_height;
	float		fov;
	int			ray_n;
	float		angle_step;
	float		player_angle;
	float		ray_angle;
	float		ray_dx;
	float		ray_dy;
	float		ray_x;
	float		ray_y;
	float		distance;
	int			hit_wall;
	int			hit_side;
	float		delta_dist_x;
	float		delta_dist_y;
	int			map_x;
	int			map_y;
	float		side_dist_x;
	float		side_dist_y;
	int			step_x;
	int			step_y;
	float		max_distance;
	float		corrected_distance;
	int			line_height;
	float		wall_x;
	t_texture	*current_texture;
	int			tex_x;
	int			draw_start;
	int			draw_end;
	int			tex_y;
	int			color;
	float		shade;
	int			r;
	int			g;
	int			b;
}	t_render_data;

typedef struct s_var_count
{
	int	i;
	int	j;
	int	k;
}	t_var_count;

typedef struct s_data
{
	void		*mlx;
	void		*win;
	void		*img;
	char		**map;
	int			map_width;
	int			map_height;
	int			x_player;
	int			y_player;
	float		float_x;
	float		float_y;
	float		player_angle;
	float		dx;
	float		dy;
	int			bits_per_pixel;
	int			line_length;
	int			endian;
	char		*img_addr;
	void		*img_buffer;
	t_texture	texture_north;
	t_texture	texture_south;
	t_texture	texture_east;
	t_texture	texture_west;
	t_texture texture_sky;
	t_texture texture_grass;
}	t_data;


// utils
void	find_player(t_data *data);
void	calculate_map_dimensions(t_data *data);
int		ft_close(t_data *data);
int		handle_keypress(int keycode, t_data *data);
void	draw_square(t_data *data, int x, int y, int color);

//Init
void	move_p(t_data *data, char direc);
void	ft_init_data(t_data *data, char *argv);


void	render_3d(t_data *data);
void 	my_mlx_pixel_put(t_data *data, int x, int y, int color);

// main.c

void	free_matrix(void **matrix);
void	ft_error(char *str);
char	**open_file(char *file_read);
int		main();

#endif
