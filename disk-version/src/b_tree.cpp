#include "headers.h"

BTree::BTree() {

}
BTree::~BTree() {

}

void BTree::init(char* filename, int block_length) {
  char b_tree_dir[30] = "./Btree_storage";
  _create_dir_if_not_exist(b_tree_dir);
  char b_tree_dir_file[50];
  _concat_dir_file(b_tree_dir_file, b_tree_dir, filename);
  _block_file = new BlockFile(b_tree_dir_file, block_length);

}

void BTree::bulkload(project_entry* arr, int arr_size) {
  
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