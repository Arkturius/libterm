// ************************************************************************** //
//                                                                            //
//                                                        :::      ::::::::   //
//   entity.c                                           :+:      :+:    :+:   //
//                                                    +:+ +:+         +:+     //
//   By: rgramati <rgramati@student.42angouleme.fr  +#+  +:+       +#+        //
//                                                +#+#+#+#+#+   +#+           //
//   Created: 2024/11/18 12:59:25 by rgramati          #+#    #+#             //
//   Updated: 2024/11/19 17:39:25 by rgramati         ###   ########.fr       //
//                                                                            //
// ************************************************************************** //

#include "ecs.h"

void	te_ecs_entity_new(t_ecs *ecs)
{
	void	*ptr;

	if (ecs)
	{
		ptr = ecs->free_list;
		if (ptr)
			ecs->free_list = te_ptr_unmask(
				((struct s_flist *)ptr)->next,
				TE_ECS_ENTITY_FREE_MASK
			);
		else
			ptr = &ecs->entities[ecs->n_entities];
		*(uint64_t *)ptr = TE_ECS_ENTITY_USED_MASK;
		ecs->n_entities++;
	}
}

void	te_ecs_entity_del(t_ecs *ecs, uint64_t id)
{
	void		*ptr;
	t_signature	sign;

	if (ecs)
	{
		ptr = &ecs->entities[id];
		sign = *(t_signature *)ptr;
		if (te_uint_testmask(sign, TE_ECS_ENTITY_FREE_MASK))
			return ;
		if (!te_uint_testmask(sign, TE_ECS_ENTITY_USED_MASK))
			return ;
		((struct s_flist *)ptr)->next = te_ptr_mask(ecs->free_list, TE_ECS_ENTITY_FREE_MASK);
		ecs->free_list = ptr;
		ecs->n_entities--;
	}
}

// void	te_ecs_entity_component_add(t_ecs *ecs, t_entity id, t_component comp)
// {
// 	t_cm_chunk	*batch;
//
// 	if (comp >= ecs->n_components)
// 		return ;
//
// }
//
// void
// te_ecs_entity_component_remove(t_ecs *ecs, t_entity id, const char *name);
//
