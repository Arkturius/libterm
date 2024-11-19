// ************************************************************************** //
//                                                                            //
//                                                        :::      ::::::::   //
//   utils.c                                            :+:      :+:    :+:   //
//                                                    +:+ +:+         +:+     //
//   By: rgramati <rgramati@student.42angouleme.fr  +#+  +:+       +#+        //
//                                                +#+#+#+#+#+   +#+           //
//   Created: 2024/10/04 21:05:50 by rgramati          #+#    #+#             //
//   Updated: 2024/11/18 01:59:56 by rgramati         ###   ########.fr       //
//                                                                            //
// ************************************************************************** //

#include <stdint.h>
#include <sys/time.h>
#include <unistd.h>
#include <errno.h>
#include <fcntl.h>
#include <stdint.h>
#include <stdlib.h>
#include <dirent.h>
#include <stdio.h>
#include <string.h>

#include <termengine.h>

void	te_terminal_fps_max(t_terminal *term, uint32_t fps)
{
	if (fps == 0 || fps > 120)
		return ;
	term->fps = fps;
}

void	te_ansi(const char *seq)
{
	const char	*tmp;

	tmp = seq;
	while (*tmp)
		tmp++;
	write(STDIN_FILENO, seq, tmp - seq);
}

void	te_str_append(const char *src, char *dst, char **remain)
{
	const char	*tmp;

	tmp = src;
	while (*tmp)
		*dst++ = *tmp++;
	if (remain)
		*remain = (char *)dst;
}

char	*tr_color(uint32_t color)
{
	static char	buffer[TE_BLEN] = {0};
	uint32_t	comp;

	color &= ~(0xFF << 24);
	te_str_append(";000;000;000m", buffer, NULL);
	comp = (color & TE_RGB_RED) >> 14;
	te_str_append(&TE_COLOR_TABLE[comp], &buffer[1], NULL);
	comp = (color & TE_RGB_GREEN) >> 6;
	te_str_append(&TE_COLOR_TABLE[comp], &buffer[5], NULL);
	comp = (color & TE_RGB_BLUE) << 2;
	te_str_append(&TE_COLOR_TABLE[comp], &buffer[9], NULL);
	return (buffer);
}

void	te_str_append_color(uint32_t mode, uint32_t color, char *cursor, char **remain)
{
	const char	*prefix;

	prefix = TE_ANSI_RGB_BG;
	if (mode)
		prefix = TE_ANSI_RGB_FG;
	te_str_append(prefix, cursor, &cursor);
	te_str_append(tr_color(color), cursor, &cursor);
	if (remain)
		*remain = cursor;
}

void	*te_dir_holder(void *dir, uint32_t reset)
{
	static void	*save_dir = NULL;

	if (reset)
		save_dir = NULL;
	else if (dir)
		save_dir = dir;
	return (save_dir);
}

void	te_dir_open(const char *dirname)
{
	DIR	*current_dir;
	
	current_dir = opendir(dirname);
	if (!current_dir)
	{
		printf("[%s] %d : %s\n", __func__, errno, strerror(errno));
		return ;
	}
	te_dir_holder(current_dir, 0);
}

void	te_dir_close(void)
{
	DIR	*current_dir;

	current_dir = te_dir_holder(NULL, 0);
	if (!current_dir)
	{
		printf("[%s] %d : %s\n", __func__, errno, strerror(errno));
		return ;
	}
	closedir(current_dir);
	te_dir_holder(NULL, 1);
}

const char	*te_dir_next_file(void)
{
	DIR				*current_dir;
	struct dirent	*entry;
	static char		filename[256];

	current_dir = te_dir_holder(NULL, 0);
	if (!current_dir)
		return (NULL);
	entry = readdir(current_dir);
	if (!entry)
		return (NULL);
	while (*(entry->d_name) == '.')
		entry = readdir(current_dir);
	cm_memcpy(&filename, entry->d_name, 256);
	return ((const char *)&filename);
}
