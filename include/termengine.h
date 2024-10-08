// ************************************************************************** //
//                                                                            //
//                                                        :::      ::::::::   //
//   termengine.h                                       :+:      :+:    :+:   //
//                                                    +:+ +:+         +:+     //
//   By: rgramati <rgramati@student.42angouleme.fr  +#+  +:+       +#+        //
//                                                +#+#+#+#+#+   +#+           //
//   Created: 2024/10/02 14:15:54 by rgramati          #+#    #+#             //
//   Updated: 2024/10/07 21:20:34 by rgramati         ###   ########.fr       //
//                                                                            //
// ************************************************************************** //

#ifndef TERMRENDER_H
# define TERMRENDER_H

# include <unistd.h>
# include <stdint.h>
# include <termios.h>
# include <assert.h>
# include <sys/ioctl.h>

# include <ft_chunk.h>

# define TR_COLOR_TABLE \
	"000\000001\000002\000003\000004\000005\000006\000007\000"\
	"008\000009\000010\000011\000012\000013\000014\000015\000"\
	"016\000017\000018\000019\000020\000021\000022\000023\000"\
	"024\000025\000026\000027\000028\000029\000030\000031\000"\
	"032\000033\000034\000035\000036\000037\000038\000039\000"\
	"040\000041\000042\000043\000044\000045\000046\000047\000"\
	"048\000049\000050\000051\000052\000053\000054\000055\000"\
	"056\000057\000058\000059\000060\000061\000062\000063\000"\
	"064\000065\000066\000067\000068\000069\000070\000071\000"\
	"072\000073\000074\000075\000076\000077\000078\000079\000"\
	"080\000081\000082\000083\000084\000085\000086\000087\000"\
	"088\000089\000090\000091\000092\000093\000094\000095\000"\
	"096\000097\000098\000099\000100\000101\000102\000103\000"\
	"104\000105\000106\000107\000108\000109\000110\000111\000"\
	"112\000113\000114\000115\000116\000117\000118\000119\000"\
	"120\000121\000122\000123\000124\000125\000126\000127\000"\
	"128\000129\000130\000131\000132\000133\000134\000135\000"\
	"136\000137\000138\000139\000140\000141\000142\000143\000"\
	"144\000145\000146\000147\000148\000149\000150\000151\000"\
	"152\000153\000154\000155\000156\000157\000158\000159\000"\
	"160\000161\000162\000163\000164\000165\000166\000167\000"\
	"168\000169\000170\000171\000172\000173\000174\000175\000"\
	"176\000177\000178\000179\000180\000181\000182\000183\000"\
	"184\000185\000186\000187\000188\000189\000190\000191\000"\
	"192\000193\000194\000195\000196\000197\000198\000199\000"\
	"200\000201\000202\000203\000204\000205\000206\000207\000"\
	"208\000209\000210\000211\000212\000213\000214\000215\000"\
	"216\000217\000218\000219\000220\000221\000222\000223\000"\
	"224\000225\000226\000227\000228\000229\000230\000231\000"\
	"232\000233\000234\000235\000236\000237\000238\000239\000"\
	"240\000241\000242\000243\000244\000245\000246\000247\000"\
	"248\000249\000250\000251\000252\000253\000254\000255\000"

# define TR_W				274
# define TR_H				77

# define TR_BLEN			41
# define TR_BLOCK			"▄"

# define TR_BACKGROUND		0
# define TR_FOREGROUND		1

# define TR_ANSI_RGB_BG		"\033[48;2"
# define TR_ANSI_RGB_FG		"\033[38;2"
# define TR_ANSI_RESET		"\033[0m"
# define TR_ANSI_CLEAR		"\033c"
# define TR_ANSI_HOME		"\033[H"
# define TR_ANSI_CURSOR_ON	"\033[?25h"
# define TR_ANSI_CURSOR_OFF	"\033[?25l"

typedef enum e_rgb_colors
{
	TR_RGB_BLACK	= 0x00000000,
	TR_RGB_RED		= 0x00FF0000,
	TR_RGB_GREEN	= 0x0000FF00,
	TR_RGB_BLUE		= 0x000000FF,
	TR_RGB_YELLOW	= TR_RGB_RED | TR_RGB_GREEN,
	TR_RGB_CYAN		= TR_RGB_GREEN | TR_RGB_BLUE,
	TR_RGB_MAGENTA	= TR_RGB_BLUE | TR_RGB_RED,
	TR_RGB_WHITE	= ~TR_RGB_BLACK
}	t_rgb_colors;

typedef uint8_t				*t_screen;

typedef struct s_terminal	t_terminal;

typedef struct s_te_img		t_te_img;

typedef struct s_te_entity	t_entity;

typedef struct s_te_anim	t_te_anim;

typedef struct s_llist
{
	uint32_t		index;
	struct s_llist	*next;
}	t_llist;

/* TERMINAL ***************************************************************** */

struct s_terminal
{
	uint32_t	col;
	uint32_t	row;
	uint32_t	ifps;
	t_chunk		*imgs;
	t_chunk		*entities;
	t_chunk		*anims;
	t_screen	screen;
	t_screen	back;
};

t_terminal
*te_init(void);

void
te_destroy(t_terminal *t);

void
te_loop(t_terminal *t);

void
te_terminal_fps_max(t_terminal *t, uint32_t fps);

t_screen
te_screen_init(void);

void
te_screen_destroy(t_screen screen);

void
te_screen_set_pixel(t_screen screen, uint32_t x, uint32_t y, uint32_t color);

void
te_screen_get_pixel(t_screen screen, uint32_t x, uint32_t y, uint32_t *color);

void
te_screen_put_img(t_screen screen, t_te_img *img, uint32_t x, uint32_t y);

void
te_terminal_screen_shift(t_terminal *t);

uint32_t
te_terminal_img_index(t_terminal *t, t_te_img *img);

void
te_ansi(const char *seq);



/* IMAGES ****************************************************************** */

struct s_te_img
{
	uint32_t	col;
	uint32_t	row;
	uint32_t	x;
	uint32_t	y;
	uint32_t	*data;
};

t_te_img
*te_img_init(t_terminal *t, uint32_t row, uint32_t col);

void
te_img_destroy(t_te_img *img);

void
te_img_set_pixel(t_te_img *img, uint32_t x, uint32_t y, uint32_t color);

void
te_img_get_pixel(t_te_img *img, uint32_t x, uint32_t y, uint32_t *color);

/* ENTITIES **************************************************************** */

struct s_te_entity
{
	uint32_t	x;
	uint32_t	y;
	uint32_t	anim_size;
	uint16_t	anim_state;
	uint16_t	anim_frame;
	uint8_t		anim_ids[32];
	uint64_t	padd[2];
};

t_entity
*te_entity_init(t_terminal *t);

void
te_entity_destroy(t_entity *e);

/* ANIMATIONS ************************************************************** */

struct s_te_anim
{
	uint8_t	frames[32];
};

t_te_anim
*te_anim_init(t_terminal *t, t_entity *e);

void
te_anim_destroy(t_terminal *t, t_te_anim *anim);

/* SCREEN ****************************************************************** */

void
te_str_append(const char *src, char *dst, char **remain);

void
te_str_append_color(uint32_t mode, uint32_t color, char *cursor, char **remain);

#endif
