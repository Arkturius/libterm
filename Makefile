# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: rgramati <rgramati@student.42angouleme.fr  +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2024/07/05 17:09:56 by rgramati          #+#    #+#              #
#    Updated: 2024/10/08 17:36:38 by rgramati         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

LIBNAME		:=	libtr

SRC_DIR		:=	src

OBJS_DIR	:=	build

LIBDIR		:=	lib

SRCS		:=	term.c		\
				te_str.c	\
				te_screen.c	\
				te_img.c	\
				te_anim.c	\
				utils.c

SRCS		:=	$(addprefix $(SRC_DIR)/, $(SRCS))

OBJS 		:=	$(addprefix $(OBJS_DIR)/, $(SRCS:%.c=%.o))
SOBJS		:=	$(addprefix $(OBJS_DIR)/PIC/, $(SRCS:%.c=%.o))

INCLUDES	:=	include

CC			:=	clang

CFLAGS		:=	-Wall -Wextra -Werror -g3 -O3

COPTS		:=	-I ./$(INCLUDES) -I ./$(LIBDIR)/cmem/$(INCLUDES)

RM			:=	rm -rf

CMEM		:=	cmem

#
# Rules
#

all:		$(CMEM) $(LIBNAME)

$(LIBNAME): $(OBJS)
	@ar rc $@.a $^
	@echo " $(GREEN)$(BOLD)$(ITALIC)■$(RESET)  linking	$(GRAY)$(BOLD)$(ITALIC)$(LIBNAME)$(RESET)"

so:			$(SOBJS)
	@$(CC) -shared -o $(LIBNAME).so $^

$(OBJS_DIR)/PIC/%.o: %.c
	@mkdir -p $(@D)
	@echo " $(CYAN)$(BOLD)$(ITALIC)■$(RESET)  compiling	$(GRAY)$(BOLD)$(ITALIC)(PIC)$(notdir $@) from $(GRAY)$(BOLD)$(ITALIC)$(notdir $^)$(RESET)"
	@$(CC) $(CFLAGS) -fPIC $(COPTS) -o $@ -c $^

$(OBJS_DIR)/%.o: %.c
	@mkdir -p $(@D)
	@echo " $(CYAN)$(BOLD)$(ITALIC)■$(RESET)  compiling	$(GRAY)$(BOLD)$(ITALIC)$(notdir $@) from $(GRAY)$(BOLD)$(ITALIC)$(notdir $^)$(RESET)"
	@$(CC) $(CFLAGS) $(COPTS) -o $@ -c $^

$(CMEM):
	@make -C $(LIBDIR)/$@ --no-print-directory
	@make -C $(LIBDIR)/$@ --no-print-directory so

clean:
	@if [ -d $(OBJS_DIR) ]; then \
		echo " $(RED)$(BOLD)$(ITALIC)■$(RESET)  deleted	$(GRAY)$(BOLD)$(ITALIC)$(OBJS_DIR)$(RESET)"; \
		$(RM) $(OBJS_DIR); \
	fi

fclean:		clean
	@if [ -f "$(LIBNAME).a" ]; then \
		echo " $(RED)$(BOLD)$(ITALIC)■$(RESET)  deleted	$(GRAY)$(BOLD)$(ITALIC)$(LIBNAME).a$(RESET)"; \
		$(RM) $(LIBNAME).a; \
	fi;
	@if [ -f "$(LIBNAME).so" ]; then \
		echo " $(RED)$(BOLD)$(ITALIC)■$(RESET)  deleted	$(GRAY)$(BOLD)$(ITALIC)$(LIBNAME).so$(RESET)"; \
		$(RM) $(LIBNAME).so; \
	fi;

re:			fclean all

.PHONY:		all clean fclean re

#
# Misc
# 

BOLD			=	\033[1m
ITALIC			=	\033[3m

BLACK			=	\033[30m
RED				=	\033[31m
GREEN			=	\033[32m
YELLOW			=	\033[33m
BLUE			=	\033[34m
MAGENTA			=	\033[35m
CYAN			=	\033[36m
WHITE			=	\033[37m
GRAY			=	\033[90m

RESET			=	\033[0m

LINE_CLR		=	\33[2K\r
