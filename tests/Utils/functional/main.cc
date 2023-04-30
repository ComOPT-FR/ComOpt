#include <math.h>
#include <string.h>
#include <iostream>
#include <fstream>
#include <vector>
#include <cstdlib>
#include "Utils/tensorialTools.h"

#include <iomanip>

using namespace MPCP;

int main() {

  int dim = 3;
  int voigtSize = 6;
  
  tensorialTools TT = tensorialTools(dim);

  std::ofstream File;
  File.open("Results_Test_TTools.txt");

  std::cout << "Tensorial Tools test" << std::endl;
  File << "Tensorial Tools test" << std::endl;

  std::cout << "***************Cubic 4 order**********************************" << std::endl;
  File << "***************Cubic 4 order**********************************" << std::endl;
  std::cout << "FOTensor2Cubic - FOCubic2Tensor" << std::endl;
  File << "FOTensor2Cubic - FOCubic2Tensor" << std::endl;
    
  double *Cd0 = new double[81];
  double *I = new double[9];
  MatrixXd *CdCubic = new MatrixXd(6, 6);
  MatrixXd buffer(3, 3);
  MatrixXd buffer2(3, 3);
  MatrixXd buffer3(3, 3);
  VectorXd vecBuffer(3);
  VectorXd vecBuffer2(3);
  VectorXd voigtBuffer(voigtSize);
  VectorXd cubicBuffer(voigtSize-1);
  MatrixXd cubicFOBuffer(5, 5);

  for (int i=0; i < 81; i++){
    Cd0[i]=0.0;
  }

  for (int i=0; i < 9; i++){
    I[i]=0.0;
  }
  I[0]=1.0;
  I[4]=1.0;
  I[8]=1.0;
  double EE=63861.47;
  double vv=0.3616;
  double u=EE/(2*(1+vv));
  double y=(EE*vv)/((1-vv)*(1-2*vv));
  for (int i=0; i <3 ; i++){
    for (int j=0; j <3 ; j++){
      for (int k=0; k <3 ; k++){
	for (int l=0; l <3 ; l++){
	  Cd0[ (i*27) + (j*9) +(k*3) + l]=(u*( (I[i*3+k] * I[j*3+l]) + (I[i*3+l] * I[j*3+k]) )) + (y * ( I[i*3+j] * I[k*3+l] ));
	}
      }
    }
  }

  std::cout << "Cd" << std::endl;
  File << "Cd" << std::endl;
  for (int i=0; i < 9; i++){
    for (int j=0; j < 9; j++){
      std::cout << std::setw(10) << Cd0[i*9+j] << " ";
      File << std::setw(10) << Cd0[i*9+j] << " ";
    }
    std::cout << std::endl;
    File << std::endl;
  }

  std::cout << "CdtoCubic" << std::endl;
  File << "CdtoCubic" << std::endl;
   
  TT.FOTensor2Cubic(Cd0,CdCubic);
  for (int i=0; i < 6; i++){
    for (int j=0; j < 6; j++){
      std::cout << std::setw(10) << (*CdCubic)(i, j) << " ";
      File << std::setw(10) << (*CdCubic)(i, j) << " ";
    }
    std::cout << std::endl;
    File << std::endl;
  }

  TT.FOCubic2Tensor(CdCubic,Cd0);
    
  std::cout << "CubictoCd" << std::endl;
  File << "CubictoCd" << std::endl;

  for (int i=0; i < 9; i++){
    for (int j=0; j < 9; j++){
      std::cout << std::setw(10) << Cd0[i*9+j] << " ";
      File << std::setw(10) << Cd0[i*9+j] << " ";
    }
    std::cout << std::endl;
    File << std::endl;
  }

  delete CdCubic;
  delete[] Cd0;
  delete[] I;

  std::cout << "***************Euler - Re***********************************" << std::endl;
  File << "***************Euler - Re***********************************" << std::endl;
  std::cout << "eulertoR - RtoEulrt" << std::endl;
  File << "eulertoR - RtoEulrt" << std::endl;

  double *euler = new double[3];
  double *euler2 = new double[3];
  double *Re = new double[9];
  double *ReT = new double[9];
  double *ReReT = new double[9];

  double pi = acos(-1.0);

  euler[0]=30*(pi/180);
  euler[1]=15*(pi/180);
  euler[2]=90*(pi/180);

  for (int i = 0; i < dim; i++) {
    vecBuffer(i) = euler[i];
  }

  TT.eulerToR(&vecBuffer, &buffer);
  for (int i = 0; i < dim; i++) {
    for (int j = 0; j < dim; j++)
      Re[i*dim + j] = buffer(i,j);
  }
  
  TT.RtoEuler(&buffer, &vecBuffer);
  for (int i = 0; i < dim; i++) {
    euler2[i] = vecBuffer(i);
  }
  
  TT.transpose(&buffer, &buffer2);
  for (int i = 0; i < dim; i++) {
    for (int j = 0; j < dim; j++)
      ReT[i*dim + j] = buffer2(i,j);
  }
  
  TT.squareMatrixDotProduct(&buffer, &buffer2, dim, &buffer3);
  for (int i = 0; i < dim; i++) {
    for (int j = 0; j < dim; j++)
      ReReT[i*dim + j] = buffer3(i,j);
  }

  std::cout << "phi1 " << euler[0] << "  " << euler2[0] << std::endl;
  std::cout << "psi  " << euler[1] << "  " << euler2[1] << std::endl;
  std::cout << "phi2 " << euler[2] << "  " << euler2[2] << std::endl;
  std::cout << "Re" << std::endl;
  std::cout << std::setw(12) <<  Re[0] << "  " << std::setw(12) << Re[1] << "  " << std::setw(12) << Re[2] << std::endl;
  std::cout << std::setw(12) <<  Re[3] << "  " << std::setw(12) << Re[4] << "  " << std::setw(12) << Re[5] << std::endl;
  std::cout << std::setw(12) <<  Re[6] << "  " << std::setw(12) << Re[7] << "  " << std::setw(12) << Re[8] << std::endl;
  std::cout << "ReT" << std::endl;
  std::cout << std::setw(12) <<  ReT[0] << "  " << std::setw(12) << ReT[1] << "  " << ReT[2] << std::endl;
  std::cout << std::setw(12) <<  ReT[3] << "  " << std::setw(12) << ReT[4] << "  " << ReT[5] << std::endl;
  std::cout << std::setw(12) <<  ReT[6] << "  " << std::setw(12) << ReT[7] << "  " << ReT[8] << std::endl;
  std::cout << "ReReT" << std::endl;
  std::cout << std::setw(12) <<  ReReT[0] << "  " << std::setw(12) << ReReT[1] << "  " << std::setw(12) << ReReT[2] << std::endl;
  std::cout << std::setw(12) <<  ReReT[3] << "  " << std::setw(12) << ReReT[4] << "  " << std::setw(12) << ReReT[5] << std::endl;
  std::cout << std::setw(12) <<  ReReT[6] << "  " << std::setw(12) << ReReT[7] << "  " << std::setw(12) << ReReT[8] << std::endl;

  File << "phi1 " << euler[0] << "  " << euler2[0] << std::endl;
  File << "psi  " << euler[1] << "  " << euler2[1] << std::endl;
  File << "phi2 " << euler[2] << "  " << euler2[2] << std::endl;
  File << "Re" << std::endl;
  File << std::setw(12) <<  Re[0] << "  " << std::setw(12) << Re[1] << "  " << std::setw(12) << Re[2] << std::endl;
  File << std::setw(12) <<  Re[3] << "  " << std::setw(12) << Re[4] << "  " << std::setw(12) << Re[5] << std::endl;
  File << std::setw(12) <<  Re[6] << "  " << std::setw(12) << Re[7] << "  " << std::setw(12) << Re[8] << std::endl;
  File << "ReT" << std::endl;
  File << std::setw(12) <<  ReT[0] << "  " << std::setw(12) << ReT[1] << "  " << std::setw(12) << ReT[2] << std::endl;
  File << std::setw(12) <<  ReT[3] << "  " << std::setw(12) << ReT[4] << "  " << std::setw(12) << ReT[5] << std::endl;
  File << std::setw(12) <<  ReT[6] << "  " << std::setw(12) << ReT[7] << "  " << std::setw(12) << ReT[8] << std::endl;
  File << "ReReT" << std::endl;
  File << std::setw(12) <<  ReReT[0] << "  " << std::setw(12) << ReReT[1] << "  " << std::setw(12) << ReReT[2] << std::endl;
  File << std::setw(12) <<  ReReT[3] << "  " << std::setw(12) << ReReT[4] << "  " << std::setw(12) << ReReT[5] << std::endl;
  File << std::setw(12) <<  ReReT[6] << "  " << std::setw(12) << ReReT[7] << "  " << std::setw(12) << ReReT[8] << std::endl;

  delete[] euler;
  delete[] euler2;
  delete[] Re;
  delete[] ReT;
  delete[] ReReT;

  std::cout << "***************Operations***********************************" << std::endl;
  File << "***************Operations***********************************" << std::endl;
  std::cout << "Deviatoric" << std::endl;
  File << "Deviatoric" << std::endl;
    
  double *A = new double[9];
  A[0]=50; A[1]=30; A[2]=20;
  A[3]=30; A[4]=-20; A[5]=-10;
  A[6]=20; A[7]=-10; A[8]=10;
  double *B = new double[9];
  double T=0.0;

  for (int i = 0; i < dim; i++) {
    for (int j = 0; j < dim; j++)
      buffer(i,j) = A[i*dim+j];
  }
  
  T = TT.trace(&buffer);
  
  TT.dev(&buffer, &buffer2);
  
  for (int i = 0; i < dim; i++) {
    for (int j = 0; j < dim; j++)
      B[i*dim + j] = buffer2(i,j);
  }

  std::cout << "Tensor" << std::endl;
  File << "Tensor" << std::endl;
  for (int i=0; i < 3; i++){
    for (int j=0; j < 3; j++){
      std::cout << std::setw(10) << A[i*3+j] << " ";
      File << std::setw(10) << A[i*3+j] << " ";
    }
    std::cout << std::endl;
    File << std::endl;
  }

  std::cout << "Trace" << T << std::endl;
  File << "Trace" << T << std::endl;  

  std::cout << "Dev" << std::endl;
  File << "Dev" << std::endl;
  for (int i=0; i < 3; i++){
    for (int j=0; j < 3; j++){
      std::cout << std::setw(10) << B[i*3+j] << " ";
      File << std::setw(10) << B[i*3+j] << " ";
    }
    std::cout << std::endl;
    File << std::endl;
  }  

  //**************************************
  std::cout << "Sym-Skew-Trans" << std::endl;
  File << "Sym-Skew-Trans" << std::endl;

  A[0]=0.5; A[1]=0.4; A[2]=0.3;
  A[3]=0.2; A[4]=-0.1; A[5]=-0.2;
  A[6]=-0.3; A[7]=-0.4; A[8]=-0.1;


  std::cout << "Tensor" << std::endl;
  File << "Tensor" << std::endl;
  for (int i=0; i < 3; i++){
    for (int j=0; j < 3; j++){
      std::cout << std::setw(10) << A[i*3+j] << " ";
      File << std::setw(10) << A[i*3+j] << " ";
    }
    std::cout << std::endl;
    File << std::endl;
  }

  for (int i = 0; i < dim; i++) {
    for (int j = 0; j < dim; j++)
      buffer(i,j) = A[i*dim+j];
  }
  TT.sym(&buffer, &buffer2);

  for (int i = 0; i < dim; i++) {
    for (int j = 0; j < dim; j++)
      B[i*dim + j] = buffer2(i,j);
  }
  
  std::cout << "Sym" << std::endl;
  File << "Sym" << std::endl;
  for (int i=0; i < 3; i++){
    for (int j=0; j < 3; j++){
      std::cout << std::setw(10) << B[i*3+j] << " ";
      File << std::setw(10) << B[i*3+j] << " ";
    }
    std::cout << std::endl;
    File << std::endl;
  }

  TT.skew(&buffer, &buffer2);

  for (int i = 0; i < dim; i++) {
    for (int j = 0; j < dim; j++)
      B[i*dim + j] = buffer2(i,j);
  }
  
  std::cout << "Skew" << std::endl;
  File << "Skew" << std::endl;
  for (int i=0; i < 3; i++){
    for (int j=0; j < 3; j++){
      std::cout << std::setw(10) << B[i*3+j] << " ";
      File << std::setw(10) << B[i*3+j] << " ";
    }
    std::cout << std::endl;
    File << std::endl;
  }
  
  TT.transpose(&buffer, &buffer2);

  for (int i = 0; i < dim; i++) {
    for (int j = 0; j < dim; j++)
      B[i*dim + j] = buffer2(i,j);
  }
  
  std::cout << "Transpose" << std::endl;
  File << "Transpose" << std::endl;
  for (int i=0; i < 3; i++){
    for (int j=0; j < 3; j++){
      std::cout << std::setw(10) << B[i*3+j] << " ";
      File << std::setw(10) << B[i*3+j] << " ";
    }
    std::cout << std::endl;
    File << std::endl;
  } 

  std::cout << "***************Transformations***********************************" << std::endl;
  File << "***************Transformations***********************************" << std::endl;
  A[0]=37; A[1]=30; A[2]=20;
  A[3]=30; A[4]=-33; A[5]=-10;
  A[6]=20; A[7]=-10; A[8]=-4;
  double *C = new double[6];
  double *D = new double[5];

  std::cout << "Tensor" << std::endl;
  File << "Tensor" << std::endl;
  for (int i=0; i < 3; i++){
    for (int j=0; j < 3; j++){
      std::cout << std::setw(10) << A[i*3+j] << " ";
      File << std::setw(10) << A[i*3+j] << " ";
    }
    std::cout << std::endl;
    File << std::endl;
  }

  for (int i = 0; i < dim; i++) {
    for (int j = 0; j < dim; j++)
      buffer(i, j) = A[i*dim + j];
  }

  TT.SOTensor2Voigt(&buffer, &voigtBuffer, false);
  for (int i = 0; i < voigtSize; i++)
    C[i] = voigtBuffer(i);

  std::cout << "Voigt" << std::endl;
  File << "Voigt" << std::endl;
  for (int i=0; i < 6; i++){
    std::cout << std::setw(10) << C[i] << " " ;
    File << std::setw(10) << C[i] << " ";
  }
  std::cout << std::endl;
  File << std::endl;
    

  TT.SOVoigt2Tensor(&voigtBuffer, &buffer, false);

  for (int i = 0; i < dim; i++) {
    for (int j = 0; j < dim; j++)
      A[i*dim + j] = buffer(i,j);
  }
  
  std::cout << "VtoTensor" << std::endl;
  File << "VtoTensor" << std::endl;
  for (int i=0; i < 3; i++){
    for (int j=0; j < 3; j++){
      std::cout << std::setw(10) << A[i*3+j] << " ";
      File << std::setw(10) << A[i*3+j] << " ";
    }
    std::cout << std::endl;
    File << std::endl;
  }

  TT.DevSOTensor2Cubic(&buffer, &cubicBuffer);
  for (int i = 0; i < voigtSize-1; i++)
    D[i] = cubicBuffer(i);
  
  std::cout << "Cubic" << std::endl;
  File << "Cubic" << std::endl;
  for (int i=0; i < 5; i++){
    std::cout << std::setw(10) << D[i] << " " ;
    File << std::setw(10) << D[i] << " ";
  }
  std::cout << std::endl;
  File << std::endl;
    

  TT.DevSOCubic2Tensor(&cubicBuffer, &buffer);

  for (int i = 0; i < dim; i++) {
    for (int j = 0; j < dim; j++)
      A[i*dim + j] = buffer(i,j);
  }
  
  std::cout << "CtoTensor" << std::endl;
  File << "CtoTensor" << std::endl;
  for (int i=0; i < 3; i++){
    for (int j=0; j < 3; j++){
      std::cout << std::setw(10) << A[i*3+j] << " ";
      File << std::setw(10) << A[i*3+j] << " ";
    }
    std::cout << std::endl;
    File << std::endl;
  }

  delete[] A;
  delete[] B;
  delete[] C;
  delete[] D;

  std::cout << "***************Schmidt***********************************" << std::endl;
  File << "***************Schmidt***********************************" << std::endl;

  double *n = new double[3];
  n[0]=-sqrt(3); n[1]=sqrt(3); n[2]=sqrt(3);
  double *m = new double[3];
  m[0]=0; m[1]=-sqrt(2); m[2]=sqrt(2);
  double *S = new double[9];
  double *ST = new double[9];
  double *SS = new double[25];

  for (int i = 0; i < dim; i++) {
    vecBuffer(i) = n[i];
    vecBuffer2(i) = m[i];
  }
  
  TT.schmidtTensor(&vecBuffer, &vecBuffer2, &buffer);
  for (int i = 0; i < dim; i++) {
    for (int j = 0; j < dim; j++)
      S[i*dim + j] = buffer(i,j);
  }
    
  std::cout << "n" << std::endl;
  File << "n" << std::endl;
  for (int i=0; i < 3; i++){
    std::cout << std::setw(10) << n[i] << " " ;
    File << std::setw(10) << n[i] << " ";
  }
  std::cout << std::endl;
  File << std::endl;

  std::cout << "m" << std::endl;
  File << "m" << std::endl;
  for (int i=0; i < 3; i++){
    std::cout << std::setw(10) << m[i] << " " ;
    File << std::setw(10) << m[i] << " ";
  }
  std::cout << std::endl;
  File << std::endl;

  std::cout << "Schmidt" << std::endl;
  File << "Schmidt" << std::endl;
  for (int i=0; i < 3; i++){
    for (int j=0; j < 3; j++){
      std::cout << std::setw(10) << S[i*3+j] << " ";
      File << std::setw(10) << S[i*3+j] << " ";
    }
    std::cout << std::endl;
    File << std::endl;
  }

  TT.sym(&buffer, &buffer2);
  for (int i = 0; i < dim; i++) {
    for (int j = 0; j < dim; j++)
      ST[i*dim + j] = buffer2(i,j);
  }

  std::cout << "Schmidt Sym" << std::endl;
  File << "Schmidt Sym" << std::endl;
  for (int i=0; i < 3; i++){
    for (int j=0; j < 3; j++){
      std::cout << std::setw(10) << ST[i*3+j] << " ";
      File << std::setw(10) << ST[i*3+j] << " ";
    }
    std::cout << std::endl;
    File << std::endl;
  }

  TT.DevF0CubicFromProduct(&buffer2, &buffer2, &cubicFOBuffer);
  for (int i = 0; i < voigtSize-1; i++) {
    for (int j = 0; j < voigtSize-1; j++)
      SS[i*(voigtSize-1) + j] = cubicFOBuffer(i,j);
  }
  std::cout << "Sijkl" << std::endl;
  File << "Sijkl" << std::endl;
  for (int i=0; i < 5; i++){
    for (int j=0; j < 5; j++){
      std::cout << std::setw(12) << SS[i*5+j] << " ";
      File << std::setw(12) << SS[i*5+j] << " ";
    }
    std::cout << std::endl;
    File << std::endl;
  }

  delete[] m;
  delete[] n;
  delete[] S;
  delete[] ST;
  delete[] SS;

  std::cout << std::endl;
  File << std::endl;

  File.close();

  return 0;
}
