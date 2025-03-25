#include "../cub3d.h"

void	free_matrix(void **matrix)
{
	size_t	i;

	i = 0;
	if (matrix == NULL)
		return ;
	while (matrix[i])
	{
		free(matrix[i]);
		matrix[i] = NULL;
		i++;
	}
	free(matrix);
	matrix = NULL;
}

void	ft_error(char *str)
{
	printf("%s\n", str);
	exit(0);
}

int	main(int argc, char **argv)
{
	t_data	*data;

	if (argc != 2)
		ft_error("Use./cub3d test_map/*.ber");
	if (ft_strnstr(argv[1], ".ber", ft_strlen_lib(argv[1])) == NULL)
		ft_error("Use only .ber file");
	data = ft_calloc(sizeof(t_data), 1);
	ft_init_data(data, argv[1]);
	return (0);
}
