SRCS  = src/random.cpp
SRCS += src/main.cpp
SRCS += src/util.cpp
SRCS += src/b_tree.cpp
SRCS += src/b_node.cpp
SRCS += src/b_node_cache.cpp
SRCS += src/block_file.cpp
SRCS += src/quick_sort.cpp
SRCS += src/medrank.cpp
SRCS += src/l2nn.cpp

OBJS=$(SRCS:.cpp=.o)

CXX?=g++
CPPFLAGS=-w

all: $(OBJS)
	$(CXX) -o medrank $(OBJS)

random.o:       src/random.h

util.o: 				src/util.h

b_tree.o: 			src/b_tree.h

b_node.o: 			src/b_node.h

b_node_cache.o: src/b_node_cache.h

block_file.o: 	src/block_file.h

quick_sort.o: 	src/quick_sort.h

medrank.o: 			src/medrank.h

l2nn.o: 				src/l2nn.h

main.o: 

clean:
	-rm $(OBJS) medrank query-result.data random_vectors.data l2nn-query-result.data medrank-query-result.data
	-rm -r Btree_storage
