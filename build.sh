# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    build.sh                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: rgramati <rgramati@student.42angouleme.fr  +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2024/10/04 20:18:14 by rgramati          #+#    #+#              #
#    Updated: 2024/11/18 22:09:29 by rgramati         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

set -xe

make so

SRCS="main.c "

if [ $# -eq 1 ]
then
	if [ $@ = "tetris" ]
	then
		SRCS=$SRCS"tetris.c "
	fi
fi

clang -Wall -Wextra -O3 $SRCS  -o te_test -L. -Llib/cmem -lterm -lcmem -lm -lX11  -I include -I lib/cmem/include -g3 -Wl,-rpath,./:./lib/cmem/
