SRCS  = src/random.cpp
SRCS += src/main.cpp
SRCS += src/util.cpp
SRCS += src/b_tree.cpp
SRCS += src/b_node.cpp
SRCS += src/block_file.cpp
SRCS += src/quick_sort.cpp
SRCS += src/medrank.cpp

OBJS=$(SRCS:.cpp=.o)

CXX?=g++
CPPFLAGS=-w

all: $(OBJS)
	$(CXX) -o medrank $(OBJS)

random.o: src/random.h

util.o: src/util.h

b_tree.o: src/b_tree.h

b_node.o: src/b_node.h

block_file.o: src/block_file.h

quick_sort.o: src/quick_sort.h

medrank.o: src/medrank.h

main.o: 

clean:
	-rm $(OBJS) medrank
