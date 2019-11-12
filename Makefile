
NAME            := libft_malloc.so

ifeq ($(HOSTTYPE),)
HOSTTYPE 		:= $(shell uname -m)_$(shell uname -s)
endif
FULLNAME		:= libft_malloc_$(HOSTTYPE).so
STATICNAME		:= libft_malloc_static.a

CC              := gcc
FLAGS           := -Wall -Wextra -Werror

SRCS_FILES      :=  malloc.c\
					free.c\
					helper.c\
					allocate_zone.c\
					realloc.c

HEADERS_FILES   :=  malloc.h\

SRCS_PATH       := srcs/
SRCS            := $(addprefix $(SRCS_PATH), $(SRCS_FILES))

OBJS_PATH       := objs/
OBJS            := $(addprefix $(OBJS_PATH), $(SRCS_FILES:.c=.o))
OBJ				:= $(addsuffix .o, $(basename $(SRCS)))

INCLUDES_PATH   := includes/
INCLUDES        := -I $(INCLUDES_PATH)
HEADERS         := $(addprefix $(INCLUDES_PATH), $(HEADERS_FILES))

LIBFT_PATH      := libft
LIBFT_INCLUDES  := -I libft/include
LIBFT           := -L $(LIBFT_PATH)/lib -lft

.PHONY: all libft clean fclean re

all: libft $(NAME)

libft:
	@make -C $(LIBFT_PATH)

%.o: %.c $(HEADERS)
	$(CC) $(FLAGS) $(INCLUDES) $(LIBFT_INCLUDES) -o $@ -c $<


$(NAME): $(OBJ)
	@$(CC) -shared $(OBJ) $(FLAGS) -o $(FULLNAME) $(LIBFT)
	@ln -fs $(FULLNAME) $@
	@echo "$(NAME):\033[92m linked\033[0m"

clean:
	@echo "Cleaning:\033[33m *.o\033[0m"
	@rm -f $(OBJ)
	@rmdir $(OBJS_PATH) 2> /dev/null || true
	@make -C $(LIBFT_PATH) clean

fclean: clean
	@echo "Cleaning:\033[33m $(NAME)\033[0m"
	@make fclean -C $(LIBFT_PATH)
	@rm -f $(NAME) ${FULLNAME} $(STATICNAME)

re: fclean all