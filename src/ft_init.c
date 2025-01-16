#include "../cub3d.h"

void draw_line(t_data *data, int start_x, int start_y,int dx, int dy, int color)
{
	while(dx < 64)
	{
		int pixel_x = start_x * TILE_SIZE + TILE_SIZE / 2;
		int pixel_y = start_y * TILE_SIZE + TILE_SIZE / 2;
		int map_x;
		int map_y;

		while (pixel_y >= 0 && pixel_y < data->map_height * TILE_SIZE)
		{
			map_x = pixel_x / TILE_SIZE;
			map_y = pixel_y / TILE_SIZE;

			if (data->map[map_y][map_x] == '1')
				break;
			mlx_pixel_put(data->mlx, data->win, pixel_x, pixel_y, color);
			pixel_y += dy;
			pixel_x += dx;
		}
		dx++;
	}
}


void draw_square(t_data *data, int x, int y, int color)
{
	for (int i = 0; i < TILE_SIZE; i++)
	{
		for (int j = 0; j < TILE_SIZE; j++)
			mlx_pixel_put(data->mlx, data->win, x * TILE_SIZE + j, y * TILE_SIZE + i, color);
	}
}

void render_map(t_data *data)
{
	for (int y = 0; data->map[y] != NULL; y++)
	{
		int row_length = ft_strlen_lib(data->map[y]);
		for (int x = 0; x < row_length; x++)
		{
			if (data->map[y][x] == '1')
				draw_square(data, x, y, 0xFF0000);
			else if (data->map[y][x] == '0')
				draw_square(data, x, y, 0x00FF00);
			else if (data->map[y][x] == 'P')
				draw_square(data, x, y, 0x0000FF);
		}
	}
}

void move_p(t_data *data, char direc)
{
	int new_x = data->x_player;
	int new_y = data->y_player;

	if(direc == 'w')
		new_y--;
	if(direc == 's')
		new_y++;
	if(direc == 'a')
		new_x--;
	if(direc == 'd')
		new_x++;
	if(new_x > 0 && new_y > 0 && new_y < data->map_height && new_x < (data->map_width - 1))
	{
		if(data->map[new_y][new_x] != '1')
		{
			data->map[data->y_player][data->x_player] = '0';
			draw_square(data, data->x_player, data->y_player, 0x00FF00);
				draw_line(data, data->x_player, data->y_player,-1, -1, 0x00FF00);
			data->x_player = new_x;
			data->y_player = new_y;
			data->map[data->y_player][data->x_player] = 'P';
			draw_square(data, data->x_player, data->y_player, 0x0000FF);
			draw_line(data, data->x_player, data->y_player,-1, -1, 0x800080);

		}
		else
			printf("Wall!!!\n");
	}
	else
		printf("Wall!!!\n");
}


void find_player(t_data *data)
{
	int i = 0;
	int j = 0;
	while(data->map[i] != NULL)
	{
		j = 0;
		while(data->map[i][j] != '\0')
		{
			if(data->map[i][j] == 'P')
			{
				data->x_player = j;
				data->y_player = i;
				return;
			}
			j++;
		}
		i++;
	}
	printf("no player\n");
	free_matrix((void **)data->map);
	free(data);
	exit(0);
}

void calculate_map_dimensions(t_data *data) {
	int row_length;
	data->map_width = 0;
	data->map_height = 0;

	row_length = 0;
	for	(int y = 0; data->map[y] != NULL; y++) {
		row_length = ft_strlen_lib(data->map[y]);
		if (row_length > data->map_width)
			data->map_width = row_length;
		data->map_height++;
	}
	data->map_width--;
	find_player(data);
	printf("x_p = %i\n", data->x_player);
	printf("y_p = %i\n", data->y_player);
}

int	ft_close(t_data *data)
{
	printf("ESC pressed. Exiting...\n");
	// mlx_destroy_image(data->mlx, data->img);
	mlx_destroy_window(data->mlx, data->win);
	free(data->mlx);
	free_matrix((void **)data->map);
	free(data);
	exit(0);
}

int handle_keypress(int keycode, t_data *data)
{
	if (keycode == 65307)
		ft_close(data);
	if(keycode == 119)
		move_p(data,'w');
	else if(keycode == 115)
		move_p(data,'s');
	else if(keycode == 97)
		move_p(data,'a');
	else if(keycode == 100)
		move_p(data,'d');
	return (0);
}

void	ft_init_data(t_data *data, char *argv)
{
	data = ft_calloc(sizeof(t_data), 1);
	data->map = open_file(argv);
	for (int i = 0; data->map[i]; i++)
		printf("%s", data->map[i]);
	printf("\nmap[0][0] = %c\n", data->map[0][0]);
	calculate_map_dimensions(data);
	printf("width = %i\n", data->map_width);
	printf("width = %i\n", data->map_height);
	data->mlx = mlx_init();
	data->win = mlx_new_window(data->mlx, data->map_width * TILE_SIZE, data->map_height * TILE_SIZE, "Cub3D Map");
	render_map(data);
	mlx_hook(data->win, 17, 0, (int (*)())ft_close, data);
	mlx_hook(data->win, 2, 1L << 0, (int (*)())handle_keypress, data);
	mlx_loop(data->mlx);
}
