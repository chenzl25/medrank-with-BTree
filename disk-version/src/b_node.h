#ifndef B_NODE_H
#define B_NODE_H

class BlockFile;
class BTree;

class BNode {
public:
  BNode();            
  virtual ~BNode();      
  virtual void init(int level, BTree* btree);          
  virtual void init_restore(BTree* btree, int block_address);           
  virtual void read_from_buffer(char* buf);           
  virtual void write_to_buffer(char* buf);           
  virtual int find_position_by_value(float value);    //get the pos just less than value       
  virtual float get_value(int index);        
  virtual BNode* get_left_sibling();
  virtual BNode* get_right_sibling();
  int get_entry_size(); 
  int get_block_address();
  int get_num_entries();      
  int get_level();    
  int  get_header_size();     
  float get_value_of_node();      
  bool isFull();          
  void set_left_sibling(int left_sibling);        
  void set_right_sibling(int right_sibling);
  char* put_buffer(char* destination, void* source, int num);
  char* get_buffer(void* destination, char* source, int num);      
protected:
  char   _level;          
  int    _num_entries;      
  int    _left_sibling;     
  int    _right_sibling;      
  float* _value;          
  bool   _dirty;          
  int    _block_address;          
  int    _capacity;       
  BTree* _btree;          
};

class BIndexNode : public BNode {
public:
  BIndexNode();         
  virtual ~BIndexNode();      
  virtual void init(int level, BTree* btree);          
  virtual void init_restore(BTree* btree, int block_address);           
  virtual void read_from_buffer(char* buf);           
  virtual void write_to_buffer(char* buf);           
  virtual int find_position_by_value(float value);                   
  virtual BIndexNode* get_left_sibling(); 
  virtual BIndexNode* get_right_sibling();
  int get_son(int index);           
  void add_new_child(float value, int son);           
protected:
  int* _son;            
};

class BLeafNode : public BNode {
public:
  BLeafNode();          
  virtual ~BLeafNode();     
  virtual void init(int level, BTree* btree);          
  virtual void init_restore(BTree* btree, int block_address);           
  virtual void read_from_buffer(char* buf);           
  virtual void write_to_buffer(char* buf);           
  virtual int find_position_by_value(float value);                  
  virtual BLeafNode* get_left_sibling();
  virtual BLeafNode* get_right_sibling();
  int get_num_values();       
  int get_entry_key(int index);           
  void add_new_child(int key, float value);           
protected:
  int  _num_values;         
  int* _key;           
  int _capacity_values;       
};

#endif
