// ************************************************************************** //
//                                                                            //
//                                                        :::      ::::::::   //
//   etile.c                                            :+:      :+:    :+:   //
//                                                    +:+ +:+         +:+     //
//   By: rgramati <rgramati@student.42angouleme.fr  +#+  +:+       +#+        //
//                                                +#+#+#+#+#+   +#+           //
//   Created: 2024/10/23 23:26:03 by rgramati          #+#    #+#             //
//   Updated: 2024/10/26 02:01:35 by rgramati         ###   ########.fr       //
//                                                                            //
// ************************************************************************** //

#include <stdint.h>
#include <stdio.h>
#include <termengine.h>
#include <unistd.h>

#include "te_tool_etile.h"

void	te_tool_etile_handle_keys(t_te_etile *etile)
{
	TE_HOOK(etile->tool.t, TE_ESCAPE | 'A', TE_ETILE_KEY_UP, etile);
	TE_HOOK(etile->tool.t, TE_ESCAPE | 'B', TE_ETILE_KEY_DOWN, etile);
	TE_HOOK(etile->tool.t, TE_ESCAPE | 'C', TE_ETILE_KEY_RIGHT, etile);
	TE_HOOK(etile->tool.t, TE_ESCAPE | 'D', TE_ETILE_KEY_LEFT, etile);
	TE_HOOK(etile->tool.t, TE_KEYDOWN | ' ', TE_ETILE_SWITCH, etile);
	TE_HOOK(etile->tool.t, TE_KEYDOWN | 's', TE_ETILE_SELECT, etile);
	TE_HOOK(etile->tool.t, TE_KEYDOWN | 'p', TE_ETILE_PLACE, etile);
	TE_HOOK(etile->tool.t, TE_KEYDOWN | 'd', TE_ETILE_DELETE, etile);
	etile->tool.flags &= ~TE_ETILE_MODE_SWITCH;
}

void	te_tool_etile_menu(t_te_etile *e)
{
	const uint32_t	res = e->set->res;
	const t_vec2	s_start = (t_vec2){0, 0};
	const t_vec2	s_end = (t_vec2){2 + e->sgrid_x * (res + 2) - 1,\
									2 + e->sgrid_y * (res + 2) - 1};
	const t_vec2	e_start = (t_vec2){2 + e->sgrid_x * (res + 2) + 1, 0};
	const t_vec2	e_end = (t_vec2){2 + e->grid_x * (res) - 1,\
									2 + e->grid_y * res - 1};

	if (e->tool.flags & TE_ETILE_MODE_SELECT)
	{
		te_screen_draw_square(e->tool.t->screen, e_start, e_end, 0xff303030);
		te_screen_draw_square(e->tool.t->screen, s_start, s_end, 0xff99d9e0);	
	}
	if (e->tool.flags & TE_ETILE_MODE_EDITOR)
	{
		te_screen_draw_square(e->tool.t->screen, s_start, s_end, 0xff303030);
		te_screen_draw_square(e->tool.t->screen, e_start, e_end, 0xff99d9e0);
	}
}

void	te_tool_etile_tiles(t_te_etile *e)
{
	t_vec2		pos;
	t_vec2		tpos;
	t_te_img	*tile;

	pos = (t_vec2){0, 0};
	for (uint32_t i = 0; i < e->set->tile_count; i++)
	{
		if (pos.y == e->sgrid_y)
			break ;
		tile = cm_chunk_at(e->imgs, e->set->tiles[i + e->soff]);
		if (!tile)
			break ;
		tpos.x = 2 + pos.x * (e->set->res + 2);
		tpos.y = 2 + pos.y * (e->set->res + 2);
		te_screen_put_img(e->tool.t->screen, tile, tpos);
		pos.x++;
		pos.y += !(pos.x & 1);
		pos.x &= 1;
	}
}

void	te_tool_etile_select(t_te_etile *e, uint32_t color)
{
	const uint32_t	res = e->set->res;
	const t_vec2	b_start = (t_vec2){1 + e->select_x * (res + 2),\
									1 + e->select_y * (res + 2)};
	const t_vec2	b_end = (t_vec2){res + 1, res + 1};

	te_screen_draw_square(e->tool.t->screen, b_start, b_end, color);
}

void	te_tool_etile_init(t_terminal *t, t_te_etile *e)
{
	cm_memset(e, 0, sizeof(t_te_etile));
	e->tool.t = t;
	e->tool.flags = TE_ETILE_MODE_EDITOR;
	e->tool.flags |= TE_ETILE_MODE_SWITCH;
	e->set = cm_chunk_at(e->tool.t->tilesets, 0);
	e->imgs = e->tool.t->images;
	if (!e->set || !e->imgs || !e->set->res)
		return ;
	e->sgrid_x = 2;
	e->sgrid_y = ((TE_H * 2) - 2) / (e->set->res + 2);
	e->grid_x = (TE_W - 2 - (e->sgrid_x * (e->set->res + 2))) / (e->set->res);
	e->grid_y = ((TE_H * 2) - 2) / (e->set->res);
	e->edit_img.res = e->set->res;
	e->edit_img.col = e->grid_x;
	e->edit_img.row = e->grid_y;
	cm_memset(&e->edit_img.indices, 255, 500 * sizeof(uint32_t));
}

void	*te_tool_etile(void *t_ptr)
{
	static t_te_etile	etile = {0};

	if (!etile.tool.t)
		te_tool_etile_init((t_terminal *)t_ptr, &etile);
	te_tool_etile_handle_keys(&etile);
	te_tool_etile_menu(&etile);
	te_tool_etile_tiles(&etile);
	if (etile.tool.flags & TE_ETILE_MODE_SELECT)
	{
		te_tool_etile_select(&etile, TE_RGB_RED);
		te_screen_draw_square(etile.tool.t->screen, (t_vec2){3 + etile.sgrid_x * (etile.set->res + 2) + etile.editor_x * etile.set->res, etile.editor_y * etile.set->res}, (t_vec2){etile.set->res + 1, etile.set->res + 1}, TE_RGB_WHITE);
	}
	if (etile.tool.flags & TE_ETILE_MODE_EDITOR)
	{
		te_tool_etile_select(&etile, TE_RGB_WHITE);
		te_screen_put_tile_img(etile.tool.t, &etile.edit_img, (t_vec2){etile.sgrid_x * etile.set->res + 2 + 2 + 4, 1});
		te_screen_draw_square(etile.tool.t->screen, (t_vec2){3 + etile.sgrid_x * (etile.set->res + 2) + etile.editor_x * etile.set->res, etile.editor_y * etile.set->res}, (t_vec2){etile.set->res + 1, etile.set->res + 1}, TE_RGB_RED);
	}
	// te_tool_etile_editor(&etile);
	return (NULL);
}
