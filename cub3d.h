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

typedef struct s_var_count
{
	int			i;
	int			j;
	int			k;
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

	// Texture
	void	*tex_north;
	void	*tex_south;
	void	*tex_east;
	void	*tex_west;
	int		*tex_north_data;
	int		*tex_south_data;
	int		*tex_east_data;
	int		*tex_west_data;
	int		tex_width;
	int		tex_height;
}	t_data;


// utils
void find_player(t_data *data);
void calculate_map_dimensions(t_data *data);
int ft_close(t_data *data);
int handle_keypress(int keycode, t_data *data);
void draw_square(t_data *data, int x, int y, int color);
// Enums


//Init
void move_p(t_data *data, char direc);
void	ft_init_data(t_data *data, char *argv);


// main.c

void	free_matrix(void **matrix);
void	ft_error(char *str);
char	**open_file(char *file_read);
int		main();

#endif
