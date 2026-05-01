NAME        := Gomoku
CXX         := g++

# directories
SRCDIR      := src
INCDIR      := include
OBJDIR      := obj
BINDIR		:= bin

# target and files
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


all: $(TARGET)

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

.PHONY: all run clean fclean re

