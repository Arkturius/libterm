// ************************************************************************** //
//                                                                            //
//                                                        :::      ::::::::   //
//   ptrmask.c                                          :+:      :+:    :+:   //
//                                                    +:+ +:+         +:+     //
//   By: rgramati <rgramati@student.42angouleme.fr  +#+  +:+       +#+        //
//                                                +#+#+#+#+#+   +#+           //
//   Created: 2024/11/18 02:35:03 by rgramati          #+#    #+#             //
//   Updated: 2024/11/18 03:05:28 by rgramati         ###   ########.fr       //
//                                                                            //
// ************************************************************************** //

#include <termengine.h>

void	*te_ptr_mask(void *ptr, uint64_t mask)
{
	uint64_t	tmp;

	tmp = (uint64_t)ptr;
	tmp |= mask;
	return ((void *)tmp);
}

void	*te_ptr_unmask(void *ptr, uint64_t mask)
{
	uint64_t	tmp;

	tmp = (uint64_t)ptr;
	tmp &= ~mask;
	return ((void *)tmp);
}

void	*te_ptr_testmask(void *ptr, uint64_t mask)
{
	uint64_t	tmp;

	tmp = (uint64_t)ptr;
	return ((void *)(tmp & mask));
}

void	*te_ptr_andmask(void *ptr, uint64_t mask)
{
	uint64_t	tmp;

	tmp = (uint64_t)ptr;
	return ((void *)(uint64_t)((tmp & mask) == mask));
}

