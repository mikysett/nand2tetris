# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: msessa <mikysett@gmail.com>                +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2021/01/14 14:13:08 by msessa            #+#    #+#              #
#    Updated: 2021/08/24 11:23:23 by msessa           ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

MAKEFLAGS	+=	--quiet

WHT	= \033[0;37m
BLK	= \033[0;30m
RED	= \033[0;31m
YEL	= \033[0;33m
GRN	= \033[0;32m

NAME	=	libft.a

FAST	=	

SRCS	=	ft_memset.c \
			ft_bzero.c \
			ft_memcpy.c \
			ft_memccpy.c \
			ft_memmove.c \
			ft_memchr.c \
			ft_memcmp.c \
			ft_strlen.c \
			ft_strlcpy.c \
			ft_strlcat.c \
			ft_strchr.c \
			ft_strrchr.c \
			ft_strnstr.c \
			ft_strncmp.c \
			ft_atoi.c \
			ft_isalpha.c \
			ft_isdigit.c \
			ft_isalnum.c \
			ft_isascii.c \
			ft_isprint.c \
			ft_toupper.c \
			ft_tolower.c \
			ft_calloc.c \
			ft_strdup.c \
			ft_substr.c \
			ft_strjoin.c \
			ft_strtrim.c \
			ft_split.c \
			ft_itoa.c \
			ft_strmapi.c \
			ft_putchar_fd.c \
			ft_putstr_fd.c \
			ft_putendl_fd.c \
			ft_putnbr_fd.c

BSRCS	:=	$(SRCS) \
			ft_lstnew.c \
			ft_lstadd_front.c \
			ft_lstsize.c \
			ft_lstlast.c \
			ft_lstadd_back.c \
			ft_lstdelone.c \
			ft_lstclear.c \
			ft_lstiter.c \
			ft_lstmap.c \
			ft_abs.c \
			ft_free_str_arr.c \
			ft_strndup.c

OBJS	=	$(SRCS:.c=.o)

BOBJS	=	$(BSRCS:.c=.o)

DEPS	=	libft.h

CC		=	gcc

CFLAGS	=	-Wall -Wextra

RM		=	rm -f

%.o: %.c
			$(CC) $(CFLAGS) $(INCLUDES) -o $@ -c $<

$(NAME):	$(DEPS) $(OBJS)
			ar rc $(NAME) $(OBJS)
			printf "$(WHT)[$(GRN)$(NAME) COMPILED$(WHT)]\n"

all:		$(NAME)

fastbonus:		CFLAGS += -O3
fastbonus:		bonus
				printf "$(WHT)[$(GRN)$(NAME) --> OPTIMIZATION FLAG ACTIVE (-O3)$(WHT)]\n"

clean:
			$(RM) $(OBJS)
			$(RM) $(BOBJS)
			printf "$(WHT)[$(YEL)$(NAME) OBJS REMOVED$(WHT)]\n"

fclean:		clean
			$(RM) $(NAME)
			printf "$(WHT)[$(YEL)$(NAME) BINARIES REMOVED$(WHT)]\n"

re:			fclean all

bonus:		$(DEPS) $(BOBJS)
			ar rc $(NAME) $(BOBJS)
			printf "$(WHT)[$(GRN)$(NAME) BONUS COMPILED$(WHT)]\n"

.PHONY:		all fastbonus clean fclean re bonus
