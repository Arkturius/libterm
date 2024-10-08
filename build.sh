# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    build.sh                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: rgramati <rgramati@student.42angouleme.fr  +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2024/10/04 20:18:14 by rgramati          #+#    #+#              #
#    Updated: 2024/10/04 20:56:04 by rgramati         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

set -xe

clang -o term main.c libtr.a lib/ft_chunk/libchunk.a -I include -I lib/ft_chunk/include -g3
