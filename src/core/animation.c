// ************************************************************************** //
//                                                                            //
//                                                        :::      ::::::::   //
//   animation.c                                        :+:      :+:    :+:   //
//                                                    +:+ +:+         +:+     //
//   By: rgramati <rgramati@student.42angouleme.fr  +#+  +:+       +#+        //
//                                                +#+#+#+#+#+   +#+           //
//   Created: 2024/10/05 20:24:21 by rgramati          #+#    #+#             //
//   Updated: 2024/10/27 19:54:41 by rgramati         ###   ########.fr       //
//                                                                            //
// ************************************************************************** //

// TODO: DO SOMETHING HERE PLEASE
//
//
//
//
//
//
//
//                  PLEASE
//
//
//
//
//
//
//
//

#include <errno.h>
#include <fcntl.h>
#include <stdint.h>
#include <stdlib.h>
#include <dirent.h>
#include <stdio.h>
#include <string.h>

#include <termengine.h>

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
