MAKEFLAGS	+=	--quiet

WHT	= \033[0;37m
BLK	= \033[0;30m
RED	= \033[0;31m
YEL	= \033[0;33m
BLU	= \033[0;34m
GRN	= \033[0;32m

NAME	=	jackCompiler

LIBFT	=	libft/libft

HEADERS	=	headers

DIR_S	=	srcs

DIR_O	=	obj

SRCS	=	$(shell find $(DIR_S) -name *.c)

OBJS	=	$(SRCS:%.c=$(DIR_O)/%.o)

SUB_DIR_O := $(shell find $(DIR_S) -type d)
SUB_DIR_O := $(SUB_DIR_O:%=$(DIR_O)/%)

DEPS	=	$(shell find $(HEADERS) -name *.h)

NAME	=	jackCompiler

CC		=	gcc

CFLAGS	=	-Wall -Wextra -Werror

INCLUDES	= -I $(HEADERS) -I $(LIBFT)

LIBS	=	-L./$(LIBFT) -lft -lreadline

RM		=	rm -f

$(DIR_O)/%.o: %.c
			@mkdir -p $(DIR_O)
			@mkdir -p $(SUB_DIR_O)
			$(CC) $(CFLAGS) $(INCLUDES) -o $@ -c $<

$(NAME):	$(DEPS) $(OBJS)
			@make $(FAST)bonus -C $(LIBFT)
			$(CC) $(CFLAGS) $(INCLUDES) -o $(NAME) $(OBJS) $(LIBS)
			printf "$(WHT)[$(GRN)$(NAME) COMPILED$(WHT)]\n"

all:		$(NAME)

bonus:		$(DEPS) $(BONUS_OBJS)
			@make $(FAST)bonus -C $(LIBFT)
			$(CC) $(CFLAGS) $(INCLUDES) -o $(NAME) $(BONUS_OBJS) $(LIBS)
			printf "$(WHT)[$(GRN)$(NAME) BONUS COMPILED$(WHT)]\n"

miniclean:
			$(RM) $(OBJS)
			$(RM) $(BONUS_OBJS)
			printf "$(WHT)[$(YEL)$(NAME) OBJS REMOVED$(WHT)]\n"

clean:		miniclean
			@make clean -C $(LIBFT)

fclean:		miniclean
			$(RM) $(NAME)
			@make fclean -C $(LIBFT)
			printf "$(WHT)[$(YEL)$(NAME) BINARIES REMOVED$(WHT)]\n"

re:			fclean all

debug:		INCLUDES += -DDEBUG_MODE
debug:		CFLAGS += -g -fsanitize=address
debug:		miniclean
debug:		all

.PHONY:		all bonus miniclean clean fclean re debug
