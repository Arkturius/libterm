// ************************************************************************** //
//                                                                            //
//                                                        :::      ::::::::   //
//   main.c                                             :+:      :+:    :+:   //
//                                                    +:+ +:+         +:+     //
//   By: rgramati <rgramati@student.42angouleme.fr  +#+  +:+       +#+        //
//                                                +#+#+#+#+#+   +#+           //
//   Created: 2024/10/02 14:49:44 by rgramati          #+#    #+#             //
//   Updated: 2024/10/26 18:35:53 by rgramati         ###   ########.fr       //
//                                                                            //
// ************************************************************************** //

#include <limits.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/ioctl.h>
#include <unistd.h>

#include <termengine.h>
#include "src/tools/te_tools.h"

void	*my_loop(void *t_ptr)
{
	(void) t_ptr;
	return (NULL);
}

void	te_tool_start(t_terminal *t, t_tool tool)
{
	if (tool == TE_TILE_EDIT)
	{
		TE_HOOK(t, TE_LOOP, te_tool_etile, t);
	}
}

int	main(int argc, char **argv)
{
	t_terminal	*t;

	if (argc != 3)
		return (1);

	t = te_init();
	if (t)
	{
		// te_tileset_init(t, "terrain", "tileset.bmp", 16);
		te_tileset_init(t, "terrain", argv[1], strtoll(argv[2], NULL, 10));

		// te_terminal_fps_max(t, 120);
		te_tool_start(t, TE_TILE_EDIT);
		// TE_HOOK(t, TE_LOOP, my_loop, t);
		te_loop(t);
	}
	te_destroy(t);
}
