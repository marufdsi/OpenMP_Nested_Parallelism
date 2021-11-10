#include<iostream>
#include <omp.h>
#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <vector>
#include <sys/stat.h>
#include <fstream>

using namespace std;

/// int L= 4;
/// int sizeX = 100000;
/// int sizeY = 10;

int main(int argc, char *argv[]){

  if(argc <6){
    cout<< "************************** Usages ***********************" << endl;
    cout<< argv[0] << "OuterLoopSize InnerLoopSize LastLoopSize OuterThreads InnerThreads" << endl;
    cout<< "*********************************************************" << endl;
    return 0;
  }

  int L = (int)std::stoul(argv[1]);
  int sizeX = (int)std::stoul(argv[2]);
  int sizeY = (int)std::stoul(argv[3]);
  omp_set_dynamic(0);
  omp_set_nested(1);
//	cout<< "start Program" <<endl;
  float a[sizeX][sizeY];
  float b[sizeX];
//	vector<double> c(sizeX);
  float c[L];
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
  int outer_threads = (int)std::stoul(argv[4]);
  int inner_threads = (int)std::stoul(argv[5]);
  struct timespec start, end;
  clock_gettime(CLOCK_MONOTONIC, &start);
#pragma omp parallel for private(i, j, k) schedule(static) num_threads(outer_threads)
  for(k=0; k<L; ++k){
      float sum = 0;
#pragma omp parallel for private(i, j) reduction (+:sum) schedule(guided) num_threads(inner_threads)
    for(i=0; i<sizeX; ++i){
	    //		#pragma omp parallel for schedule(guided) num_threads(inner_threads)
      double tmp = 0;
      for(j=0; j<sizeY; ++j){
	    tmp += a[i][j]*b[i] + a[i][j];
      }
      sum += (tmp/(k+1));
    }
    c[k] = sum;
  }
	
  clock_gettime(CLOCK_MONOTONIC, &end);
  double time = ((end.tv_sec * 1000 + (end.tv_nsec / 1.0e6)) - (start.tv_sec * 1000 + (start.tv_nsec / 1.0e6)));
  double res = 0;
  for(i=0; i<L; ++i) {
      for (j = 0; j < sizeX; ++j) {
          res += c[i] / 30 + b[j] / 20 + (i * 2);
      }
  }
  string folderName = "Results/";
  if (mkdir(folderName.c_str(), 0777) == -1)
    std::cout << "Directory " << folderName << " is already exist" << std::endl;
  else
    std::cout << "Directory " << folderName << " created" << std::endl;
  string logFileName = "Results/Log.csv";
  std::ifstream infile(logFileName);
  bool exist = infile.good();
  std::ofstream log;
  log.open(logFileName, std::ios_base::out | std::ios_base::app | std::ios_base::ate);
  if(!exist){
    log<< "OuterLoopSize,InnerLoopSize,LastLoopSize,TotalThreads,OuterThreads,InnerThreads,Time" <<endl;
  }
  infile.close();
  log<< L << "," << sizeX << "," << sizeY << "," << omp_get_max_threads() << "," << outer_threads << "," << inner_threads << "," << time*1000  << endl;
  log.close();
  
  cout<< "result: " << res << " L: " << L <<  " X: " << sizeX << " Y: " << sizeY << " outer threads: " << outer_threads << " inner threads: " << inner_threads << " time: " << time*1000 << "s" << endl;

  return 0;
}

