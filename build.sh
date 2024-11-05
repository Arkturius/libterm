# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    build.sh                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: rgramati <rgramati@student.42angouleme.fr  +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2024/10/04 20:18:14 by rgramati          #+#    #+#              #
#    Updated: 2024/11/01 16:06:01 by rgramati         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

set -xe

make so

clang -o term main.c -L. -Llib/cmem -lterm -lcmem -lm  -I include -I lib/cmem/include -g3 -Wl,-rpath,./:./lib/cmem/
