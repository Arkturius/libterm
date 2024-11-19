// ************************************************************************** //
//                                                                            //
//                                                        :::      ::::::::   //
//   uintmask.c                                         :+:      :+:    :+:   //
//                                                    +:+ +:+         +:+     //
//   By: rgramati <rgramati@student.42angouleme.fr  +#+  +:+       +#+        //
//                                                +#+#+#+#+#+   +#+           //
//   Created: 2024/11/18 02:45:57 by rgramati          #+#    #+#             //
//   Updated: 2024/11/18 16:38:15 by rgramati         ###   ########.fr       //
//                                                                            //
// ************************************************************************** //

#include <termengine.h>

uint64_t	te_uint_mask(uint64_t ptr, uint64_t mask)
{
	ptr |= mask;
	return (ptr);
}

uint64_t	te_uint_unmask(uint64_t ptr, uint64_t mask)
{
	ptr &= ~mask;
	return (ptr);
}

uint64_t	te_uint_testmask(uint64_t ptr, uint64_t mask)
{
	return (ptr & mask);
}

uint64_t	te_uint_andmask(uint64_t ptr, uint64_t mask)
{
	return ((ptr & mask) == mask);
}

