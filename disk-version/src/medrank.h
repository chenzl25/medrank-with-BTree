#ifndef MEDRANK_H
#define MEDRANK_H

class BNode;
class BTree;

struct BNode_Cache {
  BNode node;
  int current_index;
};

class Medrank {
public:
  Medrank();
  ~Medrank();
  void preprocess(char* Mnist_ds, int n, int d, int num_of_rank_list);
  void init_restore();
  void cnn_search(float* query);
private:
  int _n;
  int _d;
  int _num_of_rank_list;
  float _MINFREQ;
};

#endif