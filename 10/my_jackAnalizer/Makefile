SRCS	=	jackAnalyzer.c \
			jackTokenizer.c \
			compilationEngine.c \

OBJS	=	$(SRCS:.c=.o)

DEPS	=	jackAnalyzer.h

NAME	=	jackAnalyzer

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