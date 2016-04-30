#include <iostream>
#include <cstdio>
#include <cstring>
#include <fstream>
#include <cmath>
#include <algorithm>
#include <cstdlib>
#include <random>

#define RANK_LIST_SIZE ((28*28))
#define RANDOM_VECTOR_SIZE (50)  // 30, 50, 200
#define IMAGE_SIZE (60000)
#define IMAGE_DIM (28*28)

// uncomment this define wiil use the random vector projection as voter
// comment this define will lead to use the coordinate values as voter, which results in a poor result
#define RANDOM_VECTOR_TEST

using namespace std;
int labels[IMAGE_SIZE];
int images[IMAGE_SIZE][IMAGE_DIM];
int rank_list[RANK_LIST_SIZE][IMAGE_SIZE]; 

double random_vector[RANK_LIST_SIZE][IMAGE_DIM];
double random_project_norm_list[RANK_LIST_SIZE][IMAGE_SIZE];
int random_rank_list[RANK_LIST_SIZE][IMAGE_SIZE];

void assert(bool ok) {
  if (!ok) {
    cout << "error" << endl;
    exit(-1);
  }
}


int reverseInt (int i) 
{
    unsigned char c1, c2, c3, c4;

    c1 = i & 255;
    c2 = (i >> 8) & 255;
    c3 = (i >> 16) & 255;
    c4 = (i >> 24) & 255;

    return ((int)c1 << 24) + ((int)c2 << 16) + ((int)c3 << 8) + c4;
}
void read_mnist(/*string full_path*/)
{
    cout << "read_mnist" << endl;
    ifstream file (/*full_path*/"train-images-idx3-ubyte");
    if (file.is_open())
    {
        int magic_number=0;
        int number_of_images=0;
        int n_rows=0;
        int n_cols=0;
        file.read((char*)&magic_number,sizeof(magic_number)); 
        magic_number= reverseInt(magic_number);
        file.read((char*)&number_of_images,sizeof(number_of_images));
        number_of_images= reverseInt(number_of_images);
        file.read((char*)&n_rows,sizeof(n_rows));
        n_rows= reverseInt(n_rows);
        file.read((char*)&n_cols,sizeof(n_cols));
        n_cols= reverseInt(n_cols);
        int count = 0;
        for(int i=0;i<number_of_images;++i)
        {
            for(int r=0;r<n_rows;++r)
            {
                for(int c=0;c<n_cols;++c)
                {
                    unsigned char temp=0;
                    file.read((char*)&temp,sizeof(temp));
                    images[i][n_cols*r + c] = temp;
                }
            }
        }
    }
}

double distance (int x, int y) {
  double result = 0;
  for (int i = 0; i < IMAGE_DIM; i++) {
    result += (images[x][i] - images[y][i]) * (images[x][i] - images[y][i]);
  }
  return sqrt(result);
}

bool test_image_with_L2NN(int n) {
  double min_ = 999999999;
  int pos = -1;
  for (int i = 0; i < IMAGE_SIZE; i++) {
    if (i == n) continue;
    double tem = distance(i, n);
    if (tem < min_) {
      min_ = tem;
      pos = i;
    }
  }
  return labels[pos] == labels[n];
}

void test_lots_L2NN (int n) {
  int fail_number = 0;
  int pass_number = 0;
  for (int i = 0; i < n; i++) {
    bool pass = test_image_with_L2NN(i);
    if (pass) {
      pass_number++;
    } else {
      fail_number++;
    }
    cout << "total_pass: " << pass_number << " total_fail: " << fail_number << endl;
  }
}

struct rank_list_cmp {
  int n;
  rank_list_cmp(int nn) {
    n = nn;
  }
  bool operator () (int i, int j) {
    return images[i][n] < images[j][n];
  }
};

void test_pre() {
  for (int i = 0; i < 1; i++) {
    for (int j = 1; j < IMAGE_SIZE; j++) {
      assert(images[rank_list[i][j-1]][i] <= images[rank_list[i][j]][i]);
    }
  }
}

void test_random_vector() {
  for (int i = 0; i < RANDOM_VECTOR_SIZE; i++) {
    double avg = 0;
    double norm = 0;
    for (int j = 0; j < IMAGE_DIM; j++) {
      avg += random_vector[i][j];
      norm += random_vector[i][j] * random_vector[i][j];
    }
  }
}

void generate_random_vector() {
  cout << "generate_random_vector" << endl;
  default_random_engine e;
  normal_distribution<> n (0, 1);
  for (int i = 0; i < RANDOM_VECTOR_SIZE; i++) {
    double norm = 0;
    for (int j = 0; j < IMAGE_DIM; j++) {
      random_vector[i][j] = n(e);
      norm += random_vector[i][j] * random_vector[i][j];
    }
    for (int j = 0; j < IMAGE_DIM; j++) {
      random_vector[i][j] /= sqrt(norm);
    }
  }
}

double project_vector_norm(int x, int r) {
  double result = 0;
  for (int i = 0; i < IMAGE_DIM; i++) {
    result += images[x][i] * random_vector[r][i];
  }
  return result;
}

struct random_rank_list_cmp {
  int n;
  random_rank_list_cmp(int nn) {
    n = nn;
  }
  bool operator()(int i, int j) {
    return random_project_norm_list[n][i] < random_project_norm_list[n][j];
  }
};

void test_random_rank_list() {
  cout << "test_random_rank_list" << endl;
  for (int i = 0; i < RANDOM_VECTOR_SIZE; i++) {
    for (int j = 1; j < IMAGE_SIZE; j++) {
      assert(random_project_norm_list[i][random_rank_list[i][j-1]] <= random_project_norm_list[i][random_rank_list[i][j]]);
    }
  }
}

void generate_random_rank_list() {
  generate_random_vector();

  for (int i = 0; i < RANDOM_VECTOR_SIZE; i++) {
    cout << "random_project_norm_list " << i << " begin generate" << endl;
    for (int j = 0; j < IMAGE_SIZE; j++) {
      random_project_norm_list[i][j] = project_vector_norm(j, i);
    }
  }
  cout << "generate_random_rank_list" << endl;
  for (int i = 0; i < RANDOM_VECTOR_SIZE; i++) {
    cout << "random_rank_list " << i << " begin generate" << endl;
    for (int j = 0; j < IMAGE_SIZE; j++) {
      random_rank_list[i][j] = j;
    }
    sort(random_rank_list[i], random_rank_list[i] + IMAGE_SIZE, random_rank_list_cmp(i));
  }
  test_random_rank_list();
}

void generate_coordinate_rank_list() {
  for (int i = 0; i < RANK_LIST_SIZE; i++) {
    for (int j = 0; j < IMAGE_SIZE; j++) {
      rank_list[i][j] = j;
    }
    sort(rank_list[i], rank_list[i] + IMAGE_SIZE, rank_list_cmp(i));
  }
  test_pre();
}

void MEDRANK_pre() {
#ifdef RANDOM_VECTOR_TEST
  generate_random_rank_list();
#else
  generate_coordinate_rank_list();
#endif

}


bool test_image_with_MEDRANK_coordinate (int n) {
  cout << "test " << n << endl;
  int* query = images[n];
  int pos = -1;
  int f[IMAGE_SIZE];
  memset(f, 0, sizeof(f));
  double MINFREQ = 0.5;
  int h[IMAGE_DIM];
  int l[IMAGE_DIM];
  // initilize cursor, cursor maybe out of range.
  for (int i = 0; i < RANK_LIST_SIZE; i++) {
    // locate q
    for (int j = 0; j < IMAGE_SIZE; j++) {
      if (rank_list[i][j] == n) {
        h[i] = j-1;
        l[i] = j+1;
        break;
      }
    }
  }
  while (true) {
    for (int i = 0; i < RANK_LIST_SIZE; i++) {
      int c = -1;
      if (h[i] < 0 && l[i] >= IMAGE_SIZE) continue;
      if (h[i] < 0) {
        c = rank_list[i][l[i]++];
        f[c]++;
      }
      if (l[i] >= IMAGE_SIZE) {
        c = rank_list[i][h[i]--];
        f[c]++;
      }
      if (h[i] >= 0 && l[i] < IMAGE_SIZE) {
        assert(images[rank_list[i][h[i]]][i] <= query[i]);
        assert(images[rank_list[i][l[i]]][i] >= query[i]);
        if (abs(images[rank_list[i][h[i]]][i]-query[i]) < abs(images[rank_list[i][l[i]]][i]-query[i])) {
          c = rank_list[i][h[i]--];
          f[c]++;
        } else {
          c = rank_list[i][l[i]++];
          f[c]++;
        }
      }
      if (f[c] >= MINFREQ * RANK_LIST_SIZE) {
        pos = c;
        return labels[pos] == labels[n];
      }
    } 
  }
}
bool test_image_with_MEDRANK_random_projection(int n) {
  // n represent the nth image
  cout << "test " << n << endl;
  int* query = images[n];
  int pos = -1;
  int f[IMAGE_SIZE];
  memset(f, 0, sizeof(f));
  double MINFREQ = 0.5;
  int h[IMAGE_DIM];
  int l[IMAGE_DIM];
  // initilize cursor, cursor maybe out of range.
  for (int i = 0; i < RANDOM_VECTOR_SIZE; i++) {
    // locate q
    for (int j = 0; j < IMAGE_SIZE; j++) {
      if (random_rank_list[i][j] == n) {
        h[i] = j-1;
        l[i] = j+1;
        break;
      }
    }
  }
  while (true) {
    for (int i = 0; i < RANDOM_VECTOR_SIZE; i++) {
      int c = -1;
      if (h[i] < 0 && l[i] >= IMAGE_SIZE) continue;
      if (h[i] < 0) {
        c = random_rank_list[i][l[i]++];
        f[c]++;
      }
      if (l[i] >= IMAGE_SIZE) {
        c = random_rank_list[i][h[i]--];
        f[c]++;
      }
      if (h[i] >= 0 && l[i] < IMAGE_SIZE) {
        assert(random_project_norm_list[i][random_rank_list[i][h[i]]] <=
               random_project_norm_list[i][n]);
        assert(random_project_norm_list[i][random_rank_list[i][l[i]]] >= 
               random_project_norm_list[i][n]);
        if (fabs(random_project_norm_list[i][random_rank_list[i][h[i]]]-random_project_norm_list[i][n]) <
            fabs(random_project_norm_list[i][n]-random_project_norm_list[i][random_rank_list[i][l[i]]])) {
          c = random_rank_list[i][h[i]--];
          f[c]++;
        } else {
          c = random_rank_list[i][l[i]++];
          f[c]++;
        }
      }

      if (f[c] >= MINFREQ * RANDOM_VECTOR_SIZE) {
        pos = c;
        return labels[pos] == labels[n];
      }
    } 
  }
}
void test_lots_MEDRANK (int n) {
  cout << "begin pre" << endl;
  MEDRANK_pre();
  cout << "finish pre" << endl;
  int fail_number = 0;
  int pass_number = 0;
  for (int i = 0; i < n; i++) {
#ifdef RANDOM_VECTOR_TEST
    bool pass = test_image_with_MEDRANK_random_projection(i);
#else
    bool pass = test_image_with_MEDRANK_coordinate(i);
#endif
    if (pass) {
      pass_number++;
    } else {
      fail_number++;
    }
    cout << "total_pass: " << pass_number << " total_fail: " << fail_number << endl;
  }
}

void print_image(int n) {
  cout << endl;
  cout << "label: " << labels[n] << endl;
  for (int j = 0; j < IMAGE_DIM; j++) {
    if (j % 28 == 0) {
      cout << endl;
    }
    cout << (((int)images[n][j] > 0) ? "*" : " " )<< " ";
  }
  cout << endl;
}

void read_labels() {
  string s;
  ifstream label_in("train-labels-idx1-ubyte");
  getline(label_in, s);
  for (int i = 8; i < s.length(); i++) {
    labels[i-8] = (int)s[i];
  }
}

void format_file () {
  read_mnist();

  ofstream out("Mnist.ds");

  for (int i = 0; i < 60000; i++) {
    out << i+1 << " ";
    for (int j = 0; j < 28*28; j++) {
      out << images[i][j];
      if (j != 28*28-1) out << " ";
    }
    out << endl;
  }

  cout << "read_mnist" << endl;
    ifstream file (/*full_path*/"t10k-images-idx3-ubyte");
    if (file.is_open())
    {
        int magic_number=0;
        int number_of_images=0;
        int n_rows=0;
        int n_cols=0;
        file.read((char*)&magic_number,sizeof(magic_number)); 
        magic_number= reverseInt(magic_number);
        file.read((char*)&number_of_images,sizeof(number_of_images));
        number_of_images= reverseInt(number_of_images);
        file.read((char*)&n_rows,sizeof(n_rows));
        n_rows= reverseInt(n_rows);
        file.read((char*)&n_cols,sizeof(n_cols));
        n_cols= reverseInt(n_cols);
        int count = 0;
        cout << magic_number << endl;
        cout << number_of_images << endl;
        cout << n_rows << endl;
        cout << n_cols << endl;
        for(int i=0;i<number_of_images;++i)
        {
            for(int r=0;r<n_rows;++r)
            {
                for(int c=0;c<n_cols;++c)
                {
                    unsigned char temp=0;
                    file.read((char*)&temp,sizeof(temp));
                    images[i][n_cols*r + c] = temp;
                }
            }
        }
        ofstream out("Mnist.q");

        for (int i = 0; i < 100; i++) {
          out << i+1 << " ";
          for (int j = 0; j < 28*28; j++) {
            out << images[i][j];
            if (j != 28*28-1) out << " ";
          }
          out << endl;
        }
    }
}

int main() {
  // load the labels data
  // read_labels();
  // load the images data
  // read_mnist();



  // test_lots_MEDRANK(1000);
  // test_lots_L2NN(1000);

  // ofstream out("image-file.txt");
  // for (int i = 0; i < IMAGE_SIZE; i++) {
  //   for (int j = 0; j < IMAGE_DIM; j++) {
  //     out << images[i][j];
  //     if (j != IMAGE_DIM -1) out << ",";
  //   }
  //   out << endl;
  // }

  // for (int i = 0; i < IMAGE_DIM; i++) {
  //   cout << images[0][i] << ",";
  // }

  /** 
   *  This function can print the image to the console
   *  where i is the ith image.
   */
   // for (int i = 0; i < 10; i++)
   //  print_image(i);

}