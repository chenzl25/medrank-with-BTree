#include "headers.h"

struct project_entry;

void swap(project_entry* array, int i, int j) {
  project_entry tem;
  tem = array[i];
  array[i] = array[j];
  array[j] = tem;
}
int partition(project_entry* array, int l, int r) {
  assert(l <= r, "l should less than r");
  
  int rand_index = l + floor((r-l)*(rand()/RAND_MAX));
  swap(array, rand_index, l);
  int i, k;
  for (i = l+1, k = l; i <= r; i++) {
    if (array[i].value <= array[l].value) {
      k++;
      swap(array, k, i);
    }
  }
  swap(array, l, k);
  return k;
}

void quick_sort(project_entry* array,int l, int r) {
  if (r <= l) return;

  int index = partition(array, l, r);
  quick_sort(array, l, index-1);
  quick_sort(array, index+1, r);
}