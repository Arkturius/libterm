// ************************************************************************** //
//                                                                            //
//                                                        :::      ::::::::   //
//   te_entity.c                                        :+:      :+:    :+:   //
//                                                    +:+ +:+         +:+     //
//   By: rgramati <rgramati@student.42angouleme.fr  +#+  +:+       +#+        //
//                                                +#+#+#+#+#+   +#+           //
//   Created: 2024/10/05 21:11:42 by rgramati          #+#    #+#             //
//   Updated: 2024/10/05 21:17:29 by rgramati         ###   ########.fr       //
//                                                                            //
// ************************************************************************** //

#include <termengine.h>

t_entity	*te_entity_init(t_terminal *t)
{
	t_entity	entity;
	void		*ptr;

	entity = (t_entity){0};
	ptr = ft_chunk_push(t->entities, &entity, sizeof(t_entity));
	return (ptr);
}

void	te_entity_destroy(t_entity *e)
{
	(void)e;
}
