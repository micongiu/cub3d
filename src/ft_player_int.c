#include "../cub3d.h"

void	find_player_orientation_if_else(t_data *data, int i, int j)
{
	if (data->map[i][j] == 'N')
	{
		data->player_angle = M_PI * 2;
		data->dx = 0;
		data->dy = -1;
	}
	else if (data->map[i][j] == 'S')
	{
		data->player_angle = 3 * (M_PI * 2);
		data->dx = 0;
		data->dy = 1;
	}
	else if (data->map[i][j] == 'E')
	{
		data->player_angle = 0;
		data->dx = 1;
		data->dy = 0;
	}
	else if (data->map[i][j] == 'W')
	{
		data->player_angle = M_PI;
		data->dx = -1;
		data->dy = 0;
	}
}

void	find_player_orientation(t_data *data)
{
	int	i;
	int	j;

	i = 0;
	while (i < data->map_height)
	{
		j = 0;
		while (j < data->map_width)
		{
			if (data->map[i][j] == 'N' || data->map[i][j] == 'S' ||
				data->map[i][j] == 'E' || data->map[i][j] == 'W')
			{
				data->x_player = j;
				data->y_player = i;
				find_player_orientation_if_else(data, i, j);
				data->map[i][j] = '0';
				return ;
			}
			j++;
		}
		i++;
	}
	ft_error("Player starting position not found");
}
