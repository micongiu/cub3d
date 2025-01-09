#ifndef CUB3D_H
# define CUB3D_H

# include "libft/libft.h"
# include "gnl/get_next_line.h"
# include "minilibx-linux/mlx.h"
# include <stdlib.h>
# include <unistd.h>
# include <stdio.h>
#include <string.h>

#define WIDTH 800
#define HEIGHT 600
#define TILE_SIZE 32

// Structures

typedef struct s_var_count
{
	int			i;
	int			j;
	int			k;
}	t_var_count;

typedef struct s_data {
	void	*mlx;
	void	*win;
	void	*img;
	char	**map;
	int		map_width;
	int		map_height;
} t_data;

// Enums


//Init

t_data *ft_init_data(char **map);


// main.c

void	free_matrix(void **matrix);
void	ft_error(char *str);
char	**open_file(char *file_read);
int		main();

#endif
