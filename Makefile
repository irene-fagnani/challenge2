CXX = g++
CXXFLAGS = -std=c++20
CPPFLAGS=-Wall -Wextra -O3
SRC_DIR = src
INCLUDE_DIR = include
TARGET = main
DOXYFILE=../../pacs-examples/Examples/DoxyfileCommon
SRCS = $(wildcard $(SRC_DIR)/*.cpp)
OBJS = $(patsubst $(SRC_DIR)/%.cpp,%.o,$(SRCS))
INCLUDES:= -I$(INCLUDE_DIR)
.PHONY: all clean
%.o: $(SRC_DIR)/%.cpp
	$(CXX) -c $(CPPFLAGS) $(CXXFLAGS) $(INCLUDES) $<
$(TARGET): main.o
	$(CXX) $(CXXFLAGS) main.o -o main

clean:
	$(RM) $(OBJS) $(TARGET)

distclean: clean
	$(RM) *~

doc:
	doxygen $(DOXYFILE)