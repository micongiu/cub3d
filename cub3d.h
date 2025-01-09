#ifndef CUB3D_H
# define CUB3D_H

# include "libft/libft.h"
# include "gnl/get_next_line.h"
# include <stdlib.h>
# include <unistd.h>
# include <stdio.h>

// Structures

typedef struct s_var_count
{
	int			i;
	int			j;
	int			k;
}	t_var_count;

// Enums


// main.c

void	free_matrix(void **matrix);
void	ft_error(char *str);
char	**open_file(char *file_read);
int		main();

#endif
