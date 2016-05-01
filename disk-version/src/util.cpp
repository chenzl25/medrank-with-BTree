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
void read_datas_from_file(char* Mnist_ds, float** &datas, int n, int d) {
  printf("---------------------------------------------------\n");
  printf("read datas...\n");
  FILE* file;
  file = fopen(Mnist_ds, "r");
  assert(file != NULL, "the Mnist_ds file should can be opened.");
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
  fclose(file);
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
  // printf("generate_random_vectors...\n");
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
  // printf("store_random_vectors...\n");
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
  fclose(file);
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
  printf("---------------------------------------------------\n");
  printf("projecting data to random vector... \n");
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

  printf("---------------------------------------------------\n"); 
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

void indexing_from_data_set(float** datas, int n, int d, int num_of_random_vectors) {
  int num_of_random_project_norm_list = num_of_random_vectors;
  printf("---------------------------------------------------\n");
  printf("%s\n", "indexing...");
  
  
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
  printf("---------------------------------------------------\n");
  printf("BTree bulkload...\n");
  clock_t startTime = (clock_t) -1;
  clock_t endTime   = (clock_t) -1;
  startTime = clock();

  int total_index_size = 0;
  int num_of_b_tree = num_of_random_project_norm_list;
  for (int i = 0; i < num_of_b_tree; i++) {
    BTree* btree = new BTree();
    char b_tree_name[30];
    get_btree_name(b_tree_name, i);
    btree->init(b_tree_name, BNODE_SIZE);
    btree->bulkload(random_project_norm_list[i], n);
    total_index_size += btree->_block_file->num_blocks_ * btree->_block_file->get_blocklength();
    delete btree;
  }
  printf("total index size is %fM\n", (float)total_index_size/1024/1024);
  endTime = clock();
  printf("use %fs\n", (float)(endTime - startTime)/CLOCKS_PER_SEC);
  
  // free
  free_random_project_norm_list(random_project_norm_list, n, num_of_random_project_norm_list);
  free_random_vectors(random_vectors, d, num_of_random_vectors);
  
}

void read_random_vectors_from_file(char* filename, 
                                   float** &random_vectors,
                                   int d,
                                   int num_of_random_vectors) {
  FILE* file;
  file = fopen(filename, "r");
  assert(file != NULL, "the random_vectors file should exist");
  // printf("read_random_vectors_from_file...\n");

  for (int i = 0 ; i < num_of_random_vectors; i++) {
    int num;
    fscanf(file, "%d", &num);
    assert(num == i + 1, "the format of random_vectors file should be correct");
    for (int j = 0; j < d; j++) {
      fscanf(file, " %f", &random_vectors[i][j]);
    }
    fscanf(file, "\n");
  }
  fclose(file);
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
  // printf("read_querys_from_file...\n");

  for (int i = 0 ; i < qn; i++) {
    int num;
    fscanf(file, "%d", &num);
    assert(num == i + 1, "the format of Mnist.q file should be correct");
    for (int j = 0; j < d; j++) {
      fscanf(file, " %f", &querys[i][j]);
    }
    fscanf(file, "\n");
  }
  fclose(file);
}
void free_querys(float** &querys, int qn, int d) {
  for (int i = 0; i < qn; i++) 
    delete [] querys[i];
  delete [] querys;
}
void medrank_test(float** datas, float** querys, int qn, int d, int num_of_random_vectors) {
  printf("---------------------------------------------------\n");
  printf("begin query test...\n");
  float** random_vectors;
  init_random_vectors(random_vectors, d, num_of_random_vectors);
  read_random_vectors_from_file("random_vectors.data" ,random_vectors, d, num_of_random_vectors);
  
  int num_of_b_tree = num_of_random_vectors;
  BTree** btree_arr = new BTree*[num_of_b_tree];

  for (int i = 0; i < num_of_b_tree; i++) {
    btree_arr[i] = new BTree();
    char b_tree_name[30];
    get_btree_name(b_tree_name, i);
    btree_arr[i]->init_restore(b_tree_name);
  }

  printf("---------------------------------------------------\n");
  printf("medrank search...\n");
  clock_t startTime = (clock_t) -1;
  clock_t endTime   = (clock_t) -1;
  startTime = clock();

  int total_io_cost = 0;
  FILE* query_result_file;
  query_result_file = fopen("query-result.data", "w");
  assert(query_result_file != NULL, "query-result.file should can be open");
  Medrank* medrank = new Medrank();
  medrank->init(btree_arr, num_of_b_tree, random_vectors, d);
  for (int i = 0; i < qn; i++) {
    float* result;
    int datas_pos = medrank->search(querys[i], 0.5/*MINFREQ*/, &total_io_cost);
    result = datas[datas_pos];
    fprintf(query_result_file, "%d", i+1);
    for (int j =0 ; j < d; j++) {
      fprintf(query_result_file, " %d", (int)result[j]);
    }
    fprintf(query_result_file,"\n");
  }

  printf("average of IO Cost is %f\n", (float)total_io_cost/qn);

  endTime = clock();
  printf("average of Runing Time is %.6fs\n", ((float) endTime - startTime)/CLOCKS_PER_SEC/qn);
  // free
  delete medrank;

  for (int i = 0; i < num_of_b_tree; i++)
    delete btree_arr[i];
  delete [] btree_arr;
  
  free_random_vectors(random_vectors, d, num_of_random_vectors);
  
}

double distance (int* x, int* y, int d) {
  double result = 0;
  for (int i = 0; i < d; i++) {
    result += (x[i] - y[i]) * (x[i] - y[i]);
  }
  return sqrt(result);
}