// ************************************************************************** //
//                                                                            //
//                                                        :::      ::::::::   //
//   ecs.h                                              :+:      :+:    :+:   //
//                                                    +:+ +:+         +:+     //
//   By: rgramati <rgramati@student.42angouleme.fr  +#+  +:+       +#+        //
//                                                +#+#+#+#+#+   +#+           //
//   Created: 2024/11/17 19:03:11 by rgramati          #+#    #+#             //
//   Updated: 2024/11/19 17:43:28 by rgramati         ###   ########.fr       //
//                                                                            //
// ************************************************************************** //

#ifndef TE_ECS_H
# define TE_ECS_H

# include <termengine.h>

# define TE_PAGE_SIZE				0x1000
# define TE_ECS_SPACE				0xFFF0000

# define TE_ECS_ENTITY_CAP			524288
# define TE_ECS_ENTITY_USED_MASK	0x4000000000000000
# define TE_ECS_ENTITY_FREE_MASK	0x8000000000000000

# define TE_ECS_COMPONENT_CAP		62

typedef struct s_ecs_component_transform
{
	t_fvec4	position;
	t_fvec4	rotation;
}	t_ecsc_transform;

enum e_component
{
	TE_COMPONENT_TRANSFORM	= 0,
	TE_COMPONENT_DISPLAY	= 1,
	TE_COMPONENT_INTERACT	= 2,
	TE_COMPONENT_LAST		= 8
};

typedef uint64_t		t_entity;
typedef uint64_t		t_component;
typedef uint64_t		t_signature;
typedef struct s_ecs	t_ecs;

typedef struct s_cm_chunk	t_comp_data[16];

typedef uint8_t				t_padding;

struct s_comp_table
{
	uint32_t			e_to_c[TE_ECS_ENTITY_CAP];
	uint32_t			c_to_e[TE_ECS_ENTITY_CAP];
};

// offsets = 
// {
//		0000000,
//		0400000,
//		fc00000,
//			one page | [0x1000] of padding
//		ffe1000,
//		ffe11f0,
//		ffe1210,
//		ffe1218,
//		ffe1220,
//			last page | [0xedd8] of padding (unused space)
// } 
struct s_ecs
{
	t_signature			entities[TE_ECS_ENTITY_CAP];
	struct s_comp_table	tables[TE_ECS_COMPONENT_CAP];
	t_comp_data			comp_data[TE_ECS_COMPONENT_CAP];
	t_padding			__reserved[TE_PAGE_SIZE];
	uint64_t			comp_size[TE_ECS_COMPONENT_CAP];
	struct s_cm_htable	components;
	uint64_t			n_components;
	uint64_t			n_entities;
	struct s_flist		*free_list;
	t_padding			__footer[0xedd8];
};

t_ecs
*te_ecs_system_new(void);

void
te_ecs_system_del(t_ecs *ecs);

void
te_ecs_entity_new(t_ecs *ecs);

void
te_ecs_entity_del(t_ecs *ecs, uint64_t id);

void
te_ecs_entity_component_add(t_ecs *ecs, t_entity id, t_component comp);

void
te_ecs_entity_component_remove(t_ecs *ecs, t_entity id, t_component comp);

void
te_ecs_component_new(t_ecs *ecs, const char *name, uint32_t size);

void
te_ecs_component_del(t_ecs *ecs, const char *name);

#endif	// TE_ECS_H
