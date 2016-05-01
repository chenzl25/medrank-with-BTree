 #include "headers.h"

 BNode_Cache::BNode_Cache() {
  node = NULL;
  current_index = -1;
}
BNode_Cache::~BNode_Cache() {
  if (node != NULL)
    delete node;
}

bool BNode_Cache::valid() {
  return node != NULL && current_index != -1;
}
void BNode_Cache::prev() {
  assert(valid(), "BNode_Cache should be valid");
  if (current_index == 0) {
    BNode* prev_block = NULL;
    prev_block = node->get_left_sibling();
    if (prev_block == NULL) current_index = -1;
    else {
      current_index = prev_block->get_num_entries() -1;
      *io_cost = *io_cost + 1;
    }
    delete node;
    node = prev_block; prev_block = NULL;
  }
  current_index--;
}
void BNode_Cache::next() {
  assert(valid(), "BNode_Cache should be valid");
  if (node->get_num_entries() - 1 == current_index) {
    BNode* next_block = NULL;
    next_block = node->get_right_sibling();
    if (next_block == NULL) current_index = -1;
    else{
      current_index = 0;
      *io_cost = *io_cost + 1;
    } 
    delete node;
    node = next_block; next_block = NULL;
  }
  current_index++;
}
int BNode_Cache::key() {
  assert(valid(), "BNode_Cache should be valid");
  return node->get_son(current_index);
}

float BNode_Cache::value() {
  assert(valid(), "BNode_Cache should be valid");
  return node->get_value(current_index);
}
