// ************************************************************************** //
//                                                                            //
//                                                        :::      ::::::::   //
//   main.c                                             :+:      :+:    :+:   //
//                                                    +:+ +:+         +:+     //
//   By: rgramati <rgramati@student.42angouleme.fr  +#+  +:+       +#+        //
//                                                +#+#+#+#+#+   +#+           //
//   Created: 2024/10/02 14:49:44 by rgramati          #+#    #+#             //
//   Updated: 2024/10/23 02:30:49 by rgramati         ###   ########.fr       //
//                                                                            //
// ************************************************************************** //

#include <asm-generic/ioctls.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/ioctl.h>
#include <termengine.h>
#include <unistd.h>

void	*my_loop(void *t_ptr)
{
	(void) t_ptr;
	return (NULL);
}

int	main(void)
{
	t_terminal	*t;

	t = te_init();
	if (t)
	{
		te_tileset_init(t, "terrain", "assets/TS18_stonehenge.bmp", 20);
		

		t_te_tile_img	*platform = te_tileset_img_init(t, "terrain", 2, 2, 2, 4, 34, 36);
		
		te_screen_put_tile_img(t, platform, TE_W / 2 - 40, (TE_H * 2) - 80);
			
		te_terminal_fps_max(t, 60);
		te_terminal_hook(t, TE_LOOP, my_loop, t);
		te_loop(t);
	}
	te_destroy(t);
}
