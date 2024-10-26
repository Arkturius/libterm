// ************************************************************************** //
//                                                                            //
//                                                        :::      ::::::::   //
//   draw.c                                             :+:      :+:    :+:   //
//                                                    +:+ +:+         +:+     //
//   By: rgramati <rgramati@student.42angouleme.fr  +#+  +:+       +#+        //
//                                                +#+#+#+#+#+   +#+           //
//   Created: 2024/10/24 20:52:28 by rgramati          #+#    #+#             //
//   Updated: 2024/10/25 21:32:26 by rgramati         ###   ########.fr       //
//                                                                            //
// ************************************************************************** //

#include <stdint.h>
#include <termengine.h>

void	te_screen_draw_square(t_screen screen, t_vec2 start, t_vec2 size, uint32_t color)
{
	const t_vec2	end = (t_vec2){start.x + size.x, start.y + size.y};

	te_screen_draw_line(screen, start, (t_vec2){end.x, start.y}, color);
	te_screen_draw_line(screen, (t_vec2){end.x, start.y}, end, color);
	te_screen_draw_line(screen, end, (t_vec2){start.x, end.y}, color);
	te_screen_draw_line(screen, (t_vec2){start.x, end.y}, (t_vec2)start, color);
}

static int32_t	te_abs(int32_t n)
{
	if (n < 0)
		return (-n);
	return (n);
}

void	te_screen_draw_line(t_screen screen, t_vec2 start, t_vec2 end, uint32_t color)
{
	t_vec2		diff;
	t_fvec2		deltas;
	t_fvec2		inc;
	float		max;

	diff = (t_vec2){.x = end.x - start.x, .y = end.y - start.y};
	max = (float)te_abs((int32_t[2]){diff.x, diff.y}[te_abs(diff.x) < te_abs(diff.y)]);

	deltas = (t_fvec2){diff.x / max, diff.y / max};
	inc = (t_fvec2){start.x, start.y};
	while (inc.x != end.x || inc.y != end.y)
	{
		te_screen_set_pixel(screen, (t_vec2){inc.x, inc.y}, color);
		inc.x += deltas.x;
		inc.y += deltas.y;
	}
}
