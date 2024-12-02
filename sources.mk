# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    sources.mk                                         :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: rgramati <rgramati@student.42angouleme.fr  +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2024/11/27 21:02:15 by rgramati          #+#    #+#              #
#    Updated: 2024/12/02 18:56:46 by rgramati         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

SRC_DIR		?=	src
CORE_DIR	:=	core

TERM		:=	$(CORE_DIR)/terminal
EVENTS		:=	$(CORE_DIR)/events

SRC_TERM	:=	terminal.c		\
				screen.c		\
				draw.c			\
				images.c		\

SRC_EVENTS	:=	events.c		\
				events_funcs.c	\
				hooks.c			\
				timer.c			\

SRC_MISC	:=	utils.c

SRC_TERM	:=	$(addprefix $(TERM)/, $(SRC_TERM))
SRC_EVENTS	:=	$(addprefix $(EVENTS)/, $(SRC_EVENTS))

SRCS		:=	$(SRC_TERM) $(SRC_EVENTS) $(SRC_MISC)
