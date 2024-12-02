# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    build.sh                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: rgramati <rgramati@student.42angouleme.fr  +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2024/10/04 20:18:14 by rgramati          #+#    #+#              #
#    Updated: 2024/12/02 18:52:53 by rgramati         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

set -xe

clang -Wall -Wextra main.c -o te_test -Ilib/libcmem/include -Iinclude -Llib/libcmem -L. -lcmem -lterm -lm -lX11 -g3 -Wl,-rpath=.:lib/libcmem
