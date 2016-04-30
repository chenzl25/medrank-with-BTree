#include "headers.h"

float gaussian_rand() {
  return sqrt(-2*log(((double)rand())/RAND_MAX))*cos(2*PI*rand()/RAND_MAX);
}