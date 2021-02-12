SRCS	=	vmtranslator.c \
			parser.c \
			code.c \
			ft_split.c \

OBJS	=	$(SRCS:.c=.o)

DEPS	=	vmtranslator.h

NAME	=	VMtranslator

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

.PHONY:		all clean fclean re