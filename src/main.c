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

int main(int argc, char **argv)
{
	char	**map;

	if (argc != 2)
		return (-1);
	if (ft_strnstr(argv[1], ".ber", ft_strlen_lib(argv[1])) == NULL)
		ft_error("Use only .ber file");
	map = open_file(argv[1]);
	for (int i = 0; map[i]; i++)
		printf("%s", map[i]);
	free_matrix((void **)map);
}
