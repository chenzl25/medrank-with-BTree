#include <cstdio>
#include <cmath>
void print(int* image) {
  printf("\n");
  for (int i = 0; i < 28; i++) {
    for (int j =0 ; j < 28; j++) {
      if (image[i*28+j] != 0) {
        printf("*");
      } else {
        printf(" ");
      }
    }
    printf("\n");
  }
  printf("\n");
}
double distance (int* x, int* y) {
  double result = 0;
  for (int i = 0; i < 28*28; i++) {
    result += (x[i] - y[i]) * (x[i] - y[i]);
  }
  return sqrt(result);
}
int main() {
  FILE *result_file, *nearest_file, *query_file;
  result_file = fopen("query-result.data", "r");
  int a[100][28*28];
  int b[100][28*28];
  int c[100][28*28];
  int d = 28*28;
  int n = 100;
  for (int i = 0; i < n; i++) {
    int tem;
    fscanf(result_file, "%d", &tem);
    for (int j = 0; j < d; j++) {
      fscanf(result_file, " %d", &a[i][j]);
    }
    fscanf(result_file, "\n");
  }
  nearest_file = fopen("./data/nearest.q", "r");
  for (int i = 0; i < n; i++) {
    int tem;
    fscanf(nearest_file, "%d", &tem);
    for (int j = 0; j < d; j++) {
      fscanf(nearest_file, " %d", &b[i][j]);
    }
    fscanf(nearest_file, "\n");
  }
  query_file = fopen("./data/Mnist.q", "r");
  for (int i = 0; i < n; i++) {
    int tem;
    fscanf(query_file, "%d", &tem);
    for (int j = 0; j < d; j++) {
      fscanf(query_file, " %d", &c[i][j]);
    }
    fscanf(query_file, "\n");
  }
  for (int i = 0; i < n; i++) {
    printf("i = %d\n", i);
    print(a[i]);
    print(b[i]);
    print(c[i]);
  }
  double avg = 0;
  for (int i = 0; i < n; i++) {
    double ratio = distance(a[i], c[i]) / distance(b[i], c[i]);
    avg += ratio;
    printf("ratio%d: %f\n", i, ratio);
  }
  printf("avg: %f", avg / n);

}