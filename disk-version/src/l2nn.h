#ifndef L2NN_H
#define L2NN_H
class L2NN {
public:
  L2NN();
  ~L2NN();
  void init(float** datas, int n, int d);
  int search(float* query);
private:
  float distance (float* x, float* y, int d);
  float** _datas;
  int _n;
  int _d;
};

#endif