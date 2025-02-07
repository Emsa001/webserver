NAME = webserv
CXX = c++ -std=c++98 #-fsanitize=address
CPPFLAGS = -Iincludes #-Wall -Werror -Wextra 

SRC_DIR = src
OBJ_DIR = .obj
INCLUDES_DIR = includes

SRC = $(wildcard $(SRC_DIR)/*.cpp) $(wildcard $(SRC_DIR)/*/*.cpp) $(wildcard $(SRC_DIR)/*/*/*.cpp) $(wildcard ./ft_async/*/*.c)
OBJ = $(patsubst $(SRC_DIR)/%.cpp, $(OBJ_DIR)/%.o, $(SRC))
INCLUDES = $(wildcard $(INCLUDES_DIR)/*.h) $(wildcard $(INCLUDES_DIR)/*/*.h)

all: $(NAME)

$(NAME): $(OBJ)
	$(CXX) $(CPPFLAGS) $(OBJ) -o $(NAME)

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp $(INCLUDES)
	mkdir -p $(dir $@)
	$(CXX) $(CPPFLAGS) -c $< -o $@

run: all
	./$(NAME)

clean:
	rm -rf $(OBJ_DIR)

fclean: clean
	rm -rf $(NAME)

re: clean run

.PHONY: all clean fclean re