// ************************************************************************** //
//                                                                            //
//                                                        :::      ::::::::   //
//   events_funcs.c                                     :+:      :+:    :+:   //
//                                                    +:+ +:+         +:+     //
//   By: rgramati <rgramati@student.42angouleme.fr  +#+  +:+       +#+        //
//                                                +#+#+#+#+#+   +#+           //
//   Created: 2024/11/16 18:51:15 by rgramati          #+#    #+#             //
//   Updated: 2024/11/18 20:59:46 by rgramati         ###   ########.fr       //
//                                                                            //
// ************************************************************************** //

#include <termengine.h>

void	te_events_keydown(t_terminal *t, XEvent *event)
{
	uint16_t	key;

	key = event->xkey.keycode;
	if (!t->key_table[key])
		t->events.flags = te_uint_mask(t->events.flags, TE_EVENTS_PK);
	t->key_table[key] = 1;
}

void	te_events_keyup(t_terminal *t, XEvent *event)
{
	uint16_t	key;

	key = event->xkey.keycode;
	t->events.flags = te_uint_mask(t->events.flags, TE_EVENTS_PK);
	te_events_peek(t, event, key);
	if (te_uint_testmask(t->events.flags, TE_EVENTS_PK))
		t->key_table[key] = 0;
}

void	te_events_mousedown(t_terminal *t, XEvent *event)
{
	uint16_t	button;

	button = event->xbutton.button - 1;
	if (!t->mouse[button])
		t->events.flags = te_uint_mask(t->events.flags, TE_EVENTS_PK);
	t->mouse[button] = 1;
	t->color = TE_RGB_RED;
}

void	te_events_mouseup(t_terminal *t, XEvent *event)
{
	uint16_t	button;

	button = event->xbutton.button - 1;
	t->events.flags = te_uint_mask(t->events.flags, TE_EVENTS_PK);
	te_events_peek(t, event, button);
	if (te_uint_testmask(t->events.flags, TE_EVENTS_PK))
		t->mouse[button] = 0;
	t->color = TE_RGB_YELLOW;
}

void	te_events_mousemove(t_terminal *t, XEvent *event)
{
	t->events.mouse.x = event->xbutton.x;
	t->events.mouse.y = event->xbutton.y;
}

