#ifndef UTIL_H
#define UTIL_H

void assert(bool should, char* msg);
void error(char* msg, bool is_exit);
void debug(char* msg);
void indexing_from_data_set(char* Mnist_ds, int n, int d, int num_of_random_vectors);
void medrank_test(char* Mnist_q, int qn, int d, int num_of_random_vectors);


#endif