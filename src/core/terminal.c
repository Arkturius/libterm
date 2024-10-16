// ************************************************************************** //
//                                                                            //
//                                                        :::      ::::::::   //
//   terminal.c                                         :+:      :+:    :+:   //
//                                                    +:+ +:+         +:+     //
//   By: rgramati <rgramati@student.42angouleme.fr  +#+  +:+       +#+        //
//                                                +#+#+#+#+#+   +#+           //
//   Created: 2024/09/16 15:26:53 by rgramati          #+#    #+#             //
//   Updated: 2024/10/17 00:36:15 by rgramati         ###   ########.fr       //
//                                                                            //
// ************************************************************************** //

#include <stdint.h>
#include <stdlib.h>

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

t_terminal	*te_init(void)
{
	t_terminal	*t;

	t = malloc(sizeof(t_terminal));
	if (t)
	{
		t->row = TE_H * 2;
		t->col = TE_W;
		t->screen = te_screen_init();
		t->back = te_screen_init();
		te_set_mode(TE_MODE_RENDER);
		te_ansi(TE_ANSI_CLEAR TE_ANSI_CURSOR_OFF);
		if (!t->screen || !t->back)
		{
			te_destroy(t);
			return (NULL);
		}
	}
	return (t);
}

void	te_destroy(t_terminal *t)
{
	te_screen_destroy(t->screen);
	te_screen_destroy(t->back);
	te_set_mode(TE_MODE_BACKUP);
	te_ansi(TE_ANSI_CURSOR_ON);
	te_ansi(TE_ANSI_CLEAR TE_ANSI_RESET);
	free(t);
}

void	te_handle_keys(t_terminal *t, char seq[4])
{
	const unsigned char	key = (unsigned char)seq[0];
	uint8_t				*states;
	t_hook_func			*hooks;

	states = t->hook_table.states;
	hooks = t->hook_table.hooks;
	if (key > 0 && key < TE_KEYUP)
	{
		if (states[key])
			states[key] = 0;
		if (hooks[TE_KEYDOWN | key])
			hooks[TE_KEYDOWN | key](t->hook_table.params[TE_KEYDOWN | key]);
	}	
}

void	te_loop(t_terminal *t)
{
	char		seq[4];

	while (42)
	{
		*(uint32_t *)seq = 0;
		if (read(STDIN_FILENO, &seq, 4) < 0)
			break ;
		// if (seq[0] == TE_ESQ)
			// te_handle_escape();
		if (seq[0] == TE_EOF)
			break ;
		te_handle_keys(t, seq);
		if (t->hook_table.hooks[TE_LOOP])
			t->hook_table.hooks[TE_LOOP](t->hook_table.params[TE_LOOP]);
		te_terminal_screen_shift(t);
		usleep(t->ifps);
	}
}

