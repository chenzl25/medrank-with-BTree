#include "headers.h";
BNode::BNode() {

}            
BNode::~BNode() {

}

void BNode::init(int level, BTree* btree) {

}          
void BNode::init_restore(BTree* btree, int block_address) {

}           
void BNode::read_from_buffer(char* buf) {

}           
void BNode::write_to_buffer(char* buf) {

}           
int BNode::get_entry_size() {
  return -1;
} 
int BNode::find_position_by_value(float value) {
  return -1;
}    //get the pos just less than value       
float BNode::get_value(int index) {
  return -1;
}        
BNode* BNode::get_left_sibling() {
  return NULL;
}
BNode* BNode::get_right_sibling() {
  return NULL;
}
int BNode::get_block_address() {
  return _block_address;
}
int BNode::get_num_entries() {
  return _num_entries;
}    
int BNode::get_level() {
  return _level;
}
// _level:char _num_entries:int _left_sibling:int _right_sibling:int
int  BNode::get_header_size() {
  return sizeof(char) + sizeof(int)*3;
}

float BNode::get_value_of_node() {
  return _value[0];
}

bool BNode::isFull() {
  return _num_entries >= _capacity ? true: false;
}
void BNode::set_left_sibling(int left_sibling) {
  _left_sibling = left_sibling;
}
void BNode::set_right_sibling(int right_sibling) {
  _right_sibling = right_sibling;
}
char* BNode::put_buffer(char* destination, void* source, int num) {
  memcpy(destination, source, num);
  return destination + num;
}
char* BNode::get_buffer(void* destination, char* source, int num) {
  memcpy(destination, source, num);
  return source + num;
}

BIndexNode::BIndexNode() {
  _level= -1;
  _num_entries= -1;
  _left_sibling= _right_sibling= -1;

  _block_address= _capacity= -1;
  _dirty= false;
  _btree= NULL;

  _value= NULL;
  _son= NULL;
}

BIndexNode::~BIndexNode() {
  if (_dirty) {
    int block_length = _btree->_block_file->get_blocklength();
    char* blk = new char[block_length];
    write_to_buffer(blk);
    _btree->_block_file->write_block(blk, _block_address);
    delete [] blk;
  }
  delete [] _value;
  delete [] _son;
}

void BIndexNode::init(int level, BTree* btree) {
  _level = level;
  _btree = btree;
  _dirty = true;
  _num_entries = 0;
  _left_sibling = -1;
  _right_sibling = -1;
  int block_length = _btree->_block_file->get_blocklength();
  _capacity = (block_length - get_header_size()) / get_entry_size();
  assert(_capacity > 10, "fant out should more than 10");
  _value = new float[_capacity];
  _son   = new   int[_capacity];

  char* blk = new char[block_length];
  _block_address = _btree->_block_file->append_block(blk);
  delete [] blk;
}       
void BIndexNode::init_restore(BTree* btree, int block_address) {
  _block_address = block_address;
  _btree = btree;
  
  int block_length = _btree->_block_file->get_blocklength();
  _value = new float[_capacity];
  _son   = new   int[_capacity];
  _capacity = (block_length - get_header_size()) / get_entry_size();

  char* blk = new char[block_length];
  _btree->_block_file->read_block(blk, _block_address);
  read_from_buffer(blk);
  delete blk;
}           
void BIndexNode::read_from_buffer(char* buf) {
  buf = get_buffer(&_level, buf, sizeof(_level));
  buf = get_buffer(&_num_entries, buf, sizeof(_num_entries));
  buf = get_buffer(&_left_sibling, buf, sizeof(_left_sibling));
  buf = get_buffer(&_right_sibling, buf, sizeof(_right_sibling));
  buf = get_buffer(&_value, buf, sizeof(float) * _num_entries);
  buf = get_buffer(&_son, buf, sizeof(int) * _num_entries);
}           
void BIndexNode::write_to_buffer(char* buf) {
  // _level:char, _num_entries:int, _left_sibling:int, _right_sibling:int
  // _value:float[], _son:int[]
  buf = put_buffer(buf, &_level, sizeof(_level));
  buf = put_buffer(buf, &_num_entries, sizeof(_num_entries));
  buf = put_buffer(buf, &_left_sibling, sizeof(_left_sibling));
  buf = put_buffer(buf, &_right_sibling, sizeof(_right_sibling));
  buf = put_buffer(buf, &_value, sizeof(float) * _num_entries);
  buf = put_buffer(buf, &_son, sizeof(int) * _num_entries);
}           
int BIndexNode::find_position_by_value(float value) {
  // TODO change the linear scan to binary search
  for (int i = _num_entries-1; i >= 0; i--) {
    if (_value[i] <= value) {
      return i;
    }
  }
  return -1;
}                     
BIndexNode* BIndexNode::get_left_sibling() {
  assert(_btree != NULL, "_btree should not be NULL");
  if (_left_sibling == -1) {
    return NULL;
  }
  BIndexNode* b_index_node_ptr = new BIndexNode;
  b_index_node_ptr->init_restore(_btree, _left_sibling);
  return b_index_node_ptr;
} 
BIndexNode* BIndexNode::get_right_sibling() {
  assert(_btree != NULL, "_btree should not be NULL");
  if (_right_sibling == -1) {
    return NULL;
  }
  BIndexNode* b_index_node_ptr = new BIndexNode;
  b_index_node_ptr->init_restore(_btree, _right_sibling);
  return b_index_node_ptr;
}
int BIndexNode::get_son(int index) {
  assert(index >= 0 && index < _num_entries, "index of _son should be in range");
  return _son[index];
}        
void BIndexNode::add_new_child(float value, int son) {
  assert(_num_entries < _capacity, "add_new_child should have _num_entries < _capacity");
  _value[_num_entries] = value;
  _son[_num_entries] = son;
  _num_entries++;
  _dirty = true;
}

BLeafNode::BLeafNode() {
  
}          
BLeafNode::~BLeafNode() {

}     
void BLeafNode::init(int level, BTree* btree) {

}          
void BLeafNode::init_restore(BTree* btree, int block_address) {

}           
void BLeafNode::read_from_buffer(char* buf) {

}           
void BLeafNode::write_to_buffer(char* buf) {

}           
int BLeafNode::find_position_by_value(float value) {

}                     
BLeafNode*BLeafNode:: get_left_sibling() {

}
BLeafNode*BLeafNode:: get_right_sibling() {

}          
int BLeafNode::get_num_values() {

}       
int BLeafNode::get_entry_key(int index) {

}           
void BLeafNode::add_new_child(int key, float value) {

} 

