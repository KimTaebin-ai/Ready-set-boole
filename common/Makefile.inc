# Shared build rules for every exercise.
#
# An exercise Makefile only names what is specific to it:
#
#   NAME   = adder                     the binary to build
#   SRCS   = adder.cpp main.cpp        sources in this folder
#   SHARED = ../common/ast.cpp         sources borrowed from elsewhere
#
#   include ../common/Makefile.inc
#
# `vpath %.cpp` lets the pattern rule below find the borrowed sources, and the
# directories they live in are added to the include path, so `cd exNN && make`
# keeps working without copying anything.
#
# The search is restricted to .cpp on purpose: a plain VPATH would also make
# make look for obj/*.o in those directories and happily reuse another
# exercise's object files.

CXX = clang++
CXXFLAGS = -Wall -Wextra -Werror -std=c++17

OBJ_DIR = ./obj

SOURCES = $(SRCS) $(SHARED)
SHARED_DIRS = $(sort $(dir $(SHARED)))
OBJS = $(addprefix $(OBJ_DIR)/, $(notdir $(SOURCES:.cpp=.o)))

vpath %.cpp $(SHARED_DIRS)
CPPFLAGS = $(addprefix -I, $(SHARED_DIRS))

all: $(NAME)

$(NAME): $(OBJS)
	$(CXX) $(CXXFLAGS) $(OBJS) -o $(NAME)

$(OBJ_DIR)/%.o: %.cpp
	mkdir -p $(OBJ_DIR)
	$(CXX) $(CXXFLAGS) $(CPPFLAGS) -c $< -o $@

clean:
	rm -rf $(OBJ_DIR)

fclean: clean
	rm -f $(NAME)

re: fclean all

.PHONY: all clean fclean re
