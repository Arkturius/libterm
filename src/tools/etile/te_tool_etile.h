// ************************************************************************** //
//                                                                            //
//                                                        :::      ::::::::   //
//   te_tool_etile.h                                    :+:      :+:    :+:   //
//                                                    +:+ +:+         +:+     //
//   By: rgramati <rgramati@student.42angouleme.fr  +#+  +:+       +#+        //
//                                                +#+#+#+#+#+   +#+           //
//   Created: 2024/10/23 23:27:07 by rgramati          #+#    #+#             //
//   Updated: 2024/10/26 19:50:23 by rgramati         ###   ########.fr       //
//                                                                            //
// ************************************************************************** //

#ifndef TE_TOOL_ETILE_H
# define TE_TOOL_ETILE_H

#include <stdint.h>
# include <termengine.h>

# include "../te_tools.h"

typedef struct s_te_etile
{
	t_tool_data		tool;
	uint16_t		select_x;
	uint16_t		select_y;
	uint16_t		editor_x;
	uint16_t		editor_y;
	uint16_t		sgrid_x;
	uint16_t		sgrid_y;
	uint16_t		grid_x;
	uint16_t		grid_y;
	t_te_tileset	*set;
	t_cm_chunk		*imgs;
	t_te_tile_img	edit_img;
	uint32_t		soff;
}	t_te_etile;

#define TE_ETILE_KEY_UP		__te_etile_key_move_up
#define TE_ETILE_KEY_DOWN	__te_etile_key_move_down
#define TE_ETILE_KEY_RIGHT	__te_etile_key_move_right
#define TE_ETILE_KEY_LEFT	__te_etile_key_move_left
#define TE_ETILE_SWITCH		__te_etile_key_switch

#define TE_ETILE_PLACE		__te_etile_key_place
#define TE_ETILE_DELETE		__te_etile_key_delete
#define TE_ETILE_BRUSH		__te_etile_key_brush
#define TE_ETILE_ERASER		__te_etile_key_eraser
#define TE_ETILE_SELECT		__te_etile_key_select

#define TE_ETILE_SAVE		__te_etile_key_save
#define TE_ETILE_LOAD		__te_etile_key_load

void
*__te_etile_key_move_up(void *etile_ptr);

void
*__te_etile_key_move_down(void *etile_ptr);

void
*__te_etile_key_move_right(void *etile_ptr);

void
*__te_etile_key_move_left(void *etile_ptr);

void
*__te_etile_key_switch(void *etile_ptr);

void
*__te_etile_key_place(void *etile_ptr);

void
*__te_etile_key_delete(void *etile_ptr);

void
*__te_etile_key_brush(void *etile_ptr);

void
*__te_etile_key_eraser(void *etile_ptr);

void
*__te_etile_key_select(void *etile_ptr);

void
*__te_etile_key_save(void *etile_ptr);

void
*__te_etile_key_load(void *etile_ptr);

#endif	// TE_TOOL_ETILE_H
