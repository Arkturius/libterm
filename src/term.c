// ************************************************************************** //
//                                                                            //
//                                                        :::      ::::::::   //
//   term.c                                             :+:      :+:    :+:   //
//                                                    +:+ +:+         +:+     //
//   By: rgramati <rgramati@student.42angouleme.fr  +#+  +:+       +#+        //
//                                                +#+#+#+#+#+   +#+           //
//   Created: 2024/09/16 15:26:53 by rgramati          #+#    #+#             //
//   Updated: 2024/10/08 17:35:13 by rgramati         ###   ########.fr       //
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

uint32_t	te_guard_terminal(t_terminal *t)
{
	while (1)
	{
		if (!t)
			break ;
		if (!t->imgs || !t->entities || !t->anims)
			break ;
		if (!t->screen || !t->back)
			break ;
		return (0);
	}
	te_destroy(t);
	return (1);
}

t_terminal	*te_init(void)
{
	t_terminal	*t;

	t = malloc(sizeof(t_terminal));
	if (t)
	{
		t->row = TR_H;
		t->col = TR_W;
		t->imgs = ft_chunk_init("image", sizeof(t_te_img));
		t->entities = ft_chunk_init("entity", sizeof(t_entity));
		t->anims = ft_chunk_init("anim", sizeof(t_te_anim));
		t->screen = te_screen_init();
		t->back = te_screen_init();
		te_set_mode(TE_MODE_RENDER);
		te_ansi(TR_ANSI_CLEAR TR_ANSI_CURSOR_OFF);
	}
	if (te_guard_terminal(t))
		return (NULL);
	return (t);
}

void	te_destroy(t_terminal *t)
{
	ft_chunk_it_next(t->imgs);
	while (t->imgs->iterator.index < t->imgs->size)
	{
		te_img_destroy(t->imgs->iterator.it);
		ft_chunk_it_next(t->imgs);
	}
	ft_chunk_clear(t->imgs, FT_CHUNK_FREE);
	ft_chunk_clear(t->entities, FT_CHUNK_FREE);
	ft_chunk_clear(t->anims, FT_CHUNK_FREE);
	te_screen_destroy(t->screen);
	te_screen_destroy(t->back);
	te_set_mode(TE_MODE_BACKUP);
	te_ansi(TR_ANSI_CURSOR_ON);
	free(t);
}

#include <stdio.h>

void	te_loop(t_terminal *t)
{
	char	input;
	char	control_mask;

	control_mask = 0;
	while (1)
	{
		input = 0;
		read(STDIN_FILENO, &input, 1);
		if (input == 4)
			break ;
		if (input == 27)
			control_mask = (control_mask | 1) << 1;
		te_terminal_screen_shift(t);
		usleep(t->ifps);
	}
	te_ansi(TR_ANSI_CLEAR TR_ANSI_RESET);
}

