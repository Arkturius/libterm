# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: rgramati <rgramati@student.42angouleme.fr  +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2024/07/05 17:09:56 by rgramati          #+#    #+#              #
#    Updated: 2024/12/02 20:07:01 by rgramati         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

LIBNAME		=	$(notdir $(shell pwd))

SRC_DIR		:=	src
INC_DIR		:=	include
LIB_DIR		:=	lib
OBJ_DIR		:=	build

include			sources.mk
SRCS		:=	$(addprefix $(SRC_DIR)/, $(SRCS))

LIBS		:=	$(if $(wildcard $(LIB_DIR)/*), $(shell find $(LIB_DIR) -mindepth 1 -maxdepth 1 -type d | cut -d/ -f2),)
LIBS 		:=	$(if $(LIBS), $(LIBS),)

LIB_DIRS	:=	$(addprefix $(LIB_DIR)/,$(LIBS))
LDDIRS		:=	$(foreach dir, $(LIB_DIRS),-L$(dir))
LDFLAGS		:=	$(sort $(foreach lib, $(LIB_DIRS),-l$(patsubst lib%,%, $(shell echo $(lib) | cut -d/ -f2)) -Wl,-rpath,$(lib)))

LIB_ALL		:=	$(foreach lib, $(LIBS), $(wildcard $(LIB_DIR)/$(lib)/$(lib).a) $(wildcard $(LIB_DIR)/$(lib)/$(lib).so))
LIB_FILES	:=	$(foreach lib, $(LIBS), $(LIB_DIR)/$(lib)/$(lib).a $(LIB_DIR)/$(lib)/$(lib).so)

LIB_TOBUILD	:=	$(filter-out $(LIB_ALL), $(LIB_FILES))

COPTS_AUTO	:=	$(foreach dir, $(LIB_DIRS),$(wildcard $(dir)/inc) $(wildcard $(dir)/include) $(wildcard $(dir)/includes))
COPTS_NEW	:=	$(INC_DIR) # add other header paths

COPTS_DIRS	:=	$(COPTS_AUTO) $(COPTS_NEW)
COPTS		+=	$(foreach dir, $(COPTS_DIRS),-I$(dir))

OBJS 		:=	$(addprefix $(OBJ_DIR)/, $(SRCS:%.c=%.o))
OBJS_SO		:=	$(addprefix $(OBJ_DIR)/so/, $(SRCS:%.c=%.o))

CC			:=	clang

CFLAGS		:=	-Wall -Wextra -Werror -g3

RM			:=	rm -rf
MKDIR		:=	mkdir -p

MAKE		+=	--no-print-directory

#
# Rules
#

all:				$(LIB_TOBUILD) $(LIBNAME).a $(LIBNAME).so
	@$(MAKE) art

$(LIBNAME).a:		$(LIB_FILES) $(OBJS)
	@ar rcs $@ $^
	@echo " $(GREEN)$(BOLD)$(ITALIC)■$(RESET)  linking	$(GREEN)$(BOLD)$(ITALIC)$(LIBNAME).a$(RESET)"

$(LIBNAME).so:		$(OBJS_SO)
	@$(CC) -shared -o $@ $^ $(COPTS) $(LDDIRS) $(LDFLAGS)
	@echo " $(GREEN)$(BOLD)$(ITALIC)■$(RESET)  linking	$(GREEN)$(BOLD)$(ITALIC)$(LIBNAME).so$(RESET)"

$(OBJ_DIR)/so/%.o:	%.c
	@$(MKDIR) $(@D)
	@echo " $(CYAN)$(BOLD)$(ITALIC)■$(RESET)  compiling	$(GRAY)$(BOLD)$(ITALIC)(PIC)$(notdir $@) from $(GRAY)$(BOLD)$(ITALIC)$(notdir $^)$(RESET)"
	@$(CC) $(CFLAGS) -fPIC -o $@ -c $^ $(COPTS)

$(OBJ_DIR)/%.o: 	%.c
	@$(MKDIR) $(@D)
	@echo " $(CYAN)$(BOLD)$(ITALIC)■$(RESET)  compiling	$(GRAY)$(BOLD)$(ITALIC)$(notdir $@) from $(GRAY)$(BOLD)$(ITALIC)$(notdir $^)$(RESET)"
	@$(CC) $(CFLAGS) -o $@ -c $(COPTS) $^

$(LIB_DIRS)/%.a:
	@$(MAKE) -C $(LIB_DIRS)

$(LIB_DIRS)/%.so:
	@$(MAKE) -C $(LIB_DIRS) $(notdir $@)

clean:
	@if [ -d $(OBJ_DIR) ]; then \
		$(MAKE) cart; \
	fi;
	@if [ -d $(OBJ_DIR) ]; then \
		echo " $(RED)$(BOLD)$(ITALIC)■$(RESET)  deleted	$(RED)$(BOLD)$(ITALIC)$(LIBNAME)/$(OBJ_DIR)$(RESET)"; \
		$(RM) $(OBJ_DIR); \
	fi

fclean:				clean
	@if [ -f "$(LIBNAME).a" ]; then \
		echo " $(RED)$(BOLD)$(ITALIC)■$(RESET)  deleted	$(RED)$(BOLD)$(ITALIC)$(LIBNAME).a$(RESET)"; \
		$(RM) $(LIBNAME).a; \
	fi;
	@if [ -f "$(LIBNAME).so" ]; then \
		echo " $(RED)$(BOLD)$(ITALIC)■$(RESET)  deleted	$(RED)$(BOLD)$(ITALIC)$(LIBNAME).so$(RESET)"; \
		$(RM) $(LIBNAME).so; \
	fi;
	@for DIR in $(LIB_DIRS); \
	do $(MAKE) -C $$DIR fclean; \
	done;

re:					fclean all

art:
	@if [ ! -f .art ]; then \
		TEXT=$(shell echo $(LIBNAME) | jq -sRr @uri); \
		curl -s "https://asciified.thelicato.io/api/v2/ascii?text=$$TEXT&font=Ogre" > .art; \
		if [ $$? -ne 0 ]; then \
			exit 1; \
		fi; \
		echo "\n" >> .art; \
	fi
	@echo -n "$(BOLD)$(CYAN)"
	@cat .art | head -n6
	@echo -n "$(RESET)"

cart:
	@if [ ! -f .cart ]; then \
		TEXT=$(shell echo cleaning... | jq -sRr @uri); \
		curl -s "https://asciified.thelicato.io/api/v2/ascii?text=$$TEXT&font=Ogre" > .cart; \
		if [ $$? -ne 0 ]; then \
			exit 1; \
		fi; \
		echo "\n" >> .cart; \
	fi
	@echo -n "$(BOLD)$(RED)"
	@cat .cart | head -n6
	@echo -n "$(RESET)"

.PHONY:		all clean fclean re
.SILENT:	all clean fclean re

#
# COLORS
# 

BOLD			=	\033[1m
ITALIC			=	\033[3m

RED				=	\033[31m
GREEN			=	\033[32m
CYAN			=	\033[36m
GRAY			=	\033[90m

RESET			=	\033[0m

LINE_CLR		=	\33[2K\r


