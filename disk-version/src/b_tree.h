#ifndef B_TREE_H
#define B_TREE_H
#include "headers.h"

struct project_entry;
class BNode;
class BNode_Cache;

class BTree {
public:
  BTree();
  ~BTree();
  void init(char* filename, int block_length);
  void init_restore(char* filename);
  void bulkload(project_entry* arr, int arr_size); //bulkload from data set
  void locate(float query_point, BNode_Cache* h, BNode_Cache* l, int *io_cost);
  int get_total_entries_count();
  BlockFile* _block_file;
private:
  void _inner_locate(BNode* from_node, float query_point, BNode_Cache* h, BNode_Cache* l, int *io_cost);
  void _create_dir_if_not_exist(char dirname[]);
  void _concat_dir_file(char dir_file[], char dir[], char file[]);
  void read_header(char* blk);
  void write_header(char* blk);
  int _root_address;
  int _total_entries_count;
  BNode* _root_ptr;
};
#endif