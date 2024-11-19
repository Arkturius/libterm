// ************************************************************************** //
//                                                                            //
//                                                        :::      ::::::::   //
//   terminal.c                                         :+:      :+:    :+:   //
//                                                    +:+ +:+         +:+     //
//   By: rgramati <rgramati@student.42angouleme.fr  +#+  +:+       +#+        //
//                                                +#+#+#+#+#+   +#+           //
//   Created: 2024/09/16 15:26:53 by rgramati          #+#    #+#             //
//   Updated: 2024/11/18 16:43:50 by rgramati         ###   ########.fr       //
//                                                                            //
// ************************************************************************** //

#include <X11/Xlib.h>
#include <stdint.h>
#include <stdlib.h>

#include <sys/ioctl.h>
#include <termengine.h>
#include <unistd.h>

#define TE_MODE_RENDER	1
#define TE_MODE_BACKUP	0

static uint32_t	te_set_mode(uint32_t mode)
{
	struct termios			attr;
	static struct termios	back;

	if (!isatty(STDIN_FILENO))
		return (1);
	tcgetattr(STDIN_FILENO, &back);
	if (mode == TE_MODE_RENDER)
	{
		attr = back;
		attr.c_lflag &= ~(ICANON | ECHO);
		attr.c_cc[VMIN] = 0;
		attr.c_cc[VTIME] = 0;
		tcsetattr(STDIN_FILENO, TCSAFLUSH, &attr);
	}
	else
		tcsetattr(STDIN_FILENO, TCSANOW, &back);
	return (0);
}

static void	te_terminal_get_size(uint32_t *row, uint32_t *col)
{
	struct winsize	win;

	ioctl(STDIN_FILENO, TIOCGWINSZ, &win);
	*row = win.ws_row;
	*col = win.ws_col;
}

t_terminal	*te_init(void)
{
	t_terminal	*t;

	t = malloc(sizeof(t_terminal));
	if (t)
	{
		cm_memset(t, 0, sizeof(t_terminal));
		te_terminal_get_size(&t->row, &t->col);
		t->screen = te_screen_init(t);
		t->back = te_screen_init(t);
		te_set_mode(TE_MODE_RENDER);
		te_ansi(TE_ANSI_CLEAR TE_ANSI_CURSOR_OFF);
		t->images = cm_chunk_init(sizeof(t_te_img), NULL, 0);
		t->color = TE_RGB_BLUE;
		te_events_init(t);
		if (!t->screen || !t->back || !t->images || !t->events.display)
		{
			te_destroy(t);
			return (NULL);
		}
		t->active = 1;
	}
	return (t);
}

void	te_destroy(t_terminal *t)
{
	t_te_img	*tmp;
	t_cm_chunk	*ctmp;
	uint32_t	i;

	if (!t)
		return ;
	te_screen_destroy(t->screen);
	te_screen_destroy(t->back);
	i = 0;
	ctmp = t->images;
	tmp = cm_chunk_at(ctmp, i);
	while (tmp)
	{
		if (i == cm_chunk_size(ctmp))
		{
			i = 0;
			ctmp = cm_chunk_next(ctmp);
			if (!ctmp)
				break ;
		}
		tmp = cm_chunk_at(ctmp, i);
		te_img_destroy(tmp);
		i++;
	}
	cm_chunk_clear(t->images, CM_CLEAR_FREE);
	te_events_destroy(t);
	te_set_mode(TE_MODE_BACKUP);
	te_ansi(TE_ANSI_CURSOR_ON);
	te_ansi(TE_ANSI_CLEAR TE_ANSI_RESET);
	free(t);
}

void	te_loop_end(t_terminal *t)
{
	t->active = 0;
}

void	te_keys_handle(t_terminal *t)
{
	uint32_t	key;
	t_hook_func	*funcs;
	void		**params;

	key = 0;
	funcs = t->hook_table.keyboard_hooks;
	params = t->hook_table.keyboard_params;
	while (key < TE_HOOK_KEYBOARD)
	{
		if (t->keyboard[key] && funcs[key])
			funcs[key](params[key]);
		key++;
	}
}

void	te_mouse_handle(t_terminal *t)
{
	uint32_t	key;
	t_hook_func	*funcs;
	void		**params;

	key = 0;
	funcs = t->hook_table.mouse_hooks;
	params = t->hook_table.mouse_params;
	while (key < TE_HOOK_MOUSE)
	{
		if (t->mouse[key] && funcs[key])
			funcs[key](params[key]);
		key++;
	}
}

void	te_loop(t_terminal *t)
{
	while (t->active)
	{
		te_events_handle(t);
		te_keys_handle(t);
		te_mouse_handle(t);
		if (t->hook_table.hooks[TE_LOOP])
			t->hook_table.hooks[TE_LOOP](t->hook_table.params[TE_LOOP]);
		te_terminal_screen_shift(t);
		te_delta_time(t);
		te_usleep(1000 / (t->fps));
	}
}

