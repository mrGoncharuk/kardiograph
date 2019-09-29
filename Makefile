# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: mhonchar <mhonchar@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2019/05/15 19:28:21 by mhonchar          #+#    #+#              #
#    Updated: 2019/09/17 17:21:27 by mhonchar         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = KardioGraph
CIMGUI_NAME = cimgui.so
SRC_DIR = src/
OBJ_DIR = obj/
INC_DIR = includes/
FRM_DIR = frameworks
CIMGUI_DIR = cimgui/
IMGUI_DIR = cimgui/imgui/
IMPL_DIR = cimgui/impl
CIMGUI_LIB = 	$(addprefix $(CIMGUI_DIR), $(CIMGUI_NAME))



SRC_FILES =		main.c


HEADERS = 		$(INC_DIR)kardiograph.h

OBJS_GUI = 	imgui_impl_sdl.o \
			imgui_impl_opengl3.o

SRC = $(addprefix $(SRC_DIR), $(notdir $(SRC_FILES)))
OBJ = 	$(addprefix $(OBJ_DIR), $(notdir $(SRC_FILES:.c=.o))) \
		$(addprefix $(OBJ_DIR), $(OBJS_GUI))



CC = gcc
CFLAGS = -Wall 
INC = 		-I $(INC_DIR) \
			-I $(CIMGUI_DIR) \
			-I $(IMPL_DIR)


LIBS_LINK = -l SDL2 \
			-l SDL2_image \
			-lm \
			-lglut -lGLU -lGL -lGLEW \
			$(CIMGUI_LIB) \
			-lstdc++
			

SDL_IMPL_CFLAGS = -I$(CIMGUI_DIR) -I$(IMGUI_DIR) -I$(IMPL_DIR) -DIMGUI_IMPL_API="extern \"C\""
OPENGL3_IMPL_CFLAGS_GLEW = -I$(CIMGUI_DIR) -I$(IMGUI_DIR) -I$(IMPL_DIR) -DIMGUI_IMPL_API="extern \"C\"" -DIMGUI_IMPL_OPENGL_LOADER_GLEW


C_RED = \033[31m
C_GREEN = \033[32m
C_MAGENTA = \033[35m
C_NONE = \033[0m

all: $(NAME)

$(NAME): $(CIMGUI_NAME) $(HEADERS) gui $(OBJ_DIR) $(OBJ)
	@$(CC) $(CFLAGS) $(OBJ) $(LIBS_LINK) -o $(NAME)
	@printf "$(C_MAGENTA)$(NAME):$(C_NONE) %-25s$(C_GREEN)[done]$(C_NONE)\n" $@

$(OBJ_DIR):
	@mkdir obj
	@printf "$(C_MAGENTA)$(NAME):$(C_NONE) %-25s$(C_GREEN)[done]$(C_NONE)\n" $@

$(OBJ_DIR)%.o: $(SRC_DIR)%.c $(HEADERS)
	@$(CC) $(CFLAGS) -DIMGUI_IMPL_OPENGL_LOADER_GLEW -DIMGUI_IMPL_API="" -c $(INC) $< -o $@
	@printf "$(C_MAGENTA)$(NAME):$(C_NONE) %-25s$(C_GREEN)[done]$(C_NONE)\n" $@

$(CIMGUI_NAME):
	@make -C $(CIMGUI_DIR)
	@printf "$(C_MAGENTA)$(NAME):$(C_NONE) %-25s$(C_GREE)[done]$(C_NONE)\n" $@

gui: cimgui imgui_impl_sdl.o imgui_impl_opengl3.o

imgui_impl_sdl.o: $(IMPL_DIR)/imgui_impl_sdl.cpp $(IMPL_DIR)/imgui_impl_sdl.h
	@g++ $(SDL_IMPL_CFLAGS) -c $< -o $(OBJ_DIR)$@
	@printf "$(C_MAGENTA)$(NAME):$(C_NONE) %-25s$(C_GREE)[done]$(C_NONE)\n" $@

imgui_impl_opengl3.o: $(IMPL_DIR)/imgui_impl_opengl3.cpp $(IMPL_DIR)/imgui_impl_opengl3.h
	@g++ $(OPENGL3_IMPL_CFLAGS_GLEW) -c $< -o $(OBJ_DIR)/imgui_impl_opengl3.o
	@printf "$(C_MAGENTA)$(NAME):$(C_NONE) %-25s$(C_GREE)[done]$(C_NONE)\n" $@

clean:
	@rm -rf $(OBJ_DIR)*
	@make clean -C $(CIMGUI_DIR)
	@printf "$(C_MAGENTA)$(NAME):$(C_NONE) %-25s$(C_RED)[done]$(C_NONE)\n" $@

fclean: clean
	@rm -rf $(NAME)
	@make fclean -C $(CIMGUI_DIR)
	@printf "$(C_MAGENTA)$(NAME):$(C_NONE) %-25s$(C_RED)[done]$(C_NONE)\n" $@

re: fclean all

norm:
	@norminette $(SRC) $(HEADERS)



