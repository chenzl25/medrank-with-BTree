#ifndef UTIL_H
#define UTIL_H

void assert(bool should, char* msg);
void error(char* msg, bool is_exit);
void debug(char* msg);
void init_datas(float** &datas, int n, int d);
void free_datas(float** &datas, int n, int d);
void read_datas_from_file(char* Mnist_ds, float** &datas, int n, int d);
void init_querys(float** &querys, int qn, int d);
void read_querys_from_file(char* Mnist_q, float** &querys, int qn, int d);
void free_querys(float** &querys, int qn, int d);
void indexing_from_data_set(float** datas, int n, int d, int num_of_random_vectors);
void medrank_test(float** datas, float** querys, int qn, int d, int num_of_random_vectors);


#endif