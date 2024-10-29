// ************************************************************************** //
//                                                                            //
//                                                        :::      ::::::::   //
//   main.c                                             :+:      :+:    :+:   //
//                                                    +:+ +:+         +:+     //
//   By: rgramati <rgramati@student.42angouleme.fr  +#+  +:+       +#+        //
//                                                +#+#+#+#+#+   +#+           //
//   Created: 2024/10/02 14:49:44 by rgramati          #+#    #+#             //
//   Updated: 2024/10/29 01:07:28 by rgramati         ###   ########.fr       //
//                                                                            //
// ************************************************************************** //

#include <limits.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/ioctl.h>
#include <unistd.h>

#include <termengine.h>

// TILE EDIT /////
#include "cmem.h"
#include "src/tools/te_tools.h"
void	te_tool_start(t_terminal *t, t_tool tool)
{
	if (tool == TE_TILE_EDIT)
	{
		TE_HOOK(t, TE_LOOP, te_tool_etile, t);
	}
}
//////////////////


typedef struct s_tetris
{
	t_terminal	*t;
	uint64_t	score;
	uint64_t	frame;
	t_vec2		term;
	t_vec2		g_start;
	t_vec2		g_size;
	t_vec2		n_start;
	t_vec2		n_size;
}	t_tetris;

void	tetris_init(t_tetris *tetris, t_terminal *t_ptr)
{
	const char	*blocks[8] = {"i_block.bmp", "o_block.bmp", "t_block.bmp", \
		"l_block.bmp", "j_block.bmp", "z_block.bmp", "s_block.bmp", NULL};
	const char	**name;
	uint32_t	*data;
	t_te_img	block;

	cm_memset(tetris, 0, sizeof(t_tetris));
	tetris->t = t_ptr;
	tetris->term = (t_vec2){tetris->t->col, tetris->t->row * 2};
	tetris->g_start = (t_vec2){(tetris->term.x - 102) / 2, (tetris->term.y - 202) / 2};
	tetris->g_size = (t_vec2){100 + 2, 200 + 2};
	tetris->n_start = (t_vec2){46, 84};
	tetris->n_size = (t_vec2){48, 48};
	name = blocks;
	while (*name)
	{
		cm_bmp(*name, &data, CM_OPEN_LOAD);
		te_img_init(&block, 10, 10, data);
		cm_chunk_push(tetris->t->images, &block, sizeof(block));
		name++;
	}
}

#define TETRIMINO_RGB_I	TE_RGB_CYAN
#define TETRIMINO_RGB_O	TE_RGB_YELLOW
#define TETRIMINO_RGB_T	0xFF800080
#define TETRIMINO_RGB_L	0xFFFF7F00
#define TETRIMINO_RGB_J	TE_RGB_BLUE
#define TETRIMINO_RGB_Z	TE_RGB_RED
#define TETRIMINO_RGB_S	TE_RGB_GREEN

t_vec2	tetris_border(t_vec2 vec, uint32_t is_size)
{
	if (is_size)
		return ((t_vec2){vec.x + 4, vec.y + 4});
	return ((t_vec2){vec.x - 2, vec.y - 2});
}

void	tetris_put_block_i(t_tetris *tetris, t_vec2 g_start, t_vec2 grid_pos)
{
	t_vec2	screen_pos;

	screen_pos = (t_vec2){g_start.x + 1 + 10 * grid_pos.x, g_start.y + 1 + 10 * grid_pos.y};
	for (int i = 0; i < 4; i++)
	{
		t_te_img *test = cm_chunk_at(tetris->t->images, 0);
		te_screen_put_img(tetris->t, test, screen_pos);
		screen_pos.x += 10;
	}
}

void	tetris_overlay(t_tetris *tetris)
{
	te_screen_draw_square(tetris->t, (t_vec2){0, 0}, tetris->term, TE_RGB_WHITE);
	te_screen_draw_square(tetris->t, tetris_border(tetris->g_start, 0), tetris_border(tetris->g_size, 1), TE_RGB_WHITE);	
	te_screen_draw_square(tetris->t, tetris->g_start, tetris->g_size, 0xFF7F7F7F);	
	te_screen_draw_square(tetris->t, tetris_border(tetris->n_start, 0), tetris_border(tetris->n_size, 1), TE_RGB_WHITE);	
	te_screen_draw_square(tetris->t, tetris->n_start, tetris->n_size, 0xFF7F7F7F);
}

void	*tetris_block_move_right(void *ptr)
{
	t_vec2	*pos;

	pos = (t_vec2 *)ptr;
	pos->x += (pos->x < 10);
	return (NULL);
}

void	*tetris_block_move_left(void *ptr)
{
	t_vec2	*pos;

	pos = (t_vec2 *)ptr;
	pos->x -= (pos->x > 0);
	return (NULL);
}

void	*tetris_loop(void *t_ptr)
{
	static t_tetris	tetris = {0};
	static t_vec2	pos = {0};

	if (!tetris.t)
	{
		tetris_init(&tetris, t_ptr);
		TE_HOOK(tetris.t, TE_ESCAPE | 'C', tetris_block_move_right, (void *)&pos);
		TE_HOOK(tetris.t, TE_ESCAPE | 'D', tetris_block_move_left, (void *)&pos);
	}
	tetris.frame++;
	tetris_overlay(&tetris);

	if (!(tetris.frame % 60))
		pos.y++;

	tetris_put_block_i(&tetris, tetris.g_start, pos);
	return (NULL);
}

#include "img.c"

int	main(__attribute__((unused)) int argc, __attribute__((unused)) char **argv)
{
	t_terminal	*t;

	t = te_init();
	if (t)
	{
		te_terminal_fps_max(t, 120);
		TE_HOOK(t, TE_LOOP, tetris_loop, t);
		te_loop(t);
	}
	te_destroy(t);
}
