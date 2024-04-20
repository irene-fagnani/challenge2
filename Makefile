CXX = g++
CXXFLAGS = -std=c++20
CPPFLAGS=-Wall -Wextra 
SRC_DIR = src
INCLUDE_DIR = include
TARGET = main
SRCS = $(wildcard $(SRC_DIR)/*.cpp)
OBJS = $(patsubst $(SRC_DIR)/%.cpp,%.o,$(SRCS))
INCLUDES:= -I$(INCLUDE_DIR)
.PHONY: all clean
%.o: $(SRC_DIR)/%.cpp
	$(CXX) -c $(CPPFLAGS) $(CXXFLAGS) $(INCLUDES) $<
$(TARGET): $(OBJS)
	$(CXX) $(CXXFLAGS) $< -o $@

clean:
	$(RM) $(OBJS)

distclean: clean
	$(RM) *~