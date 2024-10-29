// ************************************************************************** //
//                                                                            //
//                                                        :::      ::::::::   //
//   tileset.c                                          :+:      :+:    :+:   //
//                                                    +:+ +:+         +:+     //
//   By: rgramati <rgramati@student.42angouleme.fr  +#+  +:+       +#+        //
//                                                +#+#+#+#+#+   +#+           //
//   Created: 2024/10/23 00:56:32 by rgramati          #+#    #+#             //
//   Updated: 2024/10/27 22:11:38 by rgramati         ###   ########.fr       //
//                                                                            //
// ************************************************************************** //

#include <stdint.h>
#include <stdlib.h>
#include <stdarg.h>

#include <termengine.h>

static void	te_tileset_split_one(t_cm_chunk *chunk, t_te_img *img, uint32_t res)
{
	uint32_t	x;
	uint32_t	y;
	uint32_t	index;
	t_te_img	new;
	uint32_t	*ptr;

	y = 0;
	ptr = malloc(res * res * sizeof(uint32_t));
	if (ptr)
	{
		te_img_init(&new, res, res, ptr);
		cm_chunk_push(chunk, &new, sizeof(new));
		while (y < res)
		{
			x = 0;
			while (x < res)
			{
				index = (img->y + y) * img->col + img->x + x;
				new.data[y * res + x] = img->data[index];
				x++;
			}
			y++;
		}
	}
}

static void	te_tileset_split(t_cm_chunk *chunk, t_te_img *img, uint32_t res)
{
	const t_te_img	tmp = *img;

	while (img->row - img->y >= res)
	{
		img->x = tmp.x;
		while (img->col - img->x >= res)
		{
			te_tileset_split_one(chunk, img, res);
			img->x += res;
		}
		img->y += res;
	}
	img->x = tmp.x;
	img->y = tmp.y;
}

t_te_tile_img	*te_tileset_img_init(t_terminal *t, const char *tileset, t_vec2 size, ...)
{
	t_vec2			coords;
	uint32_t		index;
	t_te_tile_img	*img;
	t_te_tileset	*set;
	va_list			tiles;

	img = cm_chunk_alloc(t->tile_images);
	if (img)
	{
		cm_memset(img->indices, 255, 244 * sizeof(uint32_t));
		set = cm_htable_get(t->htilesets, tileset);
		if (!set)
			return (NULL);
		img->res = set->res;
		va_start(tiles, size);
		coords.y = -1;
		while (++coords.y < size.y)
		{
			coords.x = -1;
			while (++coords.x < size.x)
			{
				index = va_arg(tiles, uint32_t);
				img->indices[coords.y * size.y + coords.x] = set->tiles[index];
			}
		}
		va_end(tiles);
		img->col = size.x;
		img->row = size.y;
	}
	return (img);
}

void	te_screen_put_tile_img(t_terminal *t, t_te_tile_img *img, t_vec2 pos)
{
	uint32_t	row;
	uint32_t	col;
	t_te_img	*img_ptr;

	row = 0;
	while (row < img->row)
	{
		col = 0;
		while (col < img->col)
		{
			if (img->indices[row * img->col + col] != (uint32_t)-1)
			{
				img_ptr = cm_chunk_at(t->images, img->indices[row * img->col + col]);
				if (img_ptr)
					te_screen_put_img(t, img_ptr, (t_vec2){pos.x + img->res * col, pos.y + img->res * row});
			}
			col++;
		}
		row++;
	}
}

void	te_tileset_init(t_terminal *t, const char *name, const char *filename, uint32_t res)
{
	t_te_tileset	set;
	t_te_img		img;
	uint32_t		size;
	uint32_t		index[2];
	void			*ptr;

	img = (t_te_img){0};
	set = (t_te_tileset){0};
	if (cm_strlen(name) > 15)
		return ;
	size = cm_bmp(filename, &img.data, CM_OPEN_LOAD);
	img.col = size >> 16;
	img.row = size & 0xFFFF;
	index[0] = cm_chunk_size(t->images);
	te_tileset_split(t->images, &img, res);
	index[1] = cm_chunk_size(t->images);
	while (index[0] < index[1])
		set.tiles[set.tile_count++] = index[0]++;
	set.res = res;
	cm_memcpy(&set.name, (void *)name, 16);
	te_img_destroy(&img);
	ptr = cm_chunk_push(t->tilesets, &set, sizeof(set));
	cm_htable_set(t->htilesets, set.name, ptr);
}
