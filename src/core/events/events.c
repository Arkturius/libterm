// ************************************************************************** //
//                                                                            //
//                                                        :::      ::::::::   //
//   events.c                                           :+:      :+:    :+:   //
//                                                    +:+ +:+         +:+     //
//   By: rgramati <rgramati@student.42angouleme.fr  +#+  +:+       +#+        //
//                                                +#+#+#+#+#+   +#+           //
//   Created: 2024/11/14 18:55:46 by rgramati          #+#    #+#             //
//   Updated: 2024/12/02 18:58:40 by rgramati         ###   ########.fr       //
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
			CopyFromParent, CWOverrideRedirect, &attr
		);
		XSelectInput(
			ev.display, ev.win,
			ButtonPressMask | ButtonReleaseMask | PointerMotionMask \
			| KeyPressMask | KeyReleaseMask | FocusChangeMask
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

#include <stdio.h>
void	__te_events_focus_in(t_terminal *t, XEvent *event)
{
	(void)t;
	(void)event;
	dprintf(2, "focus in !!\n");
}

void	__te_events_focus_out(t_terminal *t, XEvent *event)
{
	(void)t;
	(void)event;
	dprintf(2, "focus out !!\n");
}

void	te_events_handle(t_terminal *t)
{
	static const te_event_func	events[FocusOut + 1] =
	{
		[KeyPress] = te_events_keydown, 
		[KeyRelease] = te_events_keyup, 
		[ButtonPress] = te_events_mousedown, 
		[ButtonRelease] = te_events_mouseup, 
		[MotionNotify] = te_events_mousemove,
		[FocusIn] = __te_events_focus_in,
		[FocusOut] = __te_events_focus_in
	};
	t_event_window		*ev;
	XEvent				event;

	ev = &t->events;
	XSetInputFocus(ev->display, ev->win, RevertToParent, CurrentTime);
	while (XPending(ev->display))
	{
		XNextEvent(ev->display, &event);
		if (events[event.type])
			events[event.type](t, &event);
		if (event.type == KeyPress && event.xkey.keycode == 9)
			te_loop_end(t);
	}
}
