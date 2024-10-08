// ************************************************************************** //
//                                                                            //
//                                                        :::      ::::::::   //
//   te_anim.c                                          :+:      :+:    :+:   //
//                                                    +:+ +:+         +:+     //
//   By: rgramati <rgramati@student.42angouleme.fr  +#+  +:+       +#+        //
//                                                +#+#+#+#+#+   +#+           //
//   Created: 2024/10/05 20:24:21 by rgramati          #+#    #+#             //
//   Updated: 2024/10/05 21:25:32 by rgramati         ###   ########.fr       //
//                                                                            //
// ************************************************************************** //

#include <termengine.h>

t_te_anim	*te_anim_init(t_terminal *t, t_entity *e)
{
	void		*ptr;
	t_te_anim	anim;

	assert(e->anim_size < 32 && t->anims->size < t->anims->capacity);
	ptr = NULL;
	anim = (t_te_anim){0};
	if (e->anim_size < 32)
	{
		// TODO: actually manage more than this size of animations but its good for now
		e->anim_ids[e->anim_size++] = t->anims->size;
		ptr = ft_chunk_push(t->anims, &anim, sizeof(t_te_anim));
	}
	return(ptr);
}

void	te_anim_destroy(t_terminal *t, t_te_anim *anim)
{
	(void)anim;
	(void)t;
	// TODO : implement chunk free list to make this more convenient (viable for entity destroy too)
}

void
te_anim_push_img(t_te_anim *anim, uint32_t index);

t_te_img
*te_anim_next_img(t_te_anim *anim);
