#include <math.h>
#include <time.h>
#include <string.h>
#include <iostream>
#include <fstream>
#include <vector>
#include <cstdlib>
#include <random>
#include "basics/tensorialTools.h"
#include <ctime>
#include <iomanip>

using namespace MPCP;

int main() {

  int dim = 3;
  tensorialTools TT = tensorialTools(dim);
  double lower_bound = -100.0;
  double upper_bound = 100.0;
  std::uniform_real_distribution<double> unif1(lower_bound,upper_bound);
  std::default_random_engine re;

  std::ofstream File;
  File.open("Results_Test_TTools_Inv.txt");

  std::cout << std::endl;
  File << std::endl;
  std::cout << "****************Inversion test************************" << std::endl;
  File << "****************Inversion test************************" << std::endl;

  int N = 500000;
  double *AG = new double[25];
  double *BG = new double[5];
  double *CG = new double[5];
  double *A = new double[36];
  double *C1 = new double[6];
  double *C2 = new double[6];
  double *B = new double[6];
  double *invA = new double[36];
  double *AA = new double[36];
  double *I = new double[36];
  clock_t tG, tI, t1, t2;

  MatrixXd buffer(6, 6);
  MatrixXd buffer2(6, 6);
  MatrixXd buffer3(6, 6);
  VectorXd vecBuffer(6);
  VectorXd vecBuffer2(6);

  for (int i=0; i < 6; i++){
    for (int j=0; j < 6; j++){
      if (i != j){
	A[(i*6)+j]=unif1(re);
      }
    }
    double a=0.0;
    for (int j=0; j < 6; j++){
      if (i != j){
	a +=abs(A[(i*6)+j]);
      }
    }
    A[(i*6)+i]=a*1.1;
    I[(i*6)+i]=1.0;
  }

  std::cout << "A" << std::endl;
  File << "A" << std::endl;
  for (int i=0; i < 6; i++){
    for (int j=0; j < 6; j++){
      std::cout << std::setw(12) << A[i*6+j] << " ";
      File << std::setw(12) << A[i*6+j] << " ";
    }
    std::cout << std::endl;
    File << std::endl;
  }

  t1=clock();
  for (int i=0; i < N; i++) GaussElim<6, 6>::solve(A, I, invA);
  t2=clock();
  tG=t2-t1;

  std::cout << "invA - Gauss Solver" << std::endl;
  File << "invA - Gauss Solver" << std::endl;
  for (int i=0; i < 6; i++){
    for (int j=0; j < 6; j++){
      std::cout << std::setw(12) << invA[i*6+j] << " ";
      File << std::setw(12) << invA[i*6+j] << " ";
    }
    std::cout << std::endl;
    File << std::endl;
  }

  for (int i = 0; i < 6; i++) {
    for (int j = 0; j < 6; j++)
      buffer(i,j) = A[i*6 + j];
  }

  t1=clock();
  for (int i=0; i < N; i++)
    TT.inverse(&buffer, 6, &buffer2);
  t2=clock();
  tI=t2-t1;
  for (int i = 0; i < 6; i++) {
    for (int j = 0; j < 6; j++)
      invA[i*6 + j] = buffer2(i,j);
  }

  std::cout << "invA - Partitioned Inverse" << std::endl;
  File << "invA - Partitioned Inverse" << std::endl;
  for (int i=0; i < 6; i++){
    for (int j=0; j < 6; j++){
      std::cout << std::setw(12) << invA[i*6+j] << " ";
      File << std::setw(12) << invA[i*6+j] << " ";
    }
    std::cout << std::endl;
    File << std::endl;
  }

  TT.squareMatrixDotProduct(&buffer, &buffer2, 6, &buffer3);
  for (int i = 0; i < 6; i++) {
    for (int j = 0; j < 6; j++)
      AA[i*6 + j] = buffer3(i,j);
  }

  std::cout << "Check - A*invA" << std::endl;
  File << "Check - A*invA" << std::endl;
  for (int i=0; i < 6; i++){
    for (int j=0; j < 6; j++){
      std::cout << std::setw(12) << AA[i*6+j] << " ";
      File << std::setw(12) << AA[i*6+j] << " ";
    }
    std::cout << std::endl;
    File << std::endl;
  }

  std::cout << std::endl;
  File << std::endl;
  std::cout << "Solution time for " << N << " cases" << std::endl;
  File << "Solution time for " << N << " cases" << std::endl;
  std::cout << "Gauss Solver " << ((float)tG)/CLOCKS_PER_SEC << " (s) " << "Partitioned Inverse "  << ((float)tI)/CLOCKS_PER_SEC << " (s)" << std::endl;
  File << "Gauss Solver " << ((float)tG)/CLOCKS_PER_SEC << " (s) " << "Partitioned Inverse "  << ((float)tI)/CLOCKS_PER_SEC << " (s)" << std::endl;

  //Solver
  std::cout << std::endl;
  File << std::endl;
  std::cout << "****************Solve test A(6*6)x(6)=b(6)************************" << std::endl;
  File << "****************Solve test A(6*6)x(6)=b(6)************************" << std::endl;

  for (int i=0; i < 6; i++){
    for (int j=0; j < 6; j++){
      if (i != j){
	A[(i*6)+j]=unif1(re);
      }
    }
    double a=0.0;
    for (int j=0; j < 6; j++){
      if (i != j){
	a +=abs(A[(i*6)+j]);
      }
    }
    A[(i*6)+i]=a*1.1;
  }

  for (int i=0; i < 6; i++){
    B[i]=unif1(re);
  }

  std::cout << "A - B" << std::endl;
  File << "A - B" << std::endl;
  for (int i=0; i < 6; i++){
    for (int j=0; j < 6; j++){
      std::cout << std::setw(12) << A[i*6+j] << " ";
      File << std::setw(12) << A[i*6+j] << " ";
    }
    std::cout << "  " << std::setw(12) << B[i];
    std::cout << std::endl;
    File << std::endl;
  }

  t1=clock();
  //for (int i=0; i < N; i++) GaussElim<6, 1>::solve(A, B, C1);
  t2=clock();
  tG=t2-t1;

  for (int i = 0; i < 6; i++) {
    for (int j = 0; j < 6; j++)
      buffer(i,j) = A[i*6 + j];
  }

  t1=clock();
  for (int i=0; i < N; i++){
    TT.inverse(&buffer, 6, &buffer2);
    TT.matrixVectorProduct(&buffer2, &vecBuffer, 6, &vecBuffer2);
  }
  t2=clock();
  tI=t2-t1;

  for (int i = 0; i < 6; i++) {
   C2[i] = vecBuffer2(i);
  }


  std::cout << "Result Gauss Solver - Result Partitioned Inverse" << std::endl;
  File << "Result Gauss Solver - Result Partitioned Inverse" << std::endl;
  for (int i=0; i < 6; i++){
    std::cout << std::setw(12) << C1[i] << " " << std::setw(12) << C2[i] << std::endl;
    File << std::setw(12) << C1[i] << " " << std::setw(12) << C2[i] << std::endl;
  }

  std::cout << std::endl;
  File << std::endl;
  std::cout << "Solution time for " << N << " cases" << std::endl;
  File << "Solution time for " << N << " cases" << std::endl;
  std::cout << "Gauss Solver " << ((float)tG)/CLOCKS_PER_SEC << " (s) " << "Partitioned Inverse "  << ((float)tI)/CLOCKS_PER_SEC << " (s)" << std::endl;
  File << "Gauss Solver " << ((float)tG)/CLOCKS_PER_SEC << " (s) " << "Partitioned Inverse "  << ((float)tI)/CLOCKS_PER_SEC << " (s)" << std::endl;


  //Solver 2
  std::cout << std::endl;
  File << std::endl;
  std::cout << "****************Solver test A(5*5)x(5)=b(5)************************" << std::endl;
  File << "****************Solver test A(5*5)x(5)=b(5)************************" << std::endl;
        
  for (int i=0; i < 6; i++){
    for (int j=0; j < 6; j++){
      A[i*6+j]=0.0;
    }
    B[i]=0.0;
  }

  for (int i=0; i < 5; i++){
    for (int j=0; j < 5; j++){
      if (i != j){
	AG[(i*5)+j]=unif1(re);
      }
    }
    double a=0.0;
    for (int j=0; j < 5; j++){
      if (i != j){
	a +=abs(AG[(i*5)+j]);
      }
    }
    AG[(i*5)+i]=a*1.1;
    BG[i]=unif1(re);
  }

  std::cout << "A - B" << std::endl;
  File << "A - B" << std::endl;
  for (int i=0; i < 5; i++){
    for (int j=0; j < 5; j++){
      std::cout << std::setw(12) << AG[i*5+j] << " ";
      File << std::setw(12) << AG[i*5+j] << " ";
    }
    std::cout << "  " << std::setw(12) << BG[i];
    std::cout << std::endl;
    File << std::endl;
  }

  t1=clock();
  for (int i=0; i < N; i++) GaussElim<5, 1>::solve(AG, BG, CG);
  t2=clock();
  tG=t2-t1;


  t1=clock();
  for (int i=0; i < 5; i++){
    for (int j=0; j < 5; j++){
      A[i*6+j]=AG[(i*5)+j];
    }
    B[i]=BG[i];
  }
  A[35]=1.0;
  B[5]=1.0;

  for (int i = 0; i < 6; i++) {
    for (int j = 0; j < 6; j++)
      buffer(i,j) = A[i*6 + j];
    vecBuffer(i) = B[i];
  }

  
  for (int i=0; i < N; i++){
    TT.inverse(&buffer, 6, &buffer2);
    TT.matrixVectorProduct(&buffer2, &vecBuffer, 6, &vecBuffer2);
  }
  for (int i = 0; i < 6; i++) {
    C2[i] = vecBuffer2(i);
  }
  t2=clock();
  tI=t2-t1;
    
  std::cout << "Result Gauss Solver - Result Partitioned Inverse" << std::endl;
  File << "Result Gauss Solver - Result Partitioned Inverse" << std::endl;
  for (int i=0; i < 5; i++){
    std::cout << std::setw(12) << CG[i] << " " << std::setw(12) << C2[i] << std::endl;
    File << std::setw(12) << CG[i] << " " << std::setw(12) << C2[i] << std::endl;
  }

  std::cout << std::endl;
  File << std::endl;
  std::cout << "Solution time for " << N << " cases" << std::endl;
  File << "Solution time for " << N << " cases" << std::endl;
  std::cout << "Gauss Solver " << ((float)tG)/CLOCKS_PER_SEC << " (s) " << "Partitioned Inverse "  << ((float)tI)/CLOCKS_PER_SEC << " (s)" << std::endl;
  File << "Gauss Solver " << ((float)tG)/CLOCKS_PER_SEC << " (s) " << "Partitioned Inverse "  << ((float)tI)/CLOCKS_PER_SEC << " (s)" << std::endl;

  delete[] A;
  delete[] AG;
  delete[] invA;
  delete[] AA;
  delete[] C1;
  delete[] C2;
  delete[] CG;
  delete[] B;
  delete[] BG;

  std::cout << std::endl;
  File << std::endl;

  File.close();
  return 0;
}
