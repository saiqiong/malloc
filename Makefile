NAME            := libft_malloc.so

ifeq ($(HOSTTYPE),)
HOSTTYPE 		:= $(shell uname -m)_$(shell uname -s)
endif
FULLNAME		:= libft_malloc_$(HOSTTYPE).so
STATICNAME		:= libft_malloc_static.a

CC              := gcc
FLAGS           := -Wall -Wextra -Werror -g

SRCS_FILES      :=  malloc.c\

HEADERS_FILES   :=  malloc.h\

SRCS_PATH       := srcs/
SRCS            := $(addprefix $(SRCS_PATH), $(SRCS_FILES))

OBJS_PATH       := objs/
OBJS            := $(addprefix $(OBJS_PATH), $(SRCS_FILES:.c=.o))

INCLUDES_PATH   := includes/
INCLUDES        := -I $(INCLUDES_PATH)
HEADERS         := $(addprefix $(INCLUDES_PATH), $(HEADERS_FILES))

LIBFT_PATH      := libft/
LIBFT_INCLUDES  := -I libft/includes
LIBFT           := -L $(LIBFT_PATH) -lft

LIBFTPRINTF_PATH = libft/ft_printf
LIBFTPRINTF_INCLUDE = -I$(LIBFTPRINTF_PATH)/includes
LIBFTPRINTF = -L$(LIBFTPRINTF_PATH) -lftprintf



.PHONY: all libft clean fclean re

all: libft $(NAME)

libft:
	@make -C $(LIBFT_PATH)

$(OBJS_PATH)%.o: $(SRCS_PATH)%.c $(HEADERS) $(LIBFT_PATH)/libft.a $(LIBFT_PATH)/libftprintf.a
	@mkdir $(OBJS_PATH) 2> /dev/null || true
	@$(CC) $(FLAGS) $(INCLUDES) $(LIBFT_INCLUDES) $(LIBFTPRINTF_INCLUDE) -o $@ -c $<

$(NAME): $(OBJS)
	@$(CC) -shared $(OBJS) -o $(FULLNAME) $(LIBFT) $(LIBFTPRINTF)
	@ln -fs $(FULLNAME) $@
	@echo "$(NAME):\033[92m linked\033[0m"

clean:
	@echo "Cleaning:\033[33m *.o\033[0m"
	@rm -f $(OBJS)
	@rmdir $(OBJS_PATH) 2> /dev/null || true
	@make -C $(LIBFT_PATH) clean

fclean: clean
	@echo "Cleaning:\033[33m $(NAME)\033[0m"
#	@make fclean -C ./tests
	@make fclean -C $(LIBFT_PATH)
	@rm -f $(NAME) ${FULLNAME} $(STATICNAME)

re: fclean all

#test: all
#	@ar rc $(STATICNAME) $(LIBFT_PATH)objs/*.o $(OBJS)
#	@ranlib $(STATICNAME)
#	@make test -C ./tests
