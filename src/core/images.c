// ************************************************************************** //
//                                                                            //
//                                                        :::      ::::::::   //
//   images.c                                           :+:      :+:    :+:   //
//                                                    +:+ +:+         +:+     //
//   By: rgramati <rgramati@student.42angouleme.fr  +#+  +:+       +#+        //
//                                                +#+#+#+#+#+   +#+           //
//   Created: 2024/10/03 17:50:24 by rgramati          #+#    #+#             //
//   Updated: 2024/10/16 22:52:26 by rgramati         ###   ########.fr       //
//                                                                            //
// ************************************************************************** //

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <termengine.h>

t_cm_chunk	*chunk_next(t_cm_chunk *c)
{
	return (((struct s_cm_chunk *)c)->next);
}

t_te_img	*te_img_init(t_terminal *term, uint32_t row, uint32_t col)
{
	t_te_img	*img;
	t_cm_chunk		*chunk;

	chunk = term->imgs;
	while (1)
	{
		while (chunk_next(chunk))
			chunk = chunk_next(chunk);
		img = cm_chunk_alloc(term->imgs);
		if (!img)
			break ;
		if (row > term->row || col > term->col)
			break ;
		img->row = row;
		img->col = col;
		img->x = 0;
		img->y = 0;
		img->data = malloc(row * col * sizeof(uint32_t));
		if (!img->data)
			break ;
		cm_memset(img->data, 0, row * col * sizeof(uint32_t));
		if (((struct s_cm_chunk *)chunk)->iterator.index == ((struct s_cm_chunk *)chunk)->size)
			((struct s_cm_chunk *)chunk)->next = cm_chunk_init(((struct s_cm_chunk *)chunk)->name, ((struct s_cm_chunk *)chunk)->alignment);
		return (img);
	}
	return (NULL);
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

void	te_img_set_pixel(t_te_img *img, uint32_t x, uint32_t y, uint32_t color)
{
	const uint32_t	index = (img->row * y) + x;

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
			te_img_set_pixel(img, col, x, y);
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
