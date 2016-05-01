#include "headers.h"

BTree::BTree() {
  _root_address = -1;
  _block_file = NULL;
  _root_ptr = NULL;
  _total_entries_count = 0;
}
BTree::~BTree() {
  char* blk = new char[_block_file->get_blocklength()];
  write_header(blk);
  _block_file->set_header(blk);

  delete [] blk;
  delete _root_ptr;
  delete _block_file;
}

void BTree::init(char* filename, int block_length) {
  char b_tree_dir[30] = "./Btree_storage";
  _create_dir_if_not_exist(b_tree_dir);
  char b_tree_dir_file[50];
  _concat_dir_file(b_tree_dir_file, b_tree_dir, filename);
  _block_file = new BlockFile(b_tree_dir_file, block_length);
  _root_ptr = new BNode();
  _root_ptr->init(0, this);
  _root_address = _root_ptr->get_block_address();
}
void BTree::init_restore(char* filename) {
  char b_tree_dir[30] = "./Btree_storage";
  char b_tree_dir_file[50];
  _concat_dir_file(b_tree_dir_file, b_tree_dir, filename);
  FILE* btree_file;
  btree_file = fopen(b_tree_dir_file, "r");
  assert(btree_file != NULL, "init_restore should use the exist Btree file");
  _block_file = new BlockFile(b_tree_dir_file, 0);
  int block_length = _block_file->get_blocklength();
  char* blk = new char[block_length];
  _block_file->read_header(blk);
  read_header(blk);
  delete blk;
  _root_ptr = new BNode();
  _root_ptr->init_restore(this, _root_address);
  // printf("value of node : %f\n",_root_ptr->get_value_of_node());
  // printf("level: %d\n", _root_ptr->get_level());
  // printf("son[1]: %d\n", _root_ptr->get_son(2));
  // printf("get pos by value: %d\n", _root_ptr->find_position_by_value(-111111));
}

void BTree::bulkload(project_entry* arr, int arr_size) {
  // printf("BTree bulkload...\n");
  // clock_t startTime = (clock_t) -1;
  // clock_t endTime   = (clock_t) -1;
  // startTime = clock();

  int total_entries_count = 0;
  BNode* pre_left_node = NULL;
  BNode* now_left_node = NULL;

  int left_start_block_address = -1;
  int left_end_block_address   = -1;
  // THE 0 LEVEL
  for (int i = 0; i < arr_size; i++) {
    if (left_start_block_address == -1) {
      now_left_node = new BNode();
      now_left_node->init(0, this);
      left_start_block_address = now_left_node->get_block_address();
      left_end_block_address = left_start_block_address;
      pre_left_node = NULL;
      now_left_node->set_left_sibling(-1);
    }
    if (now_left_node->isFull()) {
      pre_left_node = now_left_node;
      now_left_node = new BNode();
      now_left_node->init(0, this);
      pre_left_node->set_right_sibling(now_left_node->get_block_address());
      now_left_node->set_left_sibling(pre_left_node->get_block_address());
      left_end_block_address = now_left_node->get_block_address();
      delete pre_left_node; pre_left_node = NULL;
    }
    now_left_node->add_new_child(arr[i].value, arr[i].key);
    total_entries_count++;
  }

  _total_entries_count = total_entries_count;

  if (now_left_node) {
    now_left_node->set_right_sibling(-1);
    delete now_left_node;
  } 

  // THE UPPER LEVEL
  int last_index_start_block_address = left_start_block_address;
  int last_index_end_block_address   = left_end_block_address;
  int current_level = 1;

  while (last_index_start_block_address < last_index_end_block_address) {
    int index_start_block_address = -1;
    int index_end_block_address   = -1;
    BNode* pre_index_node = NULL;
    BNode* now_index_node = NULL;
    for (int i = last_index_start_block_address; i <= last_index_end_block_address; i++) {
      if (index_start_block_address == -1) {
        now_index_node = new BNode();
        now_index_node->init(current_level, this);
        index_start_block_address = now_index_node->get_block_address();
        index_end_block_address = index_start_block_address;
        pre_index_node = NULL;
        now_index_node->set_left_sibling(-1);
      }
      if (now_index_node->isFull()) {
        pre_index_node = now_index_node;
        now_index_node = new BNode();
        now_index_node->init(current_level, this);
        pre_index_node->set_right_sibling(now_index_node->get_block_address());
        now_index_node->set_left_sibling(pre_index_node->get_block_address());
        index_end_block_address = now_index_node->get_block_address();
        delete pre_index_node; pre_index_node = NULL;
      }
      BNode* childNode = new BNode;

      childNode->init_restore(this, i);
      float value = childNode->get_value_of_node();
      int   key   = childNode->get_block_address();
      now_index_node->add_new_child(value, key);
    }
    if (now_index_node) {
      now_index_node->set_right_sibling(-1);
      delete now_index_node;
    }
    last_index_start_block_address = index_start_block_address;
    last_index_end_block_address = index_end_block_address;
    current_level++;
  }
  _root_address = last_index_start_block_address;
  // endTime = clock();
  // printf("use %fs\n", (float)(endTime - startTime)/CLOCKS_PER_SEC);
}

void BTree::locate(float query_point, BNode_Cache* h, BNode_Cache* l, int *io_cost) {
  assert(_root_ptr != NULL, "when BTree::locate _root_ptr should not be null");
  _inner_locate(_root_ptr, query_point, h, l, io_cost);
}

void BTree::_inner_locate(BNode* from_node, float query_point, BNode_Cache* h, BNode_Cache* l, int *io_cost) {
  assert(from_node != NULL, "when BTree::_inner_locate from_ptr should not be null");
  int index = from_node->find_position_by_value(query_point);
  if (from_node->get_level() == 0) {
    if (index == -1) {
      // here we need to set the l be the left most one in the BTree and h be NULL
      h->node = NULL;
      h->current_index = -1;
      h->io_cost = io_cost;
      l->node = new BNode();
      l->node->init_restore(this, from_node->get_block_address());
      l->io_cost = io_cost;
      *io_cost = *io_cost + 1;
      l->current_index = 0;
      if (from_node->get_block_address() != _root_address) delete from_node;
      return;
    }
    if (index == from_node->get_num_entries() - 1) {
      // here we need to set the h be the right most one in the BTree and l be NULL
      l->node = from_node->get_right_sibling();
      l->current_index = (l->node != NULL)? 0: -1;
      l->io_cost = io_cost;
      h->node = new BNode();
      h->node->init_restore(this, from_node->get_block_address());
      h->io_cost = io_cost;
      *io_cost = *io_cost + 1;
      h->current_index = from_node->get_num_entries() - 1;
      if (from_node->get_block_address() != _root_address) delete from_node;
      return;
    }
    // common case: index in the range (0, num_entries)
    h->node = new BNode();
    h->node->init_restore(this, from_node->get_block_address());
    h->io_cost = io_cost;
    *io_cost = *io_cost + 1;
    h->current_index = index;
    l->node = new BNode();
    l->node->init_restore(this, from_node->get_block_address());
    l->io_cost = io_cost;
    *io_cost = *io_cost + 1;
    l->current_index = index+1;
    if (from_node->get_block_address() != _root_address) delete from_node;
    return;
  }
  // level != 0
  if (index == -1) {
    // the point we want to find is less than any value in the BTree
    // we still go down the BTree, because we need to find the "h" and "l"
    index = 0;
  }
  BNode* childNode = new BNode();
  childNode->init_restore(this, from_node->get_son(index));
  *io_cost = *io_cost + 1;
  if (from_node->get_block_address() != _root_address) delete from_node;
  _inner_locate(childNode, query_point, h, l, io_cost);
}
int BTree::get_total_entries_count() {
  return _total_entries_count;
}
void BTree::read_header(char* blk) {
  memcpy(&_root_address, blk, sizeof(int));
  memcpy(&_total_entries_count, &blk[sizeof(int)], sizeof(int));
}
void BTree::write_header(char* blk) {
  memcpy(blk, &_root_address, sizeof(int));
  memcpy(&blk[sizeof(int)], &_total_entries_count, sizeof(int));
}

void BTree::_concat_dir_file(char dir_file[], char dir[], char file[]) {
  memcpy(dir_file, dir, strlen(dir)+1);
  strcat(dir_file, "/");
  strcat(dir_file, file);
}

void BTree::_create_dir_if_not_exist(char dirname[]) {
  int status;
  status = mkdir(dirname, S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH);
  if (status == 0) {
    // printf("create floader successfully\n");
  } else {
    struct stat sb;
    if (stat(dirname, &sb) == 0 && S_ISDIR(sb.st_mode))
    {
        // printf("directory exist\n");
    } else {
      printf("some error in ./Btree_storage directory\n");
    }
  }
}