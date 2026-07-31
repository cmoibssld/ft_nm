#########################
######  STRUCTURE  ######
#########################

SRC_DIR			=	srcs
INC_DIR			=	includes

LIBFT_DIR		= 	libft/

BUILD_DIR		=	build
OBJ_DIR			=	${BUILD_DIR}/obj
DEP_DIR			=	${BUILD_DIR}/dep

#######################
######  SOURCES  ######
#######################

SRCS 			=	main.c \
					errors.c

HEADERS			=	main.h \
					errors.h

###########################
######  COMPILATION  ######
###########################

MAKE			=	make
MAKEFLAGS 		+=	--no-print-directory
RED 			=	$(shell tput setaf 1)
GREEN			=	$(shell tput setaf 2)
CYAN			=	$(shell tput setaf 6)
RESET			=	$(shell tput setaf 255)

OBJS			=	${SRCS:%.c=${OBJ_DIR}/%.o}
DEPS			=	${SRCS:%.c=${DEP_DIR}/%.d}
GET_DEP_PATH	=	${@:${OBJ_DIR}/%.o=${DEP_DIR}/.%d}

OBJS_TEST		= ${UNIT_TEST:%.c=${TEST_DIR}/%.o}

CC				=	cc
CFLAG			=	-Wall -Werror -Wextra \
					-MMD -MP -g3

MKDIR			=	@mkdir -vp
RM				=	@rm -vrf

NAME			= 	ft_nm
LIBFT_LIB		=	libft.a

#####################
######  RULES  ######
#####################

all: $(LIBFT_LIB) $(NAME)

-include $(DEPS)

$(NAME): $(OBJS)
	$(CC) $(CFLAG) $^ -o $@ $(LIBFT_DIR)/$(LIBFT_LIB)

${OBJ_DIR}/%.o: ${SRC_DIR}/%.c
	$(MKDIR) $(dir $@) $(dir $(GET_DEP_PATH))
	$(CC) $(CFLAG) -I$(INC_DIR) -I$(LIBFT_DIR)/$(INC_DIR) \
		-c $< -o $@ \
		-MF $(DEP_DIR)/$(notdir $(basename $<)).d -MT $@

$(LIBFT_LIB):
	$(MAKE) -C $(LIBFT_DIR)
		
clean:
	$(RM) $(BUILD_DIR)
	$(MAKE) clean -C $(LIBFT_DIR)

fclean: clean
	rm -rf $(NAME)
	$(MAKE) clean -C $(LIBFT_DIR)

re: fclean all

.PHONY: all clean fclean re 
