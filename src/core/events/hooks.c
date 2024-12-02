// ************************************************************************** //
//                                                                            //
//                                                        :::      ::::::::   //
//   hooks.c                                            :+:      :+:    :+:   //
//                                                    +:+ +:+         +:+     //
//   By: rgramati <rgramati@student.42angouleme.fr  +#+  +:+       +#+        //
//                                                +#+#+#+#+#+   +#+           //
//   Created: 2024/10/14 00:49:19 by rgramati          #+#    #+#             //
//   Updated: 2024/11/25 16:04:45 by rgramati         ###   ########.fr       //
//                                                                            //
// ************************************************************************** //

#include <termengine.h>

void	te_terminal_hook(t_terminal *t, t_hook_info i, t_hook_func f, void *p)
{
	uint32_t	type;
	uint32_t	key;

	type = i.type;
	key = i.key;
	if (type == TE_KEYBOARD)
	{
		t->hook_table.keyboard_hooks[key] = f;
		t->hook_table.keyboard_params[key] = p;
	}
	if (type == TE_MOUSE)
	{
		t->hook_table.mouse_hooks[key] = f;
		t->hook_table.mouse_params[key] = p;
	}
	if (type == TE_LOOP)
	{
		t->hook_table.hooks[TE_LOOP] = f;
		t->hook_table.params[TE_LOOP] = p;
	}
}
