// ************************************************************************** //
//                                                                            //
//                                                        :::      ::::::::   //
//   timer.c                                            :+:      :+:    :+:   //
//                                                    +:+ +:+         +:+     //
//   By: rgramati <rgramati@student.42angouleme.fr  +#+  +:+       +#+        //
//                                                +#+#+#+#+#+   +#+           //
//   Created: 2024/11/18 01:55:26 by rgramati          #+#    #+#             //
//   Updated: 2024/11/18 17:03:10 by rgramati         ###   ########.fr       //
//                                                                            //
// ************************************************************************** //

#include <bits/time.h>
#include <stdint.h>
#include <time.h>

#include <termengine.h>

void	te_usleep(uint32_t micro)
{
	struct timespec	start;
	struct timespec	end;
	uint64_t		passed;

	clock_gettime(CLOCK_MONOTONIC, &start);
	while (1)
	{
		clock_gettime(CLOCK_MONOTONIC, &end);
		passed = (end.tv_sec - start.tv_sec) * 1000 +
			(end.tv_nsec - start.tv_nsec) / 1000;
		if (passed >= micro)
			break ;
		nanosleep(&(struct timespec){.tv_nsec = 10 * 1e5}, NULL);
	}
}

void	te_delta_time(t_terminal *t)
{
    clock_gettime(CLOCK_MONOTONIC, &t->time.curr);
    t->time.delta_t = (t->time.curr.tv_sec - t->time.past.tv_sec) +
		(t->time.curr.tv_nsec - t->time.past.tv_nsec) / 1e9;
    t->time.past = t->time.curr;
}
