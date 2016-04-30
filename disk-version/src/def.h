#ifndef DEF_H
#define DEF_H

// -----------------------------------------------------------------------------
//  Typedefs
// -----------------------------------------------------------------------------
typedef char Block[];


// the b_tree entry
struct project_entry {
  int key;
  float value;
};

// -----------------------------------------------------------------------------
//  Macros
// -----------------------------------------------------------------------------

#define SEEK_SET 0
#define SEEK_CUR 1
#define SEEK_END 2

// -----------------------------------------------------------------------------
//  Constants
// -----------------------------------------------------------------------------
const float PI = 3.141592654F;
const int   BNODE_SIZE = 1024;

                  // size of type <int>
const int   SIZEINT   = (int) sizeof(int);
                  // size of type <char>
const int   SIZECHAR  = (int) sizeof(char);
                  // size of type <float>
const int   SIZEFLOAT = (int) sizeof(float);
                  // size of type <bool>
const int   SIZEBOOL  = (int) sizeof(bool);

const int   BFHEAD_LENGTH = (int) (sizeof(int) * 2);

#endif
