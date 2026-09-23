#########################
######  STRUCTURE  ######
#########################

SRC_DIR			=	srcs
INC_DIR			=	includes

LIBFT_DIR		= 	libft

BUILD_DIR		=	build
OBJ_DIR			=	${BUILD_DIR}/obj
DEP_DIR			=	${BUILD_DIR}/dep

#######################
######  SOURCES  ######
#######################

SRCS 			=	main.c 					\
					endian.c 				\
					errors.c 				\
					filling_array.c			\
					identification.c 		\
					output_formatting.c 	\
					section_header_table.c 	\
					section_header_info.c 	\
					sorting.c				\
					symbols_sorted_array.c

HEADERS			=	main.h 					\
					endian.h 				\
					errors.h 				\
					filling_array.h			\
					identification.h 		\
					output_formatting.c 	\
					section_header_table.h	\
					section_header_info.h	\
					sorting.h				\
					symbols_sorted_array.h

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

CC				=	clang # For cross compilation and test my own object files
CFLAG			=	-Wall -Werror -Wextra \
					-MMD -MP -g3

TARGET_FLAGS	=

X86_64_FLAGS	= -target x86_64-i386pep

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
	$(CC) $(CFLAG) $(TARGET_FLAGS) $^ -o $@ $(LIBFT_DIR)/$(LIBFT_LIB)

${OBJ_DIR}/%.o: ${SRC_DIR}/%.c
	$(MKDIR) $(dir $@) $(dir $(GET_DEP_PATH))
	$(CC) $(CFLAG) $(TARGET_FLAGS) -I$(INC_DIR) -I$(LIBFT_DIR)/$(INC_DIR) \
		-c $< -o $@ \
		-MF $(DEP_DIR)/$(notdir $(basename $<)).d -MT $@

init:
	git submodule init
	git submodule update
		
$(LIBFT_LIB):
	$(MAKE) $(TARGET_FLAGS) -C $(LIBFT_DIR)
		
clean:
	$(RM) $(BUILD_DIR)
	-$(MAKE) clean -C $(LIBFT_DIR)

fclean: clean
	$(RM) $(NAME)
	-$(MAKE) fclean -C $(LIBFT_DIR)

x86_64:
	$(MAKE) TARGET_FLAGS="$(X86_64_FLAGS)" all

big_endian:
	$(MAKE) TARGET_FLAGS="$(BIG_ENDIAN_FLAGS)" all
	
re: fclean all

.PHONY: init all clean fclean re x86_64 big_endian
