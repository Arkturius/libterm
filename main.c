// ************************************************************************** //
//                                                                            //
//                                                        :::      ::::::::   //
//   main.c                                             :+:      :+:    :+:   //
//                                                    +:+ +:+         +:+     //
//   By: rgramati <rgramati@student.42angouleme.fr  +#+  +:+       +#+        //
//                                                +#+#+#+#+#+   +#+           //
//   Created: 2024/10/02 14:49:44 by rgramati          #+#    #+#             //
//   Updated: 2024/11/05 19:07:12 by rgramati         ###   ########.fr       //
//                                                                            //
// ************************************************************************** //

#include <assert.h>
#include <limits.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/ioctl.h>
#include <time.h>
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

typedef enum e_block_type
{
	NOBLOCK = 0b000,
	BLOCK_O = 0b001,	//
	BLOCK_T = 0b010,
	BLOCK_L = 0b011,
	BLOCK_J = 0b100,
	BLOCK_Z = 0b101,
	BLOCK_S = 0b110,	//
	BLOCK_I = 0b111,	//
}	t_block_type;

typedef union s_block_line
{
	uint64_t	line;
	struct
	{
		uint8_t	block_0:4;
		uint8_t	block_1:4;
		uint8_t	block_2:4;
		uint8_t	block_3:4;
		uint8_t	block_4:4;
		uint8_t	block_5:4;
		uint8_t	block_6:4;
		uint8_t	block_7:4;
		uint8_t	block_8:4;
		uint8_t	block_9:4;
	};
}	t_block_line;

typedef struct s_piece
{
	uint8_t	x:5;
	uint8_t	type:3;
	uint8_t y:5;
	uint8_t	rota:2;
}	t_piece;

typedef struct s_tetris
{
	t_terminal		*t;
	uint64_t		score;
	uint64_t		frame;
	t_vec2			term;
	t_vec2			g_start;
	t_vec2			g_size;
	t_vec2			n_start;
	t_vec2			n_size;
	t_block_line	board[20];
	t_piece			current;
	t_piece			next;
	uint8_t			level;
	uint8_t			new_piece:1;
	uint8_t			line_break:1;
	uint8_t			is_falling:1;
	uint8_t			fall_dist:5;
}	t_tetris;

#define BLOCK_I_PATH	"assets/blocks/I_block.bmp"
#define BLOCK_O_PATH	"assets/blocks/O_block.bmp"
#define BLOCK_T_PATH	"assets/blocks/T_block.bmp"
#define BLOCK_L_PATH	"assets/blocks/L_block.bmp"
#define BLOCK_J_PATH	"assets/blocks/J_block.bmp"
#define BLOCK_Z_PATH	"assets/blocks/Z_block.bmp"
#define BLOCK_S_PATH	"assets/blocks/S_block.bmp"

void	tetris_init_blocks(t_tetris *tetris)
{
	const char	*blocks[9] = {BLOCK_O_PATH, BLOCK_T_PATH, BLOCK_L_PATH, \
		BLOCK_J_PATH, BLOCK_Z_PATH, BLOCK_S_PATH, BLOCK_I_PATH, NULL};
	const char	**name;
	uint32_t	*data;
	t_te_img	block;

	name = blocks;
	te_img_init(&block, 0, 0, NULL);
	cm_chunk_push(tetris->t->images, &block, sizeof(block));
	while (*name)
	{
		cm_bmp(*name, &data, CM_OPEN_LOAD);
		te_img_init(&block, 10, 10, data);
		cm_chunk_push(tetris->t->images, &block, sizeof(block));
		name++;
	}
}

#define BLOCK_I_RGB	TE_RGB_CYAN
#define BLOCK_O_RGB	TE_RGB_YELLOW
#define BLOCK_T_RGB	0xFF800080
#define BLOCK_L_RGB	0xFFFF7F00
#define BLOCK_J_RGB	TE_RGB_BLUE
#define BLOCK_Z_RGB	TE_RGB_RED
#define BLOCK_S_RGB	TE_RGB_GREEN

t_vec2	tetris_border(t_vec2 vec, uint32_t is_size)
{
	if (is_size)
		return ((t_vec2){vec.x + 4, vec.y + 4});
	return ((t_vec2){vec.x - 2, vec.y - 2});
}

uint8_t	tetris_board_get(t_tetris *tetris, uint8_t x, uint8_t y)
{
	if (y > 235)
		return (NOBLOCK);
	if (y > 19)
		return (1);
	return (tetris->board[y].line >> (4 * (6 + x)) & 0x7);
}

void	tetris_board_set(t_tetris *tetris, uint8_t x, uint8_t y)
{
	uint64_t	mask;

	mask = tetris->current.type;
	mask <<= (4 * (6 + x));
	tetris->board[y].line |= mask;
}
void	tetris_overlay(t_tetris *tetris)
{
	te_screen_draw_square(tetris->t, (t_vec2){0, 0}, tetris->term, TE_RGB_WHITE);
	te_screen_draw_square(tetris->t, tetris_border(tetris->g_start, 0), tetris_border(tetris->g_size, 1), TE_RGB_WHITE);	
	te_screen_draw_square(tetris->t, tetris->g_start, tetris->g_size, 0xFF7F7F7F);	
	te_screen_draw_square(tetris->t, tetris_border(tetris->n_start, 0), tetris_border(tetris->n_size, 1), TE_RGB_WHITE);	
	te_screen_draw_square(tetris->t, tetris->n_start, tetris->n_size, 0xFF7F7F7F);
	for (int i = 0; i < tetris->level + 1; i++)
		te_screen_set_pixel(tetris->t, (t_vec2){10, 10 + 2 * i}, TE_RGB_MAGENTA);
}

void	tetris_draw_i(t_tetris *tetris, t_piece piece, t_te_img *img, t_vec2 center)
{
	int val;
	
	val = 20;
	if (piece.rota & 2)
		val = -20;
	if (piece.rota & 1)
		te_screen_put_img(tetris->t, img, (t_vec2){center.x, (uint32_t)center.y + val});
	else
		te_screen_put_img(tetris->t, img, (t_vec2){(uint32_t)center.x + val, center.y});
}

void	tetris_draw_blocks(t_tetris *tetris, t_vec2 grid_pos, t_piece piece, uint32_t shape)
{
	t_te_img	*img;
	t_vec2		cache;
	uint8_t		i;

	i = -1;
	cache = grid_pos;
	grid_pos.x -= 10;
	grid_pos.y -= 10;
	img = cm_chunk_at(tetris->t->images, piece.type);
	while (++i < 9)
	{
		if (i == 4 || (shape & (1 << (7 - i + (i >= 4)))))
			te_screen_put_img(tetris->t, img, grid_pos);
		grid_pos.x += 10;
		if (i % 3 == 2)
		{
			grid_pos.x -= 30;
			grid_pos.y += 10;
		}
	}
	if (piece.type == BLOCK_I)
		tetris_draw_i(tetris, piece, img, cache);
}

#define TETRIS_RIGHT	0
#define TETRIS_LEFT		1

//
//	0 1 2
//	3   4
//  5 6 7 * 4 * 7
//
//

#define BLOCK_SHAPES	"\x00\x00\x00\x00\x0b\x0b\x0b\x0b\x1a\x52\x58\x4a\x1c\xc2\x38\x43\x19\x46\x98\x62\x13\x54\xc8\x2a\x70\x49\x0e\x92\x18\x42\x18\x42"

void	tetris_draw_piece(t_tetris *tetris, uint8_t active)
{
	t_vec2			pos;
	t_piece			curr;
	uint8_t			shape;

	if (active)
		curr = tetris->current;
	else
		curr = tetris->next;
	shape = (*(uint32_t *)&BLOCK_SHAPES[curr.type << 2] >> (8 * curr.rota)) & 0xFF;
	if (!active)
	{
		pos = (t_vec2){18 + tetris->n_start.x + 1, 9 + tetris->n_start.y + 1};
		if (curr.type == BLOCK_I)
			pos.y += 5;
		if (curr.type == BLOCK_S)
			pos.y += 10;
		if (curr.type == BLOCK_O || curr.type == BLOCK_I)
			pos.x -= 5;
	}
	else
		pos = (t_vec2){10 * curr.x + tetris->g_start.x + 1, 10 * curr.y + tetris->g_start.y + 1};
	tetris_draw_blocks(tetris, pos, curr, shape);
}

void	tetris_draw_board(t_tetris *tetris)
{
	t_vec2		inc;
	uint8_t		type;
	t_te_img	*img;

	inc.y = -1;
	while (++inc.y < 20)
	{
		inc.x = -1;
		while (++inc.x < 10)
		{
			type = tetris_board_get(tetris, inc.x, inc.y);
			if (type == NOBLOCK)
				continue ;
			img = cm_chunk_at(tetris->t->images, type);
			te_screen_put_img(tetris->t, img, (t_vec2){10 * inc.x + tetris->g_start.x + 1, 10 * inc.y + tetris->g_start.y + 1});
		}
	}
}

#define TETRIS_PIECE_INIT "\x00\x00\x24\x00\x45\x00\x65\x00\x85\x00\xa5\x00\xc5\x01\xe4\x00"

#define TETRIS_ACTIVE	1
#define TETRIS_NEXT		0


#define TETRIS_OFFSET_RIGHT	"\x01\x00\x01\x01\x02\x00\x01\x00\x01\x01\x01\x01\x01\x01\x01\x00"
#define TETRIS_OFFSET_LEFT	"\x01\x01\x01\x00\x01\x00\x02\x00\x00\x00\x00\x00\x01\x00\x01\x01"

uint32_t	tetris_wall_collide(t_piece curr, uint8_t side)
{
	uint32_t	index;
	uint32_t	offset;

	index = 0;
	if (curr.type == BLOCK_I)
		index = 1;
	if (curr.type == BLOCK_O)
		index = 2;
	if (curr.type == BLOCK_S)
		index = 3;
	if (!side)
	{
		offset = *(uint32_t *)&TETRIS_OFFSET_RIGHT[index << 2];
		if (curr.x + ((offset >> (8 * curr.rota)) & 0xFF) < 9)
			return (0);
	}
	else
	{
		offset = *(uint32_t *)&TETRIS_OFFSET_LEFT[index << 2];
		if (curr.x - ((offset >> (8 * curr.rota)) & 0xFF) > 0)
			return (0);
	}
	return (1);
}

uint32_t	tetris_board_collide(t_tetris *tetris, t_piece proj)
{
	uint8_t	i;
	uint8_t	shape;

	shape = (*(uint32_t *)&BLOCK_SHAPES[proj.type << 2] >> (8 * proj.rota)) & 0xFF;
	i = -1;
	while (++i < 9)
	{
		if (i == 4 || (shape & (1 << (7 - i + (i >= 4)))))
		{
			if ((tetris_board_get(tetris, proj.x + (i % 3) - 1, proj.y + (i / 3) - 1) != NOBLOCK))
				return (1);
		}
	}	
	if (proj.type == BLOCK_I)
	{
		int val;
	
		val = 2;
		if (proj.rota & 2)
			val = -2;
		if (proj.rota & 1)
			return (tetris_board_get(tetris, proj.x, proj.y + val) != NOBLOCK);
		else
			return (tetris_board_get(tetris, proj.x + val, proj.y) != NOBLOCK);
	}
	return (0);
}

void	*tetris_move_left(void *tetris_ptr)
{
	t_tetris	*tetris;
	t_piece		proj;

	tetris = (t_tetris *)tetris_ptr;
	proj = tetris->current;
	proj.x--;
	if (tetris_board_collide(tetris, proj))
		return (NULL);
	if (!tetris_wall_collide(tetris->current, TETRIS_LEFT))
		tetris->current.x--;
	return (NULL);
}

void	*tetris_move_right(void *tetris_ptr)
{
	t_tetris	*tetris;
	t_piece		proj;

	tetris = (t_tetris *)tetris_ptr;
	proj = tetris->current;
	proj.x++;
	if (tetris_board_collide(tetris, proj))
		return (NULL);
	if (!tetris_wall_collide(tetris->current, TETRIS_RIGHT))
		tetris->current.x++;
	return (NULL);
}


void	*tetris_rotate_right(void *tetris_ptr)
{
	t_tetris	*tetris;
	t_piece		proj;

	tetris = (t_tetris *)tetris_ptr;
	proj = tetris->current;
	proj.rota++;
	if (tetris_board_collide(tetris, proj))
		return (NULL);
	proj.x--;
	if (tetris_wall_collide(proj, TETRIS_RIGHT))
		return (NULL);
	proj.x += 2;
	if (tetris_wall_collide(proj, TETRIS_LEFT))
		return (NULL);
	tetris->current.rota++;
	return (NULL);
}

void	tetris_fix_piece(t_tetris *tetris)
{
	t_piece	proj;
	uint8_t	i;
	uint8_t	shape;

	proj = tetris->current;
	shape = (*(uint32_t *)&BLOCK_SHAPES[proj.type << 2] >> (8 * proj.rota)) & 0xFF;
	i = -1;
	while (++i < 9)
	{
		if (i == 4 || (shape & (1 << (7 - i + (i >= 4)))))
			tetris_board_set(tetris, proj.x + (i % 3) - 1, proj.y + (i / 3) - 1);
	}
	if (proj.type == BLOCK_I)
	{
		int val;
	
		val = 2;
		if (proj.rota & 2)
			val = -2;
		if (proj.rota & 1)
			tetris_board_set(tetris, proj.x, proj.y + val);
		else
			tetris_board_set(tetris, proj.x + val, proj.y);
	}
}

void	tetris_score(t_tetris *tetris, uint8_t lines)
{
	const uint32_t	base[4] = {40, 100, 300, 1200};

	dprintf(2, "Lines addition: %u * %u = %u\n", base[lines - 1], tetris->level + 1, base[lines - 1] * (tetris->level + 1));
	tetris->score += base[lines - 1] * (tetris->level + 1);
	dprintf(2, "Fall distance : %u\n", tetris->fall_dist);
	tetris->score += tetris->fall_dist;
	dprintf(2, "NEW SCORE = %lu\n\n", tetris->score);
	tetris->fall_dist = 0;
}

void	tetris_break_lines(t_tetris *tetris)
{
	t_piece	proj;
	uint8_t	i;
	uint8_t	j;
	uint8_t	count;
	uint8_t	lines;
	static int level_lines = 0;

	proj = tetris->current;
	proj.y += 2;
	i = 0;
	lines = 0;
	while (i < 5)
	{
		if (proj.y > 19)
		{
			proj.y--;
			i++;
			continue ;
		}
		j = 0;
		count = 0;
		while (j < 10)
		{
			if (tetris_board_get(tetris, j, proj.y) != NOBLOCK)
				count++;
			j++;
		}
		if (count == 10)
		{
			tetris->line_break = 1;
			level_lines++;
			lines++;
			tetris->board[proj.y].line = 0;
		}
		proj.y--;
		i++;
	}
	if (lines)
		tetris_score(tetris, lines);
	if (level_lines >= 10)
	{
		level_lines -= 10;
		tetris->level++;
	}
}

#include <string.h>

void	tetris_lower_lines(t_tetris *tetris)
{
	uint8_t	i;

	i = 0;
	while (i < 20)
	{
		if (!tetris->board[i].line)
			memmove(&tetris->board[1], &tetris->board[0], i * sizeof(t_block_line));
		i++;
	}
}

void	*tetris_speed_up(void *tetris_ptr)
{
	t_tetris	*tetris;
	t_piece		proj;

	tetris = (t_tetris *)tetris_ptr;
	proj = tetris->current;
	proj.y++;
	if (!tetris_board_collide(tetris, proj))
	{
		tetris->current.y++;
		tetris->is_falling = 1;
	}
	return (NULL);
}

void	tetris_init(t_tetris *tetris, t_terminal *t_ptr)
{
	cm_memset(tetris, 0, sizeof(t_tetris));
	tetris->t = t_ptr;
	tetris->term = (t_vec2){tetris->t->col, tetris->t->row * 2};
	tetris->g_start = (t_vec2){(tetris->term.x - 102) / 2, (tetris->term.y - 202) / 2};
	tetris->g_size = (t_vec2){100 + 2, 200 + 2};
	tetris->n_start = (t_vec2){46, 94};
	tetris->n_size = (t_vec2){48, 38};
	tetris_init_blocks(tetris);
	TE_HOOK(tetris->t, TE_ESCAPE | 'A', tetris_rotate_right, tetris);
	TE_HOOK(tetris->t, TE_ESCAPE | 'B', tetris_speed_up, tetris);
	TE_HOOK(tetris->t, TE_ESCAPE | 'C', tetris_move_right, tetris);
	TE_HOOK(tetris->t, TE_ESCAPE | 'D', tetris_move_left, tetris);
	tetris->new_piece = 1;
	tetris->next = *(t_piece *)&TETRIS_PIECE_INIT[BLOCK_I << 1];
}

void	*tetris_loop(void *t_ptr)
{
	static t_tetris	tetris = {0};
	uint32_t		piece;

	if (!tetris.t)
		tetris_init(&tetris, t_ptr);

	tetris_overlay(&tetris);
	if (tetris.new_piece)
	{
		piece = (rand() % 7) + 1;
		while (piece - 1 == tetris.current.type)
			piece = (rand() % 7) + 1;
		tetris.current = tetris.next;
		tetris.next = *(t_piece *)&TETRIS_PIECE_INIT[piece << 1];
		if (tetris_board_collide(&tetris, tetris.current))
		{
			tetris.t->active = 0;
			dprintf(2, "FINAL SCORE : %lu\n", tetris.score);
			return (NULL);
		}
		tetris.new_piece = 0;
	}

	tetris_draw_piece(&tetris, TETRIS_ACTIVE);
	tetris_draw_piece(&tetris, TETRIS_NEXT);
	tetris_draw_board(&tetris);

	tetris.frame++;

	if (!(tetris.frame % 30))
	{
		if (tetris.line_break)
		{
			tetris_lower_lines(&tetris);
			tetris.line_break = 0;
			return (NULL);
		}
		t_piece proj = tetris.current;
		proj.y++;
		if (tetris_board_collide(&tetris, proj))
		{
			tetris_fix_piece(&tetris);
			tetris_break_lines(&tetris);
			tetris.new_piece = 1;
			tetris.is_falling = 0;
			return (NULL);
		}
		tetris.current.y++;
	}
	return (NULL);
}

int	main(__attribute__((unused)) int argc, __attribute__((unused)) char **argv)
{
	t_terminal	*t;

	srand(time(NULL));
	t = te_init();
	if (t)
	{
		te_terminal_fps_max(t, 120);
		TE_HOOK(t, TE_LOOP, tetris_loop, t);
		te_loop(t);
	}
	te_destroy(t);
}
