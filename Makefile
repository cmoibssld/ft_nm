#########################
######  STRUCTURE  ######
#########################

SRC_DIR			=	srcs
INC_DIR			=	includes

BUILD_DIR		=	build
OBJ_DIR			=	${BUILD_DIR}/obj
DEP_DIR			=	${BUILD_DIR}/dep

#######################
######  SOURCES  ######
#######################

SRCS 			=	main.c utils.c

HEADERS			=	main.h

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

CC				=	cc
CFLAG			=	-MMD -MP -g3

MKDIR			=	@mkdir -vp
RM				=	@rm -vrf

NAME			=	ft_nm

#####################
######  RULES  ######
#####################

all: $(NAME)

-include $(DEPS)

$(NAME): $(OBJS)
	$(CC) $(CFLAG) $^ -o $@ 

${OBJ_DIR}/%.o: ${SRC_DIR}/%.c
	$(MKDIR) $(dir $@) $(dir $(GET_DEP_PATH))
	$(CC) $(CFLAG) -I$(INC_DIR) \
		-c $< -o $@ \
		-MF $(DEP_DIR)/$(notdir $(basename $<)).d -MT $@

clean:
	$(RM) $(BUILD_DIR)

fclean: clean
	rm -rf $(NAME)

re: fclean all

.PHONY: all clean fclean re 
