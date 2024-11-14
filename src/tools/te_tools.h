// ************************************************************************** //
//                                                                            //
//                                                        :::      ::::::::   //
//   te_tools.h                                         :+:      :+:    :+:   //
//                                                    +:+ +:+         +:+     //
//   By: rgramati <rgramati@student.42angouleme.fr  +#+  +:+       +#+        //
//                                                +#+#+#+#+#+   +#+           //
//   Created: 2024/10/23 23:30:17 by rgramati          #+#    #+#             //
//   Updated: 2024/11/13 19:01:23 by rgramati         ###   ########.fr       //
//                                                                            //
// ************************************************************************** //

#ifndef TE_TOOLS_H
# define TE_TOOLS_H

# include <termengine.h>

typedef enum e_tool
{
	TE_TILE_EDIT,
}	t_tool;

enum e_tool_flags
{
	TE_ETILE_MODE_SELECT = 1,
	TE_ETILE_MODE_EDITOR = 1 << 1,
	TE_ETILE_MODE_SWITCH = 1 << 2,
	TE_ETILE_MODE_BRUSH = 1 << 3,
	TE_ETILE_MODE_ERASE = 1 << 4,
};

typedef struct s_tool_data
{
	t_terminal	*t;
	t_tool		tool;
	uint32_t	flags;
}	t_tool_data;

void	*te_tool_etile(void *t_ptr);

#endif
