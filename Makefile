NAME	:= VulkanTest

CC	:=	c++

CFLAGS	:= -std=c++17 #-Wall -Wextra -Werror

LDFLAGS		:=	-lglfw -lvulkan -ldl -pthread -lX11 -lXxf86vm -lXrandr -lXi

IFLAGS		:= -I ./includes

SRCS	:= srcs/app.cpp \
		   srcs/main.cpp \
		   srcs/queues.cpp \

ifeq ($(DEBUG), 1)
	CFLAGS 	+=	-fsanitize=address -static-libsan -D NDEBUG
endif

BUILD	:= .objs

OBJS	:= $(addprefix $(BUILD)/, $(SRCS:%.cpp=%.o))

MAKEFLAGS	+= --no-print-directory

RM			:= rm -rf

DIR_UP		= mkdir -p $(@D)

################################################################################

all: $(NAME)

$(NAME): $(OBJS)
	@$(CC) $(CFLAGS) $(OBJS) -o $(NAME) $(LDFLAGS)
	@echo " $(GREEN)$(BOLD)$(ITALIC)■$(RESET)  building	$(GREEN)$(BOLD)$(ITALIC)$(NAME)$(RESET)"

$(BUILD)/%.o: %.cpp
	@$(DIR_UP)
	@echo " $(CYAN)$(BOLD)$(ITALIC)■$(RESET)  compiling	$(GRAY)$(BOLD)$(ITALIC)$^$(RESET)"
	@$(CC) $(CFLAGS) $(IFLAGS) -o $@ -c $<

clean:
	@$(RM) $(BUILD)

fclean: clean
	@$(RM) $(NAME)

re: fclean all

################################################################################


BLACK		=	\033[30m
RED			=	\033[31m
GREEN		=	\033[32m
YELLOW		=	\033[33m
BLUE		=	\033[34m
MAGENTA		=	\033[35m
CYAN		=	\033[36m
WHITE		=	\033[37m
GRAY		=	\033[90m

BOLD		=	\033[1m
ITALIC		=	\033[3m

RESET		=	\033[0m
LINE_CLR	=	\33[2K\r
