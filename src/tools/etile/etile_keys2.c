// ************************************************************************** //
//                                                                            //
//                                                        :::      ::::::::   //
//   etile_keys2.c                                      :+:      :+:    :+:   //
//                                                    +:+ +:+         +:+     //
//   By: rgramati <rgramati@student.42angouleme.fr  +#+  +:+       +#+        //
//                                                +#+#+#+#+#+   +#+           //
//   Created: 2024/10/23 23:26:08 by rgramati          #+#    #+#             //
//   Updated: 2024/10/26 19:50:02 by rgramati         ###   ########.fr       //
//                                                                            //
// ************************************************************************** //

#include <stdint.h>
#include <termengine.h>

#include "te_tool_etile.h"

void	*__te_etile_key_place(void *etile_ptr)
{
	t_te_etile	*e;
	uint32_t	index;

	e = (t_te_etile *)etile_ptr;

	if (e->tool.flags & TE_ETILE_MODE_EDITOR)
	{
		index = e->select_y * e->sgrid_x + e->select_x + e->soff;
		e->edit_img.indices[e->editor_y * e->grid_x + e->editor_x] = index;
	}
	return (NULL);
}

void	*__te_etile_key_delete(void *etile_ptr)
{
	t_te_etile	*e;

	e = (t_te_etile *)etile_ptr;

	if (e->tool.flags & TE_ETILE_MODE_EDITOR)
		e->edit_img.indices[e->editor_y * e->grid_x + e->editor_x] = (uint32_t)-1;
	return (NULL);
}

void	*__te_etile_key_select(void *etile_ptr)
{
	(void)etile_ptr;
	return (NULL);
}

void	*__te_etile_key_brush(void *etile_ptr)
{
	t_te_etile	*e;

	e = (t_te_etile *)etile_ptr;
	if (e->tool.flags & TE_ETILE_MODE_EDITOR && !(e->tool.flags & TE_ETILE_MODE_ERASE))
		e->tool.flags ^= TE_ETILE_MODE_BRUSH;
	return (NULL);
}

void	*__te_etile_key_eraser(void *etile_ptr)
{
	t_te_etile	*e;

	e = (t_te_etile *)etile_ptr;
	if (e->tool.flags & TE_ETILE_MODE_EDITOR && !(e->tool.flags & TE_ETILE_MODE_BRUSH))
		e->tool.flags ^= TE_ETILE_MODE_ERASE;
	return (NULL);
}

void	*__te_etile_key_save(void *etile_ptr)
{
	(void)etile_ptr;
	return (NULL);
}

void	*__te_etile_key_load(void *etile_ptr)
{
	(void)etile_ptr;
	return (NULL);
}























