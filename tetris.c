// ************************************************************************** //
//                                                                            //
//                                                        :::      ::::::::   //
//   tetris.c                                           :+:      :+:    :+:   //
//                                                    +:+ +:+         +:+     //
//   By: rgramati <rgramati@student.42angouleme.fr  +#+  +:+       +#+        //
//                                                +#+#+#+#+#+   +#+           //
//   Created: 2024/11/14 18:33:41 by rgramati          #+#    #+#             //
//   Updated: 2024/11/18 17:13:50 by rgramati         ###   ########.fr       //
//                                                                            //
// ************************************************************************** //

#include "te_keycodes.h"
#include <assert.h>
#include <limits.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/ioctl.h>
#include <time.h>
#include <unistd.h>
#include <termengine.h>

typedef enum e_block_type
{
	NOBLOCK = 0b000,
	BLOCK_O = 0b001,
	BLOCK_T = 0b010,
	BLOCK_L = 0b011,
	BLOCK_J = 0b100,
	BLOCK_Z = 0b101,
	BLOCK_S = 0b110,
	BLOCK_I = 0b111,
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
	double			time;
	uint32_t		lines;
	t_vec2			term;
	t_vec2			g_start;
	t_vec2			g_size;
	t_vec2			n_start;
	t_vec2			n_size;
	t_vec2			m_size;
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

#define DIGIT_0_PATH	"assets/digits/score_0.bmp"
#define DIGIT_1_PATH	"assets/digits/score_1.bmp"
#define DIGIT_2_PATH	"assets/digits/score_2.bmp"
#define DIGIT_3_PATH	"assets/digits/score_3.bmp"
#define DIGIT_4_PATH	"assets/digits/score_4.bmp"
#define DIGIT_5_PATH	"assets/digits/score_5.bmp"
#define DIGIT_6_PATH	"assets/digits/score_6.bmp"
#define DIGIT_7_PATH	"assets/digits/score_7.bmp"
#define DIGIT_8_PATH	"assets/digits/score_8.bmp"
#define DIGIT_9_PATH	"assets/digits/score_9.bmp"

#define BLOCK_I_RGB		TE_RGB_CYAN
#define BLOCK_O_RGB		TE_RGB_YELLOW
#define BLOCK_T_RGB		0xFF800080
#define BLOCK_L_RGB		0xFFFF7F00
#define BLOCK_J_RGB		TE_RGB_BLUE
#define BLOCK_Z_RGB		TE_RGB_RED
#define BLOCK_S_RGB		TE_RGB_GREEN

#define BLOCK_SIZE		10

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
		te_img_init(&block, BLOCK_SIZE, BLOCK_SIZE, data);
		cm_chunk_push(tetris->t->images, &block, sizeof(block));
		name++;
	}
}

void	tetris_init_digits(t_tetris *tetris)
{
	const char	*digits[11] = {DIGIT_0_PATH, DIGIT_1_PATH, DIGIT_2_PATH, \
		DIGIT_3_PATH, DIGIT_4_PATH, DIGIT_5_PATH, DIGIT_6_PATH, \
		DIGIT_7_PATH, DIGIT_8_PATH, DIGIT_9_PATH, NULL};
	const char	**name;
	uint32_t	*data;
	t_te_img	digit;

	name = digits;
	// te_img_init(&digit, 0, 0, NULL);
	// cm_chunk_push(tetris->t->images, &digit, sizeof(digit));
	while (*name)
	{
		cm_bmp(*name, &data, CM_OPEN_LOAD);
		te_img_init(&digit, 8, 6, data);
		cm_chunk_push(tetris->t->images, &digit, sizeof(digit));
		name++;
	}
}

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
}

void	tetris_draw_i(t_tetris *tetris, t_piece piece, t_te_img *img, t_vec2 center)
{
	int val;
	
	val = 2 * BLOCK_SIZE;
	if (piece.rota & 2)
		val = -2 * BLOCK_SIZE;
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
	grid_pos.x -= BLOCK_SIZE;
	grid_pos.y -= BLOCK_SIZE;
	img = cm_chunk_at(tetris->t->images, piece.type);
	while (++i < 9)
	{
		if (i == 4 || (shape & (1 << (7 - i + (i >= 4)))))
			te_screen_put_img(tetris->t, img, grid_pos);
		grid_pos.x += BLOCK_SIZE;
		if (i % 3 == 2)
		{
			grid_pos.x -= 3 * BLOCK_SIZE;
			grid_pos.y += BLOCK_SIZE;
		}
	}
	if (piece.type == BLOCK_I)
		tetris_draw_i(tetris, piece, img, cache);
}

#define TETRIS_RIGHT	0
#define TETRIS_LEFT		1

#define TETRIS_NEXT		0
#define TETRIS_ACTIVE	1

#define TETRIS_PIECE_INIT "\x00\x00\x24\x00\x45\x00\x65\x00\x85\x00\xa5\x00\xc5\x01\xe4\x00"

#define TETRIS_OFFSET_RIGHT	"\x01\x00\x01\x01\x02\x00\x01\x00\x01\x01\x01\x01\x01\x01\x01\x00"
#define TETRIS_OFFSET_LEFT	"\x01\x01\x01\x00\x01\x00\x02\x00\x00\x00\x00\x00\x01\x00\x01\x01"

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
		pos = (t_vec2){BLOCK_SIZE * curr.x + tetris->g_start.x + 1, BLOCK_SIZE * curr.y + tetris->g_start.y + 1};
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
			te_screen_put_img(tetris->t, img, (t_vec2){BLOCK_SIZE * inc.x + tetris->g_start.x + 1, BLOCK_SIZE * inc.y + tetris->g_start.y + 1});
		}
	}
}

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

	tetris->score += base[lines - 1] * (tetris->level + 1);
	tetris->score += tetris->fall_dist;
	tetris->fall_dist = 0;
}

int	tetris_break_line(t_tetris *tetris, t_piece proj)
{
	uint8_t	i;
	uint8_t	count;

	i = 0;
	count = 0;
	while (i < 10)
	{
		if (tetris_board_get(tetris, i, proj.y) != NOBLOCK)
			count++;
		i++;
	}
	if (count == 10)
	{
		tetris->line_break = 1;
		tetris->board[proj.y].line = 0;
		return (1);
	}
	return (0);
}

void	tetris_break_lines(t_tetris *tetris)
{
	t_piece	proj;

	uint8_t	lines;
	static int level_lines = 0;

	proj = tetris->current;
	proj.y = cm_min(19, proj.y + 2);
	lines = 0;
	while (1)
	{
		lines += tetris_break_line(tetris, proj);
		if (!proj.y || proj.y == tetris->current.y - 2)
			break ;
		proj.y--;
	}
	level_lines += lines;
	tetris->lines += lines;
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

void	tetris_itoi(t_tetris *tetris, uint64_t n)
{
	char		buffer[21];
	char		*cursor;
	t_te_img	*img;
	int			digit;

	buffer[20] = 0;
	cursor = &buffer[19];
	if (!n)
		*cursor-- = '0';
	while (n)
	{
		digit = n % 10;
		*cursor = digit + 48;
		n /= 10;
		cursor--;
	}
	t_vec2	pos = (t_vec2){.x = tetris->m_size.x - ((&buffer[20] - cursor) * 6), .y = tetris->m_size.y};
	while (*++cursor)
	{
		img = cm_chunk_at(tetris->t->images, 8 + *cursor - 48);
		te_screen_put_img(tetris->t, img, pos);
		pos.x += 6;
	}
}

#include <stdio.h>

void shuffle(uint32_t *array, int size)
{
    for (int i = size - 1; i > 0; i--)
	{
        int j = rand() % (i + 1);
        int temp = array[i];
        array[i] = array[j];
        array[j] = temp;
    }
	dprintf(2, "{");
	for (int i = 0; i < size; i++)
	{
		if (i)
			dprintf(2, ", ");
		dprintf(2, "%d", array[i]);
	}
	dprintf(2, "}\n");
}

#define NUM_PIECES 7

void tetris_bag(uint32_t *bag)
{
    for (int i = 0; i < NUM_PIECES; i++)
        bag[i] = i;
    shuffle(bag, NUM_PIECES);
}

void	tetris_spawn_piece(t_tetris *tetris)
{
	static uint32_t	piece = 0;
	static int		bag[7] = {0, 1, 2, 3, 4, 5, 6};

	if (!piece)
		tetris_bag((uint32_t *)&bag);
	tetris->current = tetris->next;
	tetris->next = *(t_piece *)&TETRIS_PIECE_INIT[(bag[piece++] + 1) << 1];
	piece = piece % 7;
	if (tetris_board_collide(tetris, tetris->current))
		te_loop_end(tetris->t);	
	tetris->new_piece = 0;
}

void	tetris_init(t_tetris *tetris, t_terminal *t_ptr)
{
	cm_memset(tetris, 0, sizeof(t_tetris));
	tetris->t = t_ptr;
	tetris->level = 16;
	tetris->term = (t_vec2){tetris->t->col, tetris->t->row * 2};
	tetris->g_start = (t_vec2){(tetris->term.x - 102) / 2, (tetris->term.y - 202) / 2};
	tetris->g_size = (t_vec2){100 + 2, 200 + 2};
	tetris->n_start = (t_vec2){46, 94};
	tetris->n_size = (t_vec2){48, 38};
	tetris->m_size = (t_vec2){tetris->t->col - 3, 4};
	tetris_init_blocks(tetris);
	tetris_init_digits(tetris);
	TE_HOOK(tetris->t, (t_hook_info){.key = TE_KEY_UP, .type = TE_KEYBOARD}, tetris_rotate_right, tetris);
	TE_HOOK(tetris->t, (t_hook_info){.key = TE_KEY_DOWN, .type = TE_KEYBOARD}, tetris_speed_up, tetris);
	TE_HOOK(tetris->t, (t_hook_info){.key = TE_KEY_RIGHT, .type = TE_KEYBOARD}, tetris_move_right, tetris);
	TE_HOOK(tetris->t, (t_hook_info){.key = TE_KEY_LEFT, .type = TE_KEYBOARD}, tetris_move_left, tetris);
	tetris_spawn_piece(tetris);
	tetris->new_piece = 1;
	tetris_itoi(tetris, 0);
}

#define TETRIS_FRAME_DELAY	"0\x00+\x00&\x00!\x00\x1c\x00\x17\x00\x12\x00\r\x00\x08\x00\x06\x00\x05\x00\x05\x00\x05\x00\x04\x00\x04\x00\x04\x00\x03\x00\x03\x00\x03\x00\x02\x00\x02\x00\x02\x00\x02\x00\x02\x00\x02\x00\x02\x00\x02\x00\x02\x00\x02\x00\x01\x00"

void	*tetris_loop(void *t_ptr)
{
	static t_tetris			tetris = {0};
	static double			test = 0;

	if (!tetris.t)
		tetris_init(&tetris, t_ptr);
	
	tetris_overlay(&tetris);

	if (tetris.new_piece)
		tetris_spawn_piece(&tetris);

	tetris_draw_piece(&tetris, TETRIS_ACTIVE);
	tetris_draw_piece(&tetris, TETRIS_NEXT);
	tetris_draw_board(&tetris);
	tetris.m_size.y = 4;
	tetris_itoi(&tetris, tetris.score);
	tetris.m_size.y = 16;
	tetris_itoi(&tetris, tetris.level);
	tetris.m_size.y = 28;
	tetris_itoi(&tetris, tetris.lines);


	tetris.time = *(uint16_t *)&TETRIS_FRAME_DELAY[cm_min(29, tetris.level) << 1];
	
	t_piece proj = tetris.current;
	
	if (!test)
		test = proj.y;
	test += tetris.t->time.delta_t * tetris.time;

	if ((int)test == proj.y)
		return (NULL);
	proj.y = test;
	test = 0;


	if (tetris.line_break)
	{
		tetris_lower_lines(&tetris);
		tetris.line_break = 0;
	}
	if (tetris_board_collide(&tetris, proj))
	{
		tetris_fix_piece(&tetris);
		tetris_break_lines(&tetris);
		tetris.new_piece = 1;
		return (NULL);
	}
	tetris.current = proj;
	return (NULL);
}
