#include "headers.h"

BTree::BTree() {
  _root_address = -1;
  _block_file = NULL;
  _root_ptr = NULL;
}
BTree::~BTree() {
  char* blk = new char[_block_file->get_blocklength()];
  write_header(blk);
  _block_file->set_header(blk);

  delete [] blk;

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
  delete _root_ptr;
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
}

void BTree::bulkload(project_entry* arr, int arr_size) {
  printf("BTree bulkload...\n");
  clock_t startTime = (clock_t) -1;
  clock_t endTime   = (clock_t) -1;
  startTime = clock();
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
  }

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
        // float value = rand();
        // int key = rand();
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
  endTime = clock();
  printf("use %fs\n", (float)(endTime - startTime)/CLOCKS_PER_SEC);
}


void BTree::read_header(char* blk) {
  memcpy(&_root_address, blk, sizeof(int));
}
void BTree::write_header(char* blk) {
  memcpy(blk, &_root_address, sizeof(int));
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