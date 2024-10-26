// ************************************************************************** //
//                                                                            //
//                                                        :::      ::::::::   //
//   screen.c                                           :+:      :+:    :+:   //
//                                                    +:+ +:+         +:+     //
//   By: rgramati <rgramati@student.42angouleme.fr  +#+  +:+       +#+        //
//                                                +#+#+#+#+#+   +#+           //
//   Created: 2024/10/05 20:37:07 by rgramati          #+#    #+#             //
//   Updated: 2024/10/25 21:38:44 by rgramati         ###   ########.fr       //
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
	write(STDIN_FILENO, "\033[0;0f", 6);
	write(STDIN_FILENO, t->screen, TE_W * TE_H * TE_BLEN);
	cm_memcpy(t->screen, t->back, TE_W * TE_H * TE_BLEN + 1);
}

void	te_screen_set_pixel(t_screen screen, t_vec2 pos, uint32_t color)
{
	t_vec2		coords;
	uint32_t	index;

	if (pos.x < 0 || pos.y < 0 || pos.x >= TE_W || (pos.y & ~1) >= TE_H * 2)
		return ;
	coords.x = (TE_BLEN * pos.x) + (19 * (pos.y & 1));
	coords.y = (pos.y >> 1);
	index = ((TE_BLEN * TE_W) * coords.y) + coords.x;
	te_str_append_color((pos.y & 1), color, (char *)&screen[index], NULL);
}

void	te_screen_put_img(t_screen screen, t_te_img *img, t_vec2 pos)
{
	t_vec2		coords;
	t_vec2		sum;
	uint32_t	index;
	uint32_t	color;

	coords.y = 0;
	while (coords.y < (int)img->row)
	{
		coords.x = 0;
		while (coords.x < (int)img->col)
		{
			index = coords.y * img->col + coords.x;
			color = img->data[index];
			sum = (t_vec2){coords.x + pos.x, coords.y + pos.y};
			if (color == TE_RGB_TRANSPARENT)
				color = TE_RGB_BLACK;
			else
				color |= 0xFF000000;
			if (color & 0xFF000000)
				te_screen_set_pixel(screen, sum, color);
			coords.x++;
		}
		coords.y++;
	}
}
