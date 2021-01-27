SRCS	=	assembler.c \
			parser.c \
			code.c \
			symbol_table.c

OBJS	=	$(SRCS:.c=.o)

DEPS	=	assembler.h

NAME	=	assembler

CC		=	gcc

CFLAGS	=	-Wall -Wextra -Werror

RM		=	rm -f

$(NAME):	$(DEPS) $(OBJS)
			$(CC) $(CFLAGS) -E $(OBJS)
			$(CC) $(CFLAGS) -o $(NAME) $(OBJS)

all:		$(NAME)

clean:
			$(RM) $(OBJS)

fclean:		clean
			$(RM) $(NAME)

re:			fclean all

.PHONY:		all clean fclean re bonus