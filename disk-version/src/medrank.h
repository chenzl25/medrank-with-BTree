#ifndef MEDRANK_H
#define MEDRANK_H

class BNode;
class BTree;

class Medrank {
public:
  Medrank();
  ~Medrank();
  // void preprocess(char* Mnist_ds, int n, int d, int num_of_rank_list);
  void init(BTree** btree_arr, int num_of_b_tree, float** random_vectors, int d);
  int search(float* query, float MINFREQ, int *io_cost);
private:
  float inner_product(float* vector_a, float* vector_b, int d);
  int _d;
  int _num_of_b_tree;
  float _MINFREQ;
  BTree** _btree_arr;
  float** _random_vectors;
};

#endif