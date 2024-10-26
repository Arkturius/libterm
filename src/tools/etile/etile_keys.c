// ************************************************************************** //
//                                                                            //
//                                                        :::      ::::::::   //
//   etile_keys.c                                       :+:      :+:    :+:   //
//                                                    +:+ +:+         +:+     //
//   By: rgramati <rgramati@student.42angouleme.fr  +#+  +:+       +#+        //
//                                                +#+#+#+#+#+   +#+           //
//   Created: 2024/10/23 23:26:06 by rgramati          #+#    #+#             //
//   Updated: 2024/10/25 01:18:51 by rgramati         ###   ########.fr       //
//                                                                            //
// ************************************************************************** //

#include "te_tool_etile.h"
#include <stdio.h>

void	*__te_etile_key_move_up(void *etile_ptr)
{
	t_te_etile	*etile;

	etile = (t_te_etile *)etile_ptr;
	if (etile->tool.flags & TE_ETILE_MODE_SELECT)
	{
		etile->soff -= etile->sgrid_x * ((etile->select_y == 0) && etile->soff);
		etile->select_y -= (etile->select_y > 0);
	}
	if (etile->tool.flags & TE_ETILE_MODE_EDITOR)
		etile->editor_y -= (etile->editor_y > 0);
	return (NULL);
}

void	*__te_etile_key_move_down(void *etile_ptr)
{
	t_te_etile	*etile;

	etile = (t_te_etile *)etile_ptr;
	if (etile->tool.flags & TE_ETILE_MODE_SELECT)
	{
		if (etile->select_y * etile->sgrid_x + etile->select_x + etile->soff < etile->set->tile_count - 2)
			etile->soff += etile->sgrid_x * (etile->select_y == etile->sgrid_y - 1);
		etile->select_y += (etile->select_y < etile->sgrid_y - 1);
	}
	if (etile->tool.flags & TE_ETILE_MODE_EDITOR)
		etile->editor_y += (etile->editor_y < etile->grid_y - 1);
	return (NULL);
}

void	*__te_etile_key_move_right(void *etile_ptr)
{
	t_te_etile	*etile;

	etile = (t_te_etile *)etile_ptr;
	if (etile->tool.flags & TE_ETILE_MODE_SELECT)
		etile->select_x += (etile->select_x < etile->sgrid_x - 1);
	if (etile->tool.flags & TE_ETILE_MODE_EDITOR)
		etile->editor_x += (etile->editor_x < etile->grid_x - 1);
	return (NULL);
}

void	*__te_etile_key_move_left(void *etile_ptr)
{
	t_te_etile	*etile;

	etile = (t_te_etile *)etile_ptr;
	if (etile->tool.flags & TE_ETILE_MODE_SELECT)
		etile->select_x -= (etile->select_x > 0);
	if (etile->tool.flags & TE_ETILE_MODE_EDITOR)
		etile->editor_x -= (etile->editor_x > 0);
	return (NULL);
}

void	*__te_etile_key_switch(void *etile_ptr)
{
	t_te_etile	*etile;

	etile = (t_te_etile *)etile_ptr;
	if (etile->tool.flags == TE_ETILE_MODE_SELECT)
		etile->tool.flags = TE_ETILE_MODE_EDITOR;
	else if (etile->tool.flags == TE_ETILE_MODE_EDITOR)
		etile->tool.flags = TE_ETILE_MODE_SELECT;
	etile->tool.flags |= TE_ETILE_MODE_SWITCH;
	return (NULL);
}



































