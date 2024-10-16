// ************************************************************************** //
//                                                                            //
//                                                        :::      ::::::::   //
//   screen.c                                           :+:      :+:    :+:   //
//                                                    +:+ +:+         +:+     //
//   By: rgramati <rgramati@student.42angouleme.fr  +#+  +:+       +#+        //
//                                                +#+#+#+#+#+   +#+           //
//   Created: 2024/10/05 20:37:07 by rgramati          #+#    #+#             //
//   Updated: 2024/10/16 22:52:32 by rgramati         ###   ########.fr       //
//                                                                            //
// ************************************************************************** //

#include <fcntl.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#include <termengine.h>
#include <unistd.h>

t_screen	te_screen_init(void)
{
	t_screen	screen;
	char		*cursor;
	uint32_t	pixel;

	screen = malloc(TE_W * TE_H * TE_BLEN + 1);
	if (screen)
	{
		pixel = 0;
		cursor = (char *)screen;
		while (pixel < TE_W * TE_H)
		{
			te_str_append_color(0, 0, cursor, &cursor);
			te_str_append_color(1, 0, cursor, &cursor);
			te_str_append(TE_BLOCK, cursor, &cursor);
			pixel++;
		}
		screen[TE_W * TE_H * TE_BLEN] = 0;
	}
	return (screen);
}

void	te_screen_destroy(t_screen screen)
{
	free(screen);
}

void	te_terminal_screen_shift(t_terminal *t)
{
	cm_memcpy(t->screen, t->back, TE_W * TE_H * TE_BLEN + 1);
	write(STDIN_FILENO, "\033[0;0f", 6);
	write(STDIN_FILENO, t->screen, TE_W * TE_H * TE_BLEN);
}

void	te_screen_set_pixel(t_screen screen, uint32_t x, uint32_t y, uint32_t color)
{
	uint32_t	index;

	if (x < 0 || y < 0 || x >= TE_W || (y & ~1) >= TE_H * 2)
		return ;
	index = ((TE_BLEN * TE_W) * (y >> 1)) + (TE_BLEN * x) + (19 * (y & 1));
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
			index = row * img->col + col;
			color = img->data[index];
			if (color == TE_RGB_TRANSPARENT)
				color = TE_RGB_BLACK;
			if (color & 0xFF000000)
				te_screen_set_pixel(screen, col + x, row + y, color);
			col++;
		}
		row++;
	}
}
