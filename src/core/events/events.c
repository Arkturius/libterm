// ************************************************************************** //
//                                                                            //
//                                                        :::      ::::::::   //
//   events.c                                           :+:      :+:    :+:   //
//                                                    +:+ +:+         +:+     //
//   By: rgramati <rgramati@student.42angouleme.fr  +#+  +:+       +#+        //
//                                                +#+#+#+#+#+   +#+           //
//   Created: 2024/11/14 18:55:46 by rgramati          #+#    #+#             //
//   Updated: 2024/11/18 16:41:23 by rgramati         ###   ########.fr       //
//                                                                            //
// ************************************************************************** //

#include <X11/X.h>
#include <X11/Xlib.h>
#include <stdint.h>
#include <termengine.h>

void	te_events_init(t_terminal *t)
{
	t_event_window			ev;
	Window					root;
	XSetWindowAttributes	attr;

	ev.display = XOpenDisplay(NULL);
	if (ev.display)
	{
		root = DefaultRootWindow(ev.display);
		attr.override_redirect = 1;
		ev.win = XCreateWindow(
			ev.display, root,
			0, 0, 1920, 1080, 0,
			CopyFromParent, InputOnly,
			CopyFromParent, CWOverrideRedirect,
			&attr
		);
		XSelectInput(
			ev.display, ev.win,
			ButtonPressMask | ButtonReleaseMask | PointerMotionMask \
			| KeyPressMask | KeyReleaseMask
		);
		XMapWindow(ev.display, ev.win);
		t->events = ev;
	}
}

void	te_events_destroy(t_terminal *t)
{
	if (t->events.display)
		XCloseDisplay(t->events.display);
}

void	te_events_peek(t_terminal *t, XEvent *event, uint32_t val)
{
	XEvent	new;

	if (XPending(t->events.display))
	{
		XPeekEvent(t->events.display, &new);
		if (new.type == KeyPress && 
			new.xkey.keycode == val && 
			new.xkey.time == event->xkey.time)
			t->events.flags = te_uint_unmask(t->events.flags, TE_EVENTS_PK);
		if (new.type == ButtonPress && 
			new.xbutton.button == val && 
			new.xbutton.time == event->xbutton.time)
			t->events.flags = te_uint_unmask(t->events.flags, TE_EVENTS_PK);
	}
}

void	te_events_handle(t_terminal *t)
{
	const te_event_func	events[5] =
	{
		te_events_keydown, 
		te_events_keyup, 
		te_events_mousedown, 
		te_events_mouseup, 
		te_events_mousemove
	};
	t_event_window		*ev;
	XEvent				event;

	ev = &t->events;
	XSetInputFocus(ev->display, ev->win, RevertToParent, CurrentTime);
	while (XPending(ev->display))
	{
		XNextEvent(ev->display, &event);
		events[event.type - 2](t, &event);
		if (event.type == KeyPress && event.xkey.keycode == 9)
			te_loop_end(t);
	}
}
