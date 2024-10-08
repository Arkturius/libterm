// ************************************************************************** //
//                                                                            //
//                                                        :::      ::::::::   //
//   utils.c                                            :+:      :+:    :+:   //
//                                                    +:+ +:+         +:+     //
//   By: rgramati <rgramati@student.42angouleme.fr  +#+  +:+       +#+        //
//                                                +#+#+#+#+#+   +#+           //
//   Created: 2024/10/04 21:05:50 by rgramati          #+#    #+#             //
//   Updated: 2024/10/07 20:51:51 by rgramati         ###   ########.fr       //
//                                                                            //
// ************************************************************************** //

#include <termengine.h>
#include <unistd.h>

void	te_terminal_fps_max(t_terminal *term, uint32_t fps)
{
	float		delta;
	uint32_t	idelta;

	if (fps == 0 || fps > 120)
		return ;
	delta = 1. / fps;
	idelta = delta * 1000000;
	term->ifps = idelta;
}

#ifdef _STDIO_H

/**
* @brief	Writes ansi sequences using printf (only if stdio defined).
*
* @param	(const char *seq)		Ansi sequence.
*/
void	te_ansi(const char *seq)
{
	printf("%s", seq);
}

#else
# ifdef FT_PRINTF_H

/**
* @brief	Writes ansi sequences using ft_printf (only if included)
*
* @param	(const char *)		Ansi sequence.
*/
void	te_ansi(const char *seq)
{
	ft_printf("%s", seq);
}

# else

/**
* @brief	Writes ansi sequences using write (default if no printf included)
*
* @param	(const char *)		Ansi sequence.
*/
void	te_ansi(const char *seq)
{
	const char	*tmp;

	tmp = seq;
	while (*tmp)
		tmp++;
	write(STDIN_FILENO, seq, tmp - seq);
}

# endif	// FT_PRINTF_H
#endif	// _STDIO_H

