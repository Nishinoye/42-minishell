NAME    = minishell

CC      = cc
FLAG    = -Wall -Wextra -Werror -g3

DIRLIB  = ./Libft/
FILELIB = libft.a
NAMELFT = $(addprefix $(DIRLIB), $(FILELIB))

SRC_DIR = src/
OBJ_DIR = obj/
INCLUDE = -I ./headers -I ./Libft
HEADER  = headers/minishell.h

# Couleurs
DEF_COLOR   = \033[0;39m
GREEN       = \033[0;92m
YELLOW      = \033[0;93m
MAGENTA     = \033[0;95m

# Fichiers core
FILES   = main quote dollar_env dollar_replace list_utils signals create_token \
          list_token ms_utils ft_env ft_export free array_utils ft_unset ft_pwd \
          ft_cd list_cmd create_cmd cmd_fd here_doc cmd_param exec exec_builtin \
          ft_echo ft_exit find_cmd exec_child signals2

SRCS    = $(addprefix $(SRC_DIR), $(addsuffix .c, $(FILES)))
OBJS    = $(addprefix $(OBJ_DIR), $(addsuffix .o, $(FILES)))

OBJF    = .cache_exits

$(OBJF) :
	@mkdir -p $(OBJ_DIR)

vpath %.c src core/parsing core/execution core/builtins core/utils

all : $(NAME)

$(NAME) : $(OBJS)
	make -C $(DIRLIB)
	$(CC) $(FLAG) $(OBJS) $(NAMELFT) -lreadline $(INCLUDE) -o $(NAME)
	@echo "$(GREEN)Minishell Compiled!$(DEF_COLOR)"

$(OBJ_DIR)%.o : %.c $(HEADER) Makefile | $(OBJF)
	$(CC) $(FLAG) $(INCLUDE) -c $< -o $@

clean :
	@ make clean -sC $(DIRLIB)
	@rm -rf $(OBJ_DIR)
	@rm -rf $(OBJF)
	@echo "$(MAGENTA)Minishell objects cleaned !$(DEF_COLOR)"

fclean : clean
	@ make clean -sC $(DIRLIB)
	@rm -rf $(NAME)
	@echo "$(YELLOW)Minishell cleaned !$(DEF_COLOR)"

re : fclean all
	@echo "$(GREEN)Cleaned and rebuilt !$(DEF_COLOR)"

.PHONY : all clean fclean re
