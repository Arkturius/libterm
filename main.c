// ************************************************************************** //
//                                                                            //
//                                                        :::      ::::::::   //
//   main.c                                             :+:      :+:    :+:   //
//                                                    +:+ +:+         +:+     //
//   By: rgramati <rgramati@student.42angouleme.fr  +#+  +:+       +#+        //
//                                                +#+#+#+#+#+   +#+           //
//   Created: 2024/10/02 14:49:44 by rgramati          #+#    #+#             //
//   Updated: 2024/10/17 00:37:08 by rgramati         ###   ########.fr       //
//                                                                            //
// ************************************************************************** //

#include <fcntl.h>
#include <sys/ioctl.h>
#include <unistd.h>
#include <stdint.h>
#include <termengine.h>

#include <stdio.h>
#include <stdlib.h>

void	*my_loop(void *t_ptr)
{
	(void) t_ptr;
	return (NULL);
}

void	*my_b_function(void *t_ptr)
{
	(void) t_ptr;
	dprintf(2, "caca");
	return (NULL);
}

int	main(void)
{
	t_terminal	*t;

	t = te_init();
	if (!t)
		return (1);
	te_terminal_fps_max(t, 120);
	te_terminal_hook(t, TE_KEYDOWN | 'b', my_b_function, t);
	te_terminal_hook(t, TE_LOOP, my_loop, t);
	te_loop(t);
	te_destroy(t);
}
