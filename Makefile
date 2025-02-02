NAME =  webserv
CXX = c++ -std=c++98
CPPFLAGS = -Wall -Werror -Wextra

SRC = $(wildcard *.cpp) $(wildcard */*.cpp)
OBJ = $(SRC:.cpp=.o)

all: $(NAME)

$(NAME): $(OBJ)
	$(CXX) $(CPPFLAGS) $(OBJ) -o $(NAME)

clean:
	rm -rf $(OBJ)

fclean: clean
	rm -rf $(NAME)

re: clean all

.PHONY: all clean fclean re