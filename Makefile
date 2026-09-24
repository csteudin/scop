NAME		= scop

CXX			= c++
CXXFLAGS	= -Wall -Wextra -Werror -std=c++17

ifdef DEBUG
    CXXFLAGS += -DDEBUG -g
endif

SRCS_DIR	= src
OBJ_DIR		= obj
INC_DIR		= inc

GLAD_DIR	= external/glad
GLAD_SRC	= $(GLAD_DIR)/src/glad.c
GLAD_OBJ	= $(OBJ_DIR)/glad.o

SRCS		= main.cpp Shader.cpp Window.cpp Vec3.cpp Mat4.cpp Parser.cpp
OBJS		= $(addprefix $(OBJ_DIR)/, $(SRCS:.cpp=.o))

INCLUDES	= -I$(INC_DIR) -I$(GLAD_DIR)/include

LDFLAGS		= -lglfw -lGL -ldl -lpthread

all: $(NAME)

$(NAME): $(OBJS) $(GLAD_OBJ)
	$(CXX) $(OBJS) $(GLAD_OBJ) -o $(NAME) $(LDFLAGS)

$(OBJ_DIR)/%.o: $(SRCS_DIR)/%.cpp | $(OBJ_DIR)
	$(CXX) $(CXXFLAGS) $(INCLUDES) -c $< -o $@

$(GLAD_OBJ): $(GLAD_SRC) | $(OBJ_DIR)
	$(CC) -I$(GLAD_DIR)/include -c $< -o $@

$(OBJ_DIR):
	mkdir -p $(OBJ_DIR)

clean:
	rm -rf $(OBJ_DIR)

fclean: clean
	rm -f $(NAME)

re: fclean all

.PHONY: all clean fclean re