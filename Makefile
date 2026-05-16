NAME        := Gomoku
CXX         := g++

# directories
SRCDIR      := src
INCDIR      := include
OBJDIR      := obj
BINDIR		:= bin

# target and files
.DEFAULT_GOAL := all
TARGET		:= $(BINDIR)/$(NAME)
SOURCES		:= $(shell find $(SRCDIR) -type f -name "*.cpp")
OBJECTS		:= $(patsubst $(SRCDIR)/%.cpp,$(OBJDIR)/%.o,$(SOURCES))
DEPENDS		:= $(OBJECTS:.o=.d)
-include $(DEPENDS)

# sfml attention pour mac
SFML_ROOT 	:= $(shell brew --prefix sfml)

# flags
# preprocessor
CPPFLAGS	:= -I$(INCDIR) -I$(SFML_ROOT)/include
# compiler
CXXFLAGS	:= -std=c++17 -Wall -Wextra -Werror
# dependency
DEPFLAGS	:= -MMD -MP
# linker path
LDFLAGS		:= -L$(SFML_ROOT)/lib
# libraries to link
LDLIBS		:= -lsfml-graphics -lsfml-window -lsfml-system

# colors
RESET		:= \033[0m
GREEN 		:= \033[32m
CYAN   		:= \033[36m


TEST_TARGET  := $(BINDIR)/GomokuTests
TEST_SOURCES := $(shell find tests -name "*.cpp")
CORE_SOURCES := $(filter src/core/%.cpp, $(SOURCES))
CORE_OBJECTS := $(patsubst $(SRCDIR)/%.cpp,$(OBJDIR)/%.o,$(CORE_SOURCES))

all: $(TARGET)

test: $(TEST_TARGET)
	@echo "$(CYAN)Running tests$(RESET)"
	@./$(TEST_TARGET)

GTEST_ROOT  := $(shell brew --prefix googletest)
CPPFLAGS    += -I$(GTEST_ROOT)/include

$(TEST_TARGET): $(TEST_SOURCES) $(CORE_OBJECTS) | $(BINDIR)
	@echo "$(CYAN)Compiling tests$(RESET)"
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) $(TEST_SOURCES) $(CORE_OBJECTS) -o $@ -L$(GTEST_ROOT)/lib -lgtest -lgtest_main -lpthread

$(TARGET): $(OBJECTS) | $(BINDIR)
	@echo "$(CYAN)Linking $(TARGET)$(RESET)"
	$(CXX) $(OBJECTS) -o $@ $(LDFLAGS) $(LDLIBS)
	@echo "$(GREEN)Built $(TARGET)$(RESET)"

$(OBJDIR)/%.o: $(SRCDIR)/%.cpp
	@mkdir -p $(@D)
	@echo "$(CYAN)Compiling $<$(RESET)"
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) $(DEPFLAGS) -c $< -o $@

$(BINDIR):
	@mkdir -p $(BINDIR)

run: all
	@./$(TARGET)

clean:
	@rm -rf $(OBJDIR)
	@echo "$(GREEN)Cleaned objects$(RESET)"

fclean: clean
	@rm -rf $(BINDIR)
	@echo "$(GREEN)Cleaned binaries$(RESET)"

re: fclean all

.PHONY: all run test clean fclean re

