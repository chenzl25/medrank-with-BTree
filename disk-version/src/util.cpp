#include "headers.h"

void assert(bool should, char* msg) {
  if (!should) {
    printf("assert fail: %s\n", msg);
    exit(-1);
  }
}
void debug(char* msg) {
  printf("debug: %s\n", msg);
}
void error(             // display an error message
  char* msg,              // message
  bool is_exit)           // whether exit
{
  printf(msg);
  if (is_exit) exit(1);
}

void init_datas(float** &datas, int n, int d) {
  datas = new float*[n];
  for (int i = 0; i < n; i++)
    datas[i] = new float[d];
}
void free_datas(float** &datas, int n, int d) {
  for (int i = 0; i < n; i++)
    delete [] datas[i];
  delete [] datas;
}
void read_datas(float** &datas, int n, int d, FILE* file) {
  printf("read datas...\n");
  clock_t startTime = (clock_t) -1;
  clock_t endTime   = (clock_t) -1;
  startTime = clock();
  for (int i = 0; i < n; i++) {
    int num = 0;
    fscanf(file, "%d", &num);
    for (int j = 0; j < d; j++) {
      fscanf(file, " %f", &datas[i][j]);
    }
    fscanf(file, "\n");
  }
  endTime = clock();
  printf("use %.6f s.\n", ((float) endTime - startTime)/CLOCKS_PER_SEC);
}
void init_random_vectors(float** &random_vectors, int d, int num_of_random_vectors) {
  random_vectors = new float*[num_of_random_vectors];
  for (int i = 0; i < num_of_random_vectors; i++)
    random_vectors[i] = new float[d];
}
void free_random_vectors(float** &random_vectors, int d, int num_of_random_vectors) {
  for (int i = 0; i < num_of_random_vectors; i++) 
    delete [] random_vectors[i];
  delete [] random_vectors;
}
void generate_random_vectors(float** &random_vectors, int d, int num_of_random_vectors) {
  printf("generate_random_vectors\n");
  // std::default_random_engine e;
  // std::normal_distribution<> n (0, 1);
  for (int i = 0; i < num_of_random_vectors; i++) {
    float norm = 0;
    for (int j = 0; j < d; j++) {
      // random_vectors[i][j] = n(e);
      random_vectors[i][j] = gaussian_rand();
      norm += random_vectors[i][j] * random_vectors[i][j];
    }
    for (int j = 0; j < d; j++) {
      random_vectors[i][j] /= sqrt(norm);
    }
  }
}
void store_random_vectors(float** &random_vectors, int d, int num_of_random_vectors) {
  printf("store_random_vectors...\n");
  FILE* file;
  file = fopen("random_vectors.data", "w");
  if (file == NULL) {
    printf("open random_vectors.data fail\n");
    return;
  }
  for (int i = 0; i < num_of_random_vectors; i++) {
    fprintf(file, "%d", i+1);
    for (int j = 0; j < d; j++) {
      fprintf(file, " %f", random_vectors[i][j]);
    }
    fprintf(file, "\n");
  }
}

void init_random_project_norm_list(project_entry** &random_project_norm_list,
                                   int n,
                                   int num_of_random_project_norm_list) {
  random_project_norm_list = new project_entry*[num_of_random_project_norm_list];
  for (int i = 0; i < num_of_random_project_norm_list; i++)
    random_project_norm_list[i] = new project_entry[n];
}
void free_random_project_norm_list(project_entry** &random_project_norm_list,
                                   int n,
                                   int num_of_random_project_norm_list) {
  for (int i = 0; i < num_of_random_project_norm_list; i++) 
    delete [] random_project_norm_list[i];
  delete [] random_project_norm_list;
}


float inner_product(float* vector_a, float* vector_b, int d) {
  float result = 0;
  for (int i = 0; i < d; i++) {
    result += vector_a[i] * vector_b[i];
  }
  return result;
}

void generate_random_project_norm_list(project_entry** &random_project_norm_list,
                               int d,
                               int n,
                               int num_of_random_project_norm_list,
                               float** &random_vectors,
                               float** &datas) {
  printf("random_project_norm_list begin generate... \n");
  clock_t startTime = (clock_t) -1;
  clock_t endTime   = (clock_t) -1;
  
  startTime = clock();
  for (int i = 0; i < num_of_random_project_norm_list; i++) {
    // printf("random_project_norm_list %d begin generate\n", i);
    for (int j = 0; j < n; j++) {
      random_project_norm_list[i][j].key = j;
      random_project_norm_list[i][j].value = inner_product(random_vectors[i], datas[j], d);
    }
  }
  endTime = clock();
  printf("use %fs\n", (float)(endTime - startTime)/CLOCKS_PER_SEC);
  
  printf("sort_random_project_norm_list...\n");
  startTime = clock();
  for (int i = 0; i < num_of_random_project_norm_list; i++)
    quick_sort(random_project_norm_list[i], 0, n-1);
  endTime = clock();
  printf("use %fs\n", (float)(endTime - startTime)/CLOCKS_PER_SEC);
  // for (int i = 0; i < num_of_random_project_norm_list; i++) {
  //   for (int j = 1; j < d; j++) {
  //     assert(random_project_norm_list[i][j-1].value <= random_project_norm_list[i][j].value, "sort should hold");
  //   }
  // }

}

void get_btree_name(char b_tree_name[], int i) {
  char name[15] = "b-tree-";
  memcpy(b_tree_name, name, sizeof(name));
  char num[5];
  sprintf(num, "%d", i);
  strcat(b_tree_name, num);
  strcat(b_tree_name, ".data");
}

void indexing_from_data_set(char* Mnist_ds, int n, int d, int num_of_random_vectors) {
  int num_of_random_project_norm_list = num_of_random_vectors;
  printf("%s\n", "indexing");
  FILE* file;
  file = fopen(Mnist_ds, "r");
  if (file == NULL) {
    printf("open %s fail\n", Mnist_ds);
  }
  printf("open %s successfully\n", Mnist_ds);

  float** datas;
  init_datas(datas, n, d);
  read_datas(datas, n, d, file);
  
  float** random_vectors;
  init_random_vectors(random_vectors, d, num_of_random_vectors);
  generate_random_vectors(random_vectors, d, num_of_random_vectors);
  store_random_vectors(random_vectors, d, num_of_random_vectors);

  project_entry** random_project_norm_list;
  init_random_project_norm_list(random_project_norm_list, n, num_of_random_project_norm_list);
  generate_random_project_norm_list(random_project_norm_list,
                                    d,
                                    n,
                                    num_of_random_vectors,
                                    random_vectors,
                                    datas);

  int num_of_b_tree = num_of_random_project_norm_list;
  for (int i = 0; i < num_of_b_tree; i++) {
    BTree* btree = new BTree();
    char b_tree_name[30];
    get_btree_name(b_tree_name, i);
    btree->init(b_tree_name, BNODE_SIZE);
    btree->bulkload(random_project_norm_list[i], n);
    delete btree;
  }

  free_random_project_norm_list(random_project_norm_list, n, num_of_random_project_norm_list);
  free_random_vectors(random_vectors, d, num_of_random_vectors);
  free_datas(datas, n, d);

}

void read_random_vectors_from_file(char* filename, 
                                   float** &random_vectors,
                                   int d,
                                   int num_of_random_vectors) {
  FILE* file;
  file = fopen(filename, "r");
  assert(file != NULL, "the random_vectors file should exist");
  printf("read_random_vectors_from_file...\n");

  for (int i = 0 ; i < num_of_random_vectors; i++) {
    int num;
    fscanf(file, "%d", &num);
    assert(num == i + 1, "the format of random_vectors file should be correct");
    for (int j = 0; j < d; j++) {
      fscanf(file, " %f", &random_vectors[i][j]);
    }
    fscanf(file, "\n");
  }
}
void init_querys(float** &querys, int qn, int d) {
  querys = new float*[qn];
  for (int i = 0; i < qn; i++)
    querys[i] = new float[d];

}
void read_querys_from_file(char* Mnist_q, float** &querys, int qn, int d) {
  FILE* file;
  file = fopen(Mnist_q, "r");
  assert(file != NULL, "the Mnist.q file should exist");
  printf("read_querys_from_file...\n");

  for (int i = 0 ; i < qn; i++) {
    int num;
    fscanf(file, "%d", &num);
    assert(num == i + 1, "the format of Mnist.q file should be correct");
    for (int j = 0; j < d; j++) {
      fscanf(file, " %f", &querys[i][j]);
    }
    fscanf(file, "\n");
  }
}
void free_querys(float** &querys, int qn, int d) {
  for (int i = 0; i < qn; i++) 
    delete [] querys[i];
  delete [] querys;
}
void medrank_test(char* Mnist_q, int qn, int d, int num_of_random_vectors) {
  float** random_vectors;
  init_random_vectors(random_vectors, d, num_of_random_vectors);
  read_random_vectors_from_file("random_vectors.data" ,random_vectors, d, num_of_random_vectors);

  float** querys;
  init_querys(querys, qn, d);
  read_querys_from_file(Mnist_q, querys, qn, d);
  free_querys(querys, qn, d);
  free_random_vectors(random_vectors, d, num_of_random_vectors);
}

