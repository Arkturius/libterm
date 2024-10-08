// ************************************************************************** //
//                                                                            //
//                                                        :::      ::::::::   //
//   te_screen.c                                        :+:      :+:    :+:   //
//                                                    +:+ +:+         +:+     //
//   By: rgramati <rgramati@student.42angouleme.fr  +#+  +:+       +#+        //
//                                                +#+#+#+#+#+   +#+           //
//   Created: 2024/10/05 20:37:07 by rgramati          #+#    #+#             //
//   Updated: 2024/10/07 21:22:57 by rgramati         ###   ########.fr       //
//                                                                            //
// ************************************************************************** //

#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>

#include <termengine.h>

t_screen	te_screen_init(void)
{
	t_screen	screen;
	int32_t		fd;

	screen = malloc(TR_W * TR_H * TR_BLEN + 1);
	if (screen)
	{
		fd = open("conf/blank.tes", O_RDONLY);
		if (fd == -1)
		{
			free(screen);
			return (NULL);
		}
		read(fd, screen, TR_W * TR_H * TR_BLEN + 1);
		close(fd);
	}
	screen[TR_W * TR_H * TR_BLEN] = 0;
	return (screen);
}

void	te_screen_destroy(t_screen screen)
{
	free(screen);
}

void	te_terminal_screen_shift(t_terminal *t)
{
	ft_memcopy(t->screen, t->back, TR_W * TR_H * TR_BLEN + 1);
	// TODO : Get each entity's state, and for each one on screen, display the current animation frame, depending on anim_state
	printf("\033[0;0f%s", (char *)t->screen);
	// fflush(stdout);
}

void	te_screen_set_pixel(t_screen screen, uint32_t x, uint32_t y, uint32_t color)
{
	uint32_t	index;

	if (x < 0 || y < 0 || x >= TR_W || (y & ~1) >= TR_H * 2)
		return ;
	// dprintf(2, "Setting color to %u %u -> %x\n", x, y, color);
	index = ((TR_BLEN * TR_W) * (y >> 1)) + (TR_BLEN * x) + (19 * (y & 1));
	te_str_append_color((y & 1), color, (char *)&screen[index], NULL);
}

void	te_screen_put_img(t_screen screen, t_te_img *img, uint32_t x, uint32_t y)
{
	uint32_t	row;
	uint32_t	col;
	uint32_t	index;
	uint32_t	color;

	row = 0;
	while (row < img->row)
	{
		col = 0;
		while (col < img->col)
		{
			index = row * img->row + col;
			color = img->data[index];
			te_screen_set_pixel(screen, col + x, row + y, color);
			col++;
		}
		row++;
	}
}
