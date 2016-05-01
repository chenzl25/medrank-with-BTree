#ifndef B_NODE_CACHE_H
#define B_NODE_CACHE_H

class BNode_Cache {
public:
  // here we just combine all the operator and the data together.
  BNode* node;
  int current_index;
  int *io_cost;
  BNode_Cache();
  ~BNode_Cache();
  bool valid();
  void prev();
  void next();
  int key();
  float value();
};

#endif