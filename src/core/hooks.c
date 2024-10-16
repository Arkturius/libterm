// ************************************************************************** //
//                                                                            //
//                                                        :::      ::::::::   //
//   hooks.c                                            :+:      :+:    :+:   //
//                                                    +:+ +:+         +:+     //
//   By: rgramati <rgramati@student.42angouleme.fr  +#+  +:+       +#+        //
//                                                +#+#+#+#+#+   +#+           //
//   Created: 2024/10/14 00:49:19 by rgramati          #+#    #+#             //
//   Updated: 2024/10/16 22:52:11 by rgramati         ###   ########.fr       //
//                                                                            //
// ************************************************************************** //

#include <termengine.h>

void	te_terminal_hook(t_terminal *t, enum e_hook_types type, t_hook_func func, void *param)
{
	t->hook_table.hooks[type] = func;
	t->hook_table.params[type] = param;
}
