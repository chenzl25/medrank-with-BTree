#include "headers.h"

int main(int argc, char const *argv[]) {
  srand(time(NULL));
  int n = 60000;    // data size
  int d = 784;      // data dimension
  int qn = 100;     // query size
  char Mnist_ds[30] = "./data/Mnist.ds"; // path of data file
  char Mnist_q[30] = "./data/Mnist.q";   // path of query file
  for (int i = 1; i < argc; i++) {
    if (strcmp(argv[i], "-n") == 0) {
      n = atoi(argv[i+1]);
    } else if (strcmp(argv[i], "-d") == 0) {
      d = atoi(argv[i+1]);
    } else if (strcmp(argv[i], "-qn") == 0) {
      qn = atoi(argv[i+1]);
    } else if (strcmp(argv[i], "-ds") == 0) {
      memcpy(Mnist_ds, argv[i+1], sizeof(Mnist_ds));
    } else if (strcmp(argv[i], "-qs") == 0) {
      memcpy(Mnist_q, argv[i+1], sizeof(Mnist_ds));
    }
  }

  const float MINFREQ = 0.5;
  const int   num_of_random_vectors = 50;
  
  // read file
  

  float** datas;
  init_datas(datas, n, d);
  read_datas_from_file(Mnist_ds, datas, n, d);

  float** querys;
  init_querys(querys, qn, d);
  read_querys_from_file(Mnist_q, querys, qn, d);
  
  // indexing
  indexing_from_data_set(datas, n, d, num_of_random_vectors);
  
  // test query
  medrank_test(datas, n, querys, qn, d, num_of_random_vectors);
  
  // free
  free_datas(datas, n, d);
  
  free_querys(querys, qn, d);
  
  return 0;
}






