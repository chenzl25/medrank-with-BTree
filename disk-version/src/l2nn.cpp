#include "headers.h"

L2NN::L2NN() {

}
L2NN::~L2NN() {

}
void L2NN::init(float** datas, int n, int d) {
  _datas = datas;
  _n = n;
  _d = d;
}
int L2NN::search(float* query) {
  float min_ = 99999999;
  int pos = -1;
  for (int i = 0; i < _n; i++) {
    float temp = distance(query, _datas[i], _d);
    if (temp < min_) {
      min_ = temp;
      pos = i;
    }
  }
  return pos;
}

float L2NN::distance (float* x, float* y, int d) {
  float result = 0;
  for (int i = 0; i < d; i++) {
    result += (x[i] - y[i]) * (x[i] - y[i]);
  }
  return sqrt(result);
}