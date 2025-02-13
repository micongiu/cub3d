#include "../cub3d.h"

char	**read_file(char **str_read, int fd, char *file_read)
{
	int		k;
	int		i;
	
	k = 0;
	i = 0;
	while (str_read[0] != NULL)
	{
		free(str_read[0]);
		i++;
		str_read[0] = get_next_line(fd);
	}
	close(fd);
	free_matrix((void **)str_read);
	fd = open(file_read, O_RDONLY);
	str_read = ft_calloc((i + 2), sizeof(char **));
	if (str_read == NULL)
		ft_error("Error in the allocate");
	while (k < i)
	{
		str_read[k] = get_next_line(fd);
		k++;
	}
	return (str_read);
}

char	**open_file(char *file_read)
{
	int		fd;
	char	**str_read;
	char	**str_return;

	fd = open(file_read, O_RDONLY);
	str_read = ft_calloc((1), sizeof(char **));
	if (str_read == NULL)
		ft_error("Error in the allocate");
	str_read[0] = get_next_line(fd);
	if (str_read[0] == NULL)
	{
		printf("Error in the reading\n");
		free_matrix((void **)str_read);
		exit(0);
	}
	str_return = read_file(str_read, fd, file_read);
	return (close(fd), str_return);
}

