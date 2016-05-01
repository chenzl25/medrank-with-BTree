#include "headers.h"

class BNode_Cache;

Medrank::Medrank() {
  _d = -1;
  _num_of_b_tree = -1;
  _MINFREQ = 0.5;
  _btree_arr = NULL;
}
Medrank::~Medrank() {

}
void Medrank::init(BTree** btree_arr, int num_of_b_tree, float** random_vectors, int d) {
  _btree_arr = btree_arr;
  _num_of_b_tree = num_of_b_tree;
  _random_vectors = random_vectors;
  _d = d;
}
int Medrank::search(float* query, float MINFREQ, int *io_cost) {
  _MINFREQ = MINFREQ;
  float* project_query = new float[_num_of_b_tree];
  
  for (int i = 0; i < _num_of_b_tree; i++){
    project_query[i] = inner_product(query, _random_vectors[i], _d);

  }

  BNode_Cache* h = new BNode_Cache[_num_of_b_tree];
  BNode_Cache* l = new BNode_Cache[_num_of_b_tree];
  for (int i = 0 ; i < _num_of_b_tree; i++) {
    _btree_arr[i]->locate(project_query[i], &h[i], &l[i], io_cost);
    
    {// test locate work correctly
      if (h[i].node != NULL) assert(h[i].node->get_level() == 0, "h level should be 0");
      if (l[i].node != NULL) assert(l[i].node->get_level() == 0, "l level should be 0");
      if (h[i].node == NULL) assert(l[i].node != NULL && l[i].current_index == 0, "locate assert fail");
      if (l[i].node == NULL) assert(h[i].node != NULL && h[i].current_index == h[i].node->get_num_entries()-1, "locate assert fail");
      if (l[i].current_index == 0) {
        // printf("%d\n", h[i].current_index);
        assert(h[i].current_index == h[i].node->get_num_entries()-1, "...");
        assert(h[i].node->get_block_address()+1 == l[i].node->get_block_address(), "...");
        // printf("%d, %d\n", h[i].node->get_block_address(), l[i].node->get_block_address());
      }
      // _btree_arr[i]->locate(-1111111, &h[i], &l[i]);
      // assert(h[i].node == NULL, "h should be NULL");
      // assert(l[i].current_index == 0, "l current_index should be 0");
      // _btree_arr[i]->locate(1111111, &h[i], &l[i]);
      // assert(l[i].node == NULL, "l should be NULL");
      // assert(h[i].current_index == h[i].node->get_num_entries()-1, "h current_index should be get_num_entries()-1");
    }

  }
  int total_entries_count = _btree_arr[0]->get_total_entries_count();
  int* f = new int[total_entries_count];
  memset(f, 0, sizeof(int) * total_entries_count);
  while (true) {
    for (int i = 0; i < _num_of_b_tree; i++) {
      int c = -1;
      if (!h[i].valid() && l[i].valid()) {
        c = l[i].key();
        f[c]++;
        l[i].next();
      }
      if (!l[i].valid() && h[i].valid()) {
        c = h[i].key();
        f[c]++;
        h[i].prev();
      }
      if (h[i].valid() && l[i].valid()) {
        assert(h[i].value() <= project_query[i], "h[i] <= q[i]");
        assert(l[i].value() >= project_query[i], "l[i] >= q[i]");
        if (fabs(project_query[i] - h[i].value()) < fabs(l[i].value() - project_query[i])) {
          c = h[i].key();
          f[c]++;
          h[i].prev();
        } else {
          c = l[i].key();
          f[c]++;
          l[i].next();
        }
      }

      if (f[c] >= MINFREQ * _num_of_b_tree) {
        delete [] h;
        delete [] l;
        return c;
      }
    }
  }
}

float Medrank::inner_product(float* vector_a, float* vector_b, int d) {
  float result = 0;
  for (int i = 0; i < d; i++) {
    result += vector_a[i] * vector_b[i];
  }
  return result;
}