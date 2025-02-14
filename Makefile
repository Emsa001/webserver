
NAME			= webserv

# compiler settings
CXX				= c++
CXXFLAGS		= -Wall -Wextra #-Werror # TODO
CXXFLAGS		+= -std=c++98 -pedantic
CXXFLAGS		+= -I$(INCDIR)

# source files
INCDIR			= inc
SRCDIR			= src
OBJDIR			= .obj
SRC				= $(wildcard $(SRCDIR)/*.cpp)
OBJ				= $(SRC:$(SRCDIR)/%.cpp=$(OBJDIR)/%.o)


run: $(NAME)
	./$(NAME)

all: $(NAME)

clean:
	$(RM) -rf $(wildcard $(OBJDIR)/*)

fclean: clean
	$(RM) -f $(NAME)

re: fclean
	$(MAKE)

$(NAME): $(OBJ)
	$(CXX) $(CXXFLAGS) $(LDFLAGS) -o "$@" $?

$(OBJ): $(OBJDIR)/%.o: $(SRCDIR)/%.cpp
	$(CXX) $(CXXFLAGS) $(LDFLAGS) -c -o "$@" "$<"

setup:
	docker build -t pre-commit -f assets/pre-commit.Dockerfile . 

format:
	docker run -v.:/mnt -it --rm pre-commit

.PHONY: run all clean fclean re test t setup format

# ===== TESTING =====
TEST_CXXFLAGS		+= -I$(GTESTDIR)/googletest/include -g -I$(INCDIR)
TEST_LDFLAGS		+= -L$(GTESTDIR)/build/lib
TEST_LDLIBS			+= -lgtest
TEST_MAIN			= tests/test.cpp
TEST_OBJ			= $(filter-out %/main.o, $(OBJ))

GTESTDIR = gtest
GTEST = $(GTESTDIR)/build/lib/libgtest.a

test: $(GTEST) $(TEST_OBJ) $(TEST_MAIN)
	$(CXX) -o "$@" $(TEST_CXXFLAGS) $(TEST_MAIN) $(TEST_OBJ) $(TEST_LDFLAGS) $(TEST_LDLIBS)

t: test
	./$<

$(GTESTDIR):
	git clone --depth=1 https://github.com/google/googletest "$@"

$(GTEST): $(GTESTDIR)
	cd $(GTESTDIR) && \
	mkdir -p build && \
	cd build && \
	cmake .. && \
	make -j$(shell nproc)

.PHONY: all clean fclean re run leak t