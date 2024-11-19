// ************************************************************************** //
//                                                                            //
//                                                        :::      ::::::::   //
//   ecs.c                                              :+:      :+:    :+:   //
//                                                    +:+ +:+         +:+     //
//   By: rgramati <rgramati@student.42angouleme.fr  +#+  +:+       +#+        //
//                                                +#+#+#+#+#+   +#+           //
//   Created: 2024/11/17 22:18:20 by rgramati          #+#    #+#             //
//   Updated: 2024/11/19 17:39:40 by rgramati         ###   ########.fr       //
//                                                                            //
// ************************************************************************** //

#include <stdint.h>
#include <stdlib.h>
#include <sys/mman.h>

#include "ecs.h"

static void	te_ecs_system_components(t_ecs *ecs)
{
	uint32_t	i;
	t_comp_data	*comps;
	t_cm_chunk	*head;

	i = 0;
	comps = &ecs->comp_data[0];
	while (i < ecs->n_components)
	{
		head = &comps[i];
		head = cm_chunk_init(ecs->comp_size[i], head, 16);
		if (!head)
			break ;
		i++;
	}
}

t_ecs	*te_ecs_system_new(void)
{
	static uint64_t	sizes[TE_COMPONENT_LAST] = {sizeof(t_ecsc_transform), 0};
	t_ecs			*ecs;
	void			*ptr;

	if (TE_ECS_SPACE & 0xFFF)
		return (NULL);
	ecs = mmap(NULL, TE_ECS_SPACE, PROT_NONE, MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);
	if (ecs)
	{
		mprotect(ecs, TE_ECS_SPACE, PROT_WRITE | PROT_READ);
		cm_memset(ecs, 0, TE_ECS_SPACE);
		mprotect(&ecs->__reserved, TE_PAGE_SIZE, PROT_NONE);
		ecs->n_components = TE_COMPONENT_LAST;
		cm_memcpy(&ecs->comp_size, sizes, TE_COMPONENT_LAST * sizeof(uint64_t));
		ptr = cm_htable_init(64, &ecs->components);
		if (!ptr)
			te_ecs_system_del(ecs);
		te_ecs_system_components(ecs);
	}
	return (ecs);
}

void	te_ecs_system_del(t_ecs *ecs)
{
	t_cm_chunk	*chunk;
	uint32_t	i;
	uint32_t	j;

	if (ecs)
	{
		i = 0;
		while (i < 64)
		{
			chunk = cm_htable_at(&ecs->components, i);
			chunk = cm_htable_get_data(chunk);
			if (chunk)
			{
				j = -1;
				while (chunk && ++j < 16)
					chunk = cm_chunk_next(chunk);
			}
			if (chunk)
				cm_chunk_clear(chunk, CM_CLEAR_FREE);
			i++;
		}
		cm_htable_clear(&ecs->components, CM_CLEAR_FREE | CM_CLEAR_STATIC);
		munmap(ecs, TE_ECS_SPACE);
	}
}
