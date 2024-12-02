// ************************************************************************** //
//                                                                            //
//                                                        :::      ::::::::   //
//   events_funcs.c                                     :+:      :+:    :+:   //
//                                                    +:+ +:+         +:+     //
//   By: rgramati <rgramati@student.42angouleme.fr  +#+  +:+       +#+        //
//                                                +#+#+#+#+#+   +#+           //
//   Created: 2024/11/16 18:51:15 by rgramati          #+#    #+#             //
//   Updated: 2024/12/02 19:35:51 by rgramati         ###   ########.fr       //
//                                                                            //
// ************************************************************************** //

#include <stdint.h>
#include <termengine.h>

void	te_events_keydown(t_terminal *t, XEvent *event)
{
	uint16_t	key;

	key = event->xkey.keycode;
	if (!t->key_table[key])
		te_uint_mask(t->events.flags, TE_EVENTS_PK);
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
	if (t->mouse[button] == 0)
		t->events.click = (t_vec2){(float)event->xbutton.x * t->col / 1920, (float)event->xbutton.y * t->row * 2 / 1080};
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
	t->events.mouse.x = ((float)event->xbutton.x / 1920) * t->col;
	t->events.mouse.y = ((float)event->xbutton.y / 1080) * (t->row * 2);
}

