#include "headers.h"

// #define RANK_LIST_SIZE ((28*28))
// #define RANDOM_VECTOR_SIZE (50)
// #define IMAGE_SIZE (60000)
// #define IMAGE_DIM (28*28)
// // #define PI  3.141592653
// // uncomment this define wiil use the random vector projection as voter
// // comment this define will lead to use the coordinate values as voter, which results in a poor result
// #define RANDOM_VECTOR_TEST

// int labels[IMAGE_SIZE];
// int images[IMAGE_SIZE][IMAGE_DIM];
// int rank_list[RANK_LIST_SIZE][IMAGE_SIZE]; 

// double random_vector[RANK_LIST_SIZE][IMAGE_DIM];
// double random_project_norm_list[RANK_LIST_SIZE][IMAGE_SIZE];
// int random_rank_list[RANK_LIST_SIZE][IMAGE_SIZE];


// int a[10] = {1,5,6,3,4,9,8,2,7,0};
// void swap(int i, int j) {
//   int tem;
//   tem = a[i];
//   a[i] = a[j];
//   a[j] = tem;
// }
// int partition(int l, int r) {
//   assert(l <= r, "l should less than r");
  
//   int rand_index = l + floor((r-l)*(rand()/RAND_MAX));
//   swap(rand_index, l);
//   int i, k;
//   for (i = l+1, k = l; i <= r; i++) {
//     if (a[i] <= a[l]) {
//       k++;
//       swap(k, i);
//     }
//   }
//   swap(l, k);
//   return k;
// }

// void quick_sort(int l, int r) {
//   if (r <= l) return;

//   int index = partition(l, r);
//   quick_sort(l, index-1);
//   quick_sort(index+1, r);

// }


// double distance (int x, int y) {
//   double result = 0;
//   for (int i = 0; i < IMAGE_DIM; i++) {
//     result += (images[x][i] - images[y][i]) * (images[x][i] - images[y][i]);
//   }
//   return sqrt(result);
// }

// int test_image_with_L2NN(int n) {
//   double min_ = 999999999;
//   int pos = -1;
//   for (int i = 0; i < IMAGE_SIZE; i++) {
//     if (i == n) continue;
//     double tem = distance(i, n);
//     if (tem < min_) {
//       min_ = tem;
//       pos = i;
//     }
//   }
//   return labels[pos] == labels[n];
// }

// void test_lots_L2NN (int n) {
//   int fail_number = 0;
//   int pass_number = 0;
//   for (int i = 0; i < n; i++) {
//     int pass = test_image_with_L2NN(i);
//     if (pass) {
//       pass_number++;
//     } else {
//       fail_number++;
//     }
//     printf("total_pass: %d, total_fail: %d\n", pass_number, fail_number);
//     // cout << "total_pass: " << pass_number << " total_fail: " << fail_number << endl;
//   }
// }

// // struct rank_list_cmp {
// //   int n;
// //   rank_list_cmp(int nn) {
// //     n = nn;
// //   }
// //   int operator () (int i, int j) {
// //     return images[i][n] < images[j][n];
// //   }
// // };

// void test_pre() {
//   for (int i = 0; i < 1; i++) {
//     for (int j = 1; j < IMAGE_SIZE; j++) {
//       assert(images[rank_list[i][j-1]][i] <= images[rank_list[i][j]][i], "sort should hold");
//     }
//   }
// }

// void test_random_vector() {
//   for (int i = 0; i < RANDOM_VECTOR_SIZE; i++) {
//     double avg = 0;
//     double norm = 0;
//     for (int j = 0; j < IMAGE_DIM; j++) {
//       avg += random_vector[i][j];
//       norm += random_vector[i][j] * random_vector[i][j];
//     }
//   }
// }

// void generate_random_vector() {
//   // cout << "generate_random_vector" << endl;
//   // default_random_engine e;
//   // normal_distribution<> n (0, 1);
//   for (int i = 0; i < RANDOM_VECTOR_SIZE; i++) {
//     double norm = 0;
//     for (int j = 0; j < IMAGE_DIM; j++) {
//       // random_vector[i][j] = n(e);
//       random_vector[i][j] = sqrt(-2*log(((double)rand())/RAND_MAX))*cos(2*PI*rand()/RAND_MAX);
//       norm += random_vector[i][j] * random_vector[i][j];
//     }
//     for (int j = 0; j < IMAGE_DIM; j++) {
//       random_vector[i][j] /= sqrt(norm);
//     }
//   }
// }

// double project_vector_norm(int x, int r) {
//   double result = 0;
//   for (int i = 0; i < IMAGE_DIM; i++) {
//     result += images[x][i] * random_vector[r][i];
//   }
//   return result;
// }

// // struct random_rank_list_cmp {
// //   int n;
// //   random_rank_list_cmp(int nn) {
// //     n = nn;
// //   }
// //   int operator()(int i, int j) {
// //     return random_project_norm_list[n][i] < random_project_norm_list[n][j];
// //   }
// // };

// void test_random_rank_list() {
//   // cout << "test_random_rank_list" << endl;
//   for (int i = 0; i < RANDOM_VECTOR_SIZE; i++) {
//     for (int j = 1; j < IMAGE_SIZE; j++) {
//       assert(random_project_norm_list[i][random_rank_list[i][j-1]] <= random_project_norm_list[i][random_rank_list[i][j]], "sort should hold");
//     }
//   }
// }

// void generate_random_rank_list() {
//   generate_random_vector();

//   for (int i = 0; i < RANDOM_VECTOR_SIZE; i++) {
//     printf("random_project_norm_list %d begin generate\n", i);
//     // cout << "random_project_norm_list " << i << " begin generate" << endl;
//     for (int j = 0; j < IMAGE_SIZE; j++) {
//       random_project_norm_list[i][j] = project_vector_norm(j, i);
//     }
//   }
//   // cout << "generate_random_rank_list" << endl;
//   for (int i = 0; i < RANDOM_VECTOR_SIZE; i++) {
//     printf("random_rank_list %d begin generate\n", i);
//     // cout << "random_rank_list " << i << " begin generate" << endl;
//     for (int j = 0; j < IMAGE_SIZE; j++) {
//       random_rank_list[i][j] = j;
//     }
//     // sort(random_rank_list[i], random_rank_list[i] + IMAGE_SIZE, random_rank_list_cmp(i));
//   }
//   test_random_rank_list();
// }

// void generate_coordinate_rank_list() {
//   for (int i = 0; i < RANK_LIST_SIZE; i++) {
//     for (int j = 0; j < IMAGE_SIZE; j++) {
//       rank_list[i][j] = j;
//     }
//     // sort(rank_list[i], rank_list[i] + IMAGE_SIZE, rank_list_cmp(i));
//   }
//   test_pre();
// }

// void MEDRANK_pre() {
// #ifdef RANDOM_VECTOR_TEST
//   generate_random_rank_list();
// #else
//   generate_coordinate_rank_list();
// #endif

// }


// int test_image_with_MEDRANK_coordinate (int n) {
//   // cout << "test " << n << endl;
//   int* query = images[n];
//   int pos = -1;
//   int f[IMAGE_SIZE];
//   memset(f, 0, sizeof(f));
//   double MINFREQ = 0.5;
//   int h[IMAGE_DIM];
//   int l[IMAGE_DIM];
//   // initilize cursor, cursor maybe out of range.
//   for (int i = 0; i < RANK_LIST_SIZE; i++) {
//     // locate q
//     for (int j = 0; j < IMAGE_SIZE; j++) {
//       if (rank_list[i][j] == n) {
//         h[i] = j-1;
//         l[i] = j+1;
//         break;
//       }
//     }
//   }
//   while (1) {
//     for (int i = 0; i < RANK_LIST_SIZE; i++) {
//       int c = -1;
//       if (h[i] < 0 && l[i] >= IMAGE_SIZE) continue;
//       if (h[i] < 0) {
//         c = rank_list[i][l[i]++];
//         f[c]++;
//       }
//       if (l[i] >= IMAGE_SIZE) {
//         c = rank_list[i][h[i]--];
//         f[c]++;
//       }
//       if (h[i] >= 0 && l[i] < IMAGE_SIZE) {
//         assert(images[rank_list[i][h[i]]][i] <= query[i], "h should less than query");
//         assert(images[rank_list[i][l[i]]][i] >= query[i], "l should greater than query");
//         if (abs(images[rank_list[i][h[i]]][i]-query[i]) < abs(images[rank_list[i][l[i]]][i]-query[i])) {
//           c = rank_list[i][h[i]--];
//           f[c]++;
//         } else {
//           c = rank_list[i][l[i]++];
//           f[c]++;
//         }
//       }
//       if (f[c] >= MINFREQ * RANK_LIST_SIZE) {
//         pos = c;
//         return labels[pos] == labels[n];
//       }
//     } 
//   }
// }
// int test_image_with_MEDRANK_random_projection(int n) {
//   // n represent the nth image
//   // cout << "test " << n << endl;
//   int* query = images[n];
//   int pos = -1;
//   int f[IMAGE_SIZE];
//   memset(f, 0, sizeof(f));
//   double MINFREQ = 0.5;
//   int h[IMAGE_DIM];
//   int l[IMAGE_DIM];
//   // initilize cursor, cursor maybe out of range.
//   for (int i = 0; i < RANDOM_VECTOR_SIZE; i++) {
//     // locate q
//     for (int j = 0; j < IMAGE_SIZE; j++) {
//       if (random_rank_list[i][j] == n) {
//         h[i] = j-1;
//         l[i] = j+1;
//         break;
//       }
//     }
//   }
//   while (1) {
//     for (int i = 0; i < RANDOM_VECTOR_SIZE; i++) {
//       int c = -1;
//       if (h[i] < 0 && l[i] >= IMAGE_SIZE) continue;
//       if (h[i] < 0) {
//         c = random_rank_list[i][l[i]++];
//         f[c]++;
//       }
//       if (l[i] >= IMAGE_SIZE) {
//         c = random_rank_list[i][h[i]--];
//         f[c]++;
//       }
//       if (h[i] >= 0 && l[i] < IMAGE_SIZE) {
//         assert(random_project_norm_list[i][random_rank_list[i][h[i]]] <=
//                random_project_norm_list[i][n], "h should less than query");
//         assert(random_project_norm_list[i][random_rank_list[i][l[i]]] >= 
//                random_project_norm_list[i][n], "l should greater than query");
//         if (fabs(random_project_norm_list[i][random_rank_list[i][h[i]]]-random_project_norm_list[i][n]) <
//             fabs(random_project_norm_list[i][n]-random_project_norm_list[i][random_rank_list[i][l[i]]])) {
//           c = random_rank_list[i][h[i]--];
//           f[c]++;
//         } else {
//           c = random_rank_list[i][l[i]++];
//           f[c]++;
//         }
//       }

//       if (f[c] >= MINFREQ * RANDOM_VECTOR_SIZE) {
//         pos = c;
//         return labels[pos] == labels[n];
//       }
//     } 
//   }
// }
// void test_lots_MEDRANK (int n) {
//   // cout << "begin pre" << endl;
//   MEDRANK_pre();
//   // cout << "finish pre" << endl;  
//   int fail_number = 0;
//   int pass_number = 0;
//   for (int i = 0; i < n; i++) {
// #ifdef RANDOM_VECTOR_TEST
//     int pass = test_image_with_MEDRANK_random_projection(i);
// #else
//     int pass = test_image_with_MEDRANK_coordinate(i);
// #endif
//     if (pass) {
//       pass_number++;
//     } else {
//       fail_number++;
//     }
//     printf("total_pass: %d, total_fail: %d\n", pass_number, fail_number);
//     // cout << "total_pass: " << pass_number << " total_fail: " << fail_number << endl;
//   }
// }

// void print_image(int n) {
//   // cout << endl;
//   // cout << "label: " << labels[n] << endl;
//   // for (int j = 0; j < IMAGE_DIM; j++) {
//   //   if (j % 28 == 0) {
//   //     cout << endl;
//   //   }
//   //   cout << (((int)images[n][j] > 0) ? "*" : " " )<< " ";
//   // }
//   // cout << endl;
// }

// // void read_labels() {
// //   string s;
// //   ifstream label_in("train-labels-idx1-ubyte");
// //   getline(label_in, s);
// //   for (int i = 8; i < s.length(); i++) {
// //     labels[i-8] = (int)s[i];
// //   }
// // }
// int query[100][28*28];
// double test_distance(int x, int y) {
//   double result = 0;
//   for (int i = 0; i < IMAGE_DIM; i++) {
//     result += (images[x][i] - query[y][i]) * (images[x][i] - query[y][i]);
//   }
//   return sqrt(result);
// }


void init(float*** datas_ptr, float*** querys_ptr, int n, int d, int qn);

int main(int argc, char const *argv[]) {
  srand(time(NULL));
  int n = 60000;    // data size
  int d = 784;      // data dimension
  int qn = 100;     // query size
  char Mnist_ds[30] = "./data/Mnist.ds"; // path of data file
  char Mnist_q[30] = "./data/Mnist.q";   // path of query file
  for (int i = 1; i < argc; i++) {
    if (strcmp(argv[i], "-n") == 0) {
      n = atoi(argv[i+1]);
    } else if (strcmp(argv[i], "-d") == 0) {
      d = atoi(argv[i+1]);
    } else if (strcmp(argv[i], "-qn") == 0) {
      qn = atoi(argv[i+1]);
    } else if (strcmp(argv[i], "-ds") == 0) {
      memcpy(Mnist_ds, argv[i+1], sizeof(Mnist_ds));
    } else if (strcmp(argv[i], "-qs") == 0) {
      memcpy(Mnist_q, argv[i+1], sizeof(Mnist_ds));
    }
  }

  const float MINFREQ = 0.5;
  const int   num_of_random_vectors = 50;
  


  // indexing_from_data_set(Mnist_ds, n, d, num_of_random_vectors);

  medrank_test(Mnist_q, qn, d, num_of_random_vectors);


  // load the labels data
  // read_labels();
  // load the images data
  // read_mnist();

  // test_lots_MEDRANK(100);
  

  // test_lots_L2NN(100);

  /** 
   *  This function can print the image to the console
   *  where i is the ith image.
   */
   // for (int i = 0; i < 10; i++)
   //  print_image(i);
   return 0;
}






