// ************************************************************************** //
//                                                                            //
//                                                        :::      ::::::::   //
//   main.c                                             :+:      :+:    :+:   //
//                                                    +:+ +:+         +:+     //
//   By: rgramati <rgramati@student.42angouleme.fr  +#+  +:+       +#+        //
//                                                +#+#+#+#+#+   +#+           //
//   Created: 2024/10/02 14:49:44 by rgramati          #+#    #+#             //
//   Updated: 2024/10/07 21:14:39 by rgramati         ###   ########.fr       //
//                                                                            //
// ************************************************************************** //

#include <fcntl.h>
#include <unistd.h>
#include <stdint.h>

#include <termengine.h>

int	main(void)
{
	t_terminal	*t;

	t = te_init();
	if (!t)
		return (1);
	te_terminal_fps_max(t, 60);
	te_loop(t);
	te_destroy(t);
}
