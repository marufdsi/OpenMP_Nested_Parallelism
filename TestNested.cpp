#include<iostream>
#include <omp.h>
#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <vector>

using namespace std;

int L= 4;
int sizeX = 100000;
int sizeY = 10;

int main(int argc, char *argv[]){
  omp_set_dynamic(0);
  omp_set_nested(1);
//	cout<< "start Program" <<endl;
  float a[sizeX][sizeY];
  float b[sizeX];
//	vector<double> c(sizeX);
  float c[sizeX];
  int i, j, k;
  srand(time(NULL));
//	cout<<"Start Initialization" << endl;
  for(i=0; i<sizeX; ++i){
    for(j=0; j<sizeY; ++j){
      a[i][j] = rand()%30 + 1;
    }
    b[i] = rand()%20 + 2;
  }
//	cout<<"Initialization done" << endl;
  int outer_threads = (int)std::stoul(argv[1]);
  int inner_threads = (int)std::stoul(argv[2]);
  struct timespec start, end;
  clock_gettime(CLOCK_MONOTONIC, &start);
#pragma omp parallel for private(i, j, k) schedule(static) num_threads(outer_threads)
  for(k=0; k<L; ++k){
#pragma omp parallel for private(i, j) schedule(guided) num_threads(inner_threads)
    for(i=0; i<sizeX; ++i){
	    //		#pragma omp parallel for schedule(guided) num_threads(inner_threads)
      double tmp = 0;
      for(j=0; j<sizeY; ++j){
	tmp += a[i][j]*b[i] + a[i][j];
      }
      c[i] = tmp/k+1;
    }
  }
	
  clock_gettime(CLOCK_MONOTONIC, &end);
  double time = ((end.tv_sec * 1000 + (end.tv_nsec / 1.0e6)) - (start.tv_sec * 1000 + (start.tv_nsec / 1.0e6)));
  double res = 0;
  for(i=0; i<sizeX; ++i){
    res += c[i]/30 + b[i]/20 + (i*2);
  }
  cout<< "result: " << res << " L: " << L <<  " X: " << sizeX << " Y: " << sizeY << " outer threads: " << outer_threads << " inner threads: " << inner_threads << " time: " << time*1000 << "s" << endl;

  return 0;
}

