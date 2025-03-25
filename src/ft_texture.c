#include "../cub3d.h"

void	convert_texture_data_loop(t_texture *texture)
{
	t_var_count	var;
	int			pixel;
	char		*ptr;

	var.y = 0;
	while (var.y < texture->height)
	{
		var.x = 0;
		while (var.x < texture->width)
		{
			pixel = y * texture->line_length + x * 4;
			ptr = addr + pixel;
			texture->data[y * texture->width + x]
				= ((unsigned char)ptr[2] << 16)
				+ ((unsigned char)ptr[1] << 8)
				+ ((unsigned char)ptr[0]);
			var.x++;
		}
		var.y++;
	}
}

// Funzione per convertire i dati della texture in un array di interi
void	convert_texture_data(t_data *data, t_texture *texture)
{
	void		*addr;

	(void)data;
	texture->data = (int *)ft_calloc(sizeof(int),
			texture->width * texture->height);
	if (!texture->data)
		ft_error("no texture");
	addr = mlx_get_data_addr(texture->img, &texture->bits_per_pixel,
			&texture->line_length, &texture->endian);
	convert_texture_data_loop(texture);
}
