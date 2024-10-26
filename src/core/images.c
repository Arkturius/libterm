// ************************************************************************** //
//                                                                            //
//                                                        :::      ::::::::   //
//   images.c                                           :+:      :+:    :+:   //
//                                                    +:+ +:+         +:+     //
//   By: rgramati <rgramati@student.42angouleme.fr  +#+  +:+       +#+        //
//                                                +#+#+#+#+#+   +#+           //
//   Created: 2024/10/03 17:50:24 by rgramati          #+#    #+#             //
//   Updated: 2024/10/25 21:33:14 by rgramati         ###   ########.fr       //
//                                                                            //
// ************************************************************************** //

#include <stdint.h>
#include <stdlib.h>
#include <termengine.h>

void	te_img_init(t_te_img *img, uint32_t row, uint32_t col, uint32_t *data)
{
	while (1)
	{
		if (!img)
			break ;
		img->row = row;
		img->col = col;
		img->x = 0;
		img->y = 0;
		img->data = data;
		break ;
	}
}

void	te_img_destroy(t_te_img *img)
{
	if (img)
	{
		img->col = 0;
		img->row = 0;
		free(img->data);
	}
}

void	te_img_set_pixel(t_te_img *img, t_vec2 pos, uint32_t color)
{
	const uint32_t	index = (img->row * pos.y) + pos.x;

	img->data[index] = color;
}

void	te_img_fill(t_te_img *img, uint32_t col)
{
	uint32_t	x;
	uint32_t	y;

	x = 0;
	while (x < TE_H - 1)
	{
		y = 0;
		while (y < TE_W)
		{
			te_img_set_pixel(img, (t_vec2){x, y}, col);
			y++;
		}
		x++;
	}
}

// void	te_img_patch_desc(t_te_img *img, t_te_desc *desc)
// {
// 	uint16_t	col;
// 	uint16_t	row;
//
// 	row = 0;
// 	while (row < desc->rows)
// 	{
// 		col = 0;
// 		while (col < desc->cols)
// 		{
// 			te_img_put_pixel(img, col, row, *desc->data[0]);	
// 			col++;
// 		}
// 		row++;
// 	}
// }
