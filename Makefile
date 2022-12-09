# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    MAKEFILE                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: marius <marius@student.42.fr>              +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2022/10/26 10:11:17 by marius            #+#    #+#              #
#    Updated: 2022/12/09 10:31:47 by marius           ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME		=	lemin
LIBFT		=	libft/libftprintf.a
FLAGS		=	-Wall -Wextra -Werror -I includes -I libft/includes -I libft/libft

SRC		=	ant_helper.c breadthsearch.c generate.c ft_free.c read_file.c get_links.c get_room.c gnl_store.c main.c new_room.c optimize_ants.c path_helper.c queue_helper.c save_path.c send_ants.c solve.c weight_helper.c

LIBFT_DIR	=	libft/
SRC_DIR		=	srcs/

CHK_SRCS	=	$(addprefix $(SRC_DIR), $(SRC))

OBJ_DIR		=	obj/
OBJS		= 	$(SRC_OBJS)
SRC_OBJS	=	$(patsubst %, $(OBJ_DIR)%, $(SRC:.c=.o))

all: $(NAME)

$(NAME): $(LIBFT) $(OBJ_DIR) $(SRC_OBJS)
	@gcc $(FLAGS) $(LIBFT) $(SRC_OBJS) -o $(NAME)
	@printf "Compilation complete.\n"
	
$(LIBFT):
	@make -C $(LIBFT_DIR)

$(OBJ_DIR):
	@mkdir -p $(OBJ_DIR)

$(OBJ_DIR)%.o: $(SRC_DIR)%.c
	@gcc $(FLAGS) -o $@ -c $<
	@printf "Lem-in compiled.\n"

clean:
	@make -C $(LIBFT_DIR) clean
	@rm -rf $(OBJS) $(SURPL_O)

fclean: clean
	@make -C $(LIBFT_DIR) fclean
	@rm -rf $(NAME) *.out

re: fclean all