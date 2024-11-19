// ************************************************************************** //
//                                                                            //
//                                                        :::      ::::::::   //
//   main.c                                             :+:      :+:    :+:   //
//                                                    +:+ +:+         +:+     //
//   By: rgramati <rgramati@student.42angouleme.fr  +#+  +:+       +#+        //
//                                                +#+#+#+#+#+   +#+           //
//   Created: 2024/10/02 14:49:44 by rgramati          #+#    #+#             //
//   Updated: 2024/11/19 19:24:12 by rgramati         ###   ########.fr       //
//                                                                            //
// ************************************************************************** //

#include "src/ecs/ecs.h"
#include <X11/X.h>
#include <stdlib.h>
#include <time.h>

#include <termengine.h>

// TILE EDIT /////
// #include "src/tools/te_tools.h"
// void	te_tool_start(t_terminal *t, t_tool tool)
// {
// 	if (tool == TE_TILE_EDIT)
// 	{
// 		TE_HOOK(t, TE_LOOP, te_tool_etile, t);
// 	}
// }
//////////////////

void	*tetris_loop(void *tetris_ptr);

#include <stdio.h>

void	*dummy(void *t_ptr)
{
	t_terminal	*t;

	t = t_ptr;
	te_screen_draw_square(t, (t_vec2){0 , 0}, (t_vec2) {10, 10}, t->color);

	for (int i = 0; i < 128; i++)
	{
		if (t->key_table[i])
			TE_SET_PIXEL(t, (t_vec2){i, 0}, TE_RGB_GREEN);
		else
			TE_SET_PIXEL(t, (t_vec2){i, 0}, TE_RGB_RED);
	}
	for (int i = 0; i < 5; i++)
	{
		// dprintf(2, "%d ", t->mouse[i]);
		if (t->mouse[i])
			TE_SET_PIXEL(t, (t_vec2){i + 150, 0}, TE_RGB_GREEN);
		else
			TE_SET_PIXEL(t, (t_vec2){i + 150, 0}, TE_RGB_RED);
	}
	// dprintf(2, "\n");
	return (NULL);
}

#define SIZE(X)	printf("Size of %s is [%zu]\n", #X, sizeof(X))

int	main(__attribute__((unused)) int argc, __attribute__((unused)) char **argv)
{

	// SIZE(t_ecs);

	// printf("{%lx, ", (uint64_t)&(((t_ecs *)(NULL))->entities));
	// printf("%lx, ", (uint64_t)&(((t_ecs *)(NULL))->tables));
	// printf("%lx, ", (uint64_t)&(((t_ecs *)(NULL))->comp_data));
	// printf("\033[31;1m%lx,\033[0m ", (uint64_t)&(((t_ecs *)(NULL))->__reserved));
	// printf("%lx, ", (uint64_t)&(((t_ecs *)(NULL))->comp_size));
	// printf("%lx, ", (uint64_t)&(((t_ecs *)(NULL))->components));
	// printf("%lx, ", (uint64_t)&(((t_ecs *)(NULL))->n_components));
	// printf("%lx, ", (uint64_t)&(((t_ecs *)(NULL))->n_entities));
	// printf("%lx", (uint64_t)&(((t_ecs *)(NULL))->free_list));
	// printf("\033[31;1m%lx\033[0m", (uint64_t)&(((t_ecs *)(NULL))->__footer));
	// printf("}\n");

	// SIZE(t_ecs);
	// SIZE(struct s_cm_chunk);
	// SIZE(struct s_cm_htable);
	// SIZE(struct s_comp_table);

	t_ecs	*system;

	system = te_ecs_system_new();

	te_ecs_entity_new(system);
	te_ecs_entity_new(system);
	te_ecs_entity_new(system);
	te_ecs_entity_new(system);
	te_ecs_entity_new(system);

	te_ecs_entity_del(system, 2);
	te_ecs_entity_del(system, 4);

	te_ecs_entity_new(system);
	te_ecs_entity_new(system);

	te_ecs_system_del(system);

	// t_terminal	*t;
	//
	// srand(time(NULL));
	// t = te_init();
	// if (t)
	// {
	// 	te_terminal_fps_max(t, 60);
	// 	TE_HOOK(t, (t_hook_info){.key = TE_LOOP, .type = TE_LOOP}, dummy, t);
	// 	te_loop(t);
	// }
	// te_destroy(t);
}
