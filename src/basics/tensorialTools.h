// copyright [2015] Daniel Pino Muñoz

// tensorialTools.h

/* This class provides mayny tensorial useful functions
   Daniel Pino Muñoz
   daniel.pino_munoz@mines-paristech.fr
   13 August 2015
 */

#ifndef SRC_BASICS_TENSORIALTOOLS_H_
#define SRC_BASICS_TENSORIALTOOLS_H_

#include <string>
#include <vector>
#include <cstdio>
#include <math.h> // sqrt

#include <Eigen/Dense>


#include "basicsExport.h"

namespace MPCP {

using namespace Eigen;

// Class for (small) linear system solving (A*x = b) by Gauss elimination with
// partial pivoting. Works with template recursion (solve reduced sub-system
// after line pivoting)
// Template arguments: M = dimension of square matrix A, N = number of columns
// of b and x.
template<int M, int N> class GaussElim {
 public:
  // Solve linear system A*x = b (with A square matrix)
  static inline void solve(const double *A, const double *b, double *x) {
    // Find pivoting line (max. absolute value of leading coefficient)
    double maxAbsCoeff = fabs(A[iA(0, 0)]);
    int iPiv = 0;
    for (int i = 1; i < M; i++) {
      const double absCoeff = fabs(A[iA(i, 0)]);
      if (absCoeff > maxAbsCoeff) {
        maxAbsCoeff = absCoeff;
        iPiv = i;
      }
    }

    // Build reduced sub-system
    std::vector<double> AA((M-1)*(M-1), 0);
    std::vector<double> bb((M-1)*N, 0);
    const double invMax = 1./A[iA(iPiv, 0)];
    for (int i = 0; i < iPiv; i++) {  // Before iPiv
      const double alpha = A[iA(i, 0)]*invMax;
      for (int j = 1; j < M; j++) {
        AA[iAA(i, j-1)] = A[iA(i, j)] - A[iA(iPiv, j)]*alpha;
      }
      for (int j = 0; j < N; j++) {
        bb[ibb(i, j)] = b[ib(i, j)] - b[ib(iPiv, j)]*alpha;
      }
    }
    for (int i = iPiv+1; i < M; i++) {  // After iPiv
      const double alpha = A[iA(i, 0)]*invMax;
      for (int j = 1; j < M; j++) {
        AA[iAA(i-1, j-1)] = A[iA(i, j)] - A[iA(iPiv, j)]*alpha;
      }
      for (int j = 0; j < N; j++) {
        bb[ibb(i-1, j)] = b[ib(i, j)] - b[ib(iPiv, j)]*alpha;
      }
    }

    // Solve reduced sub-system
    double xx[(M-1)*N];
    GaussElim<M-1, N>::solve(AA.data(), bb.data(), xx);

    // Compute solution for pivoting line, store in first line
    for (int j = 0; j < N; j++) {
      x[ix(0, j)] = b[ib(iPiv, j)];
      for (int k = 1; k < M; k++) {
        x[ix(0, j)] -= A[iA(iPiv, k)]*xx[ixx(k-1, j)];
      }
      x[ix(0, j)] *= invMax;
    }

    // Copy solution of reduced sub-system in following lines
    for (int i = 1; i < M; i++) {
      for (int j = 0; j < N; j++) {
        x[ix(i, j)] = xx[ixx(i-1, j)];
      }
    }
  }

 private:

  static inline int iA(int i, int j) { return i*M+j; }
  static inline int ib(int i, int j) { return i*N+j; }
  static inline int ix(int i, int j) { return i*N+j; }
  static inline int iAA(int i, int j) { return i*(M-1)+j; }
  static inline int ibb(int i, int j) { return i*N+j; }
  static inline int ixx(int i, int j) { return i*N+j; }
};


// Class for (small) linear system solving (A*x = b) by Gauss elimination
// with partial pivoting.
// Specialization for 1x1 matrix A (termination of template recursion):
// trivial solution
// Template argument: N = number of columns of b and x.
template<int N> class GaussElim<1, N> {
 public:
    static inline void solve(const double *A, const double *b, double *x) {
    for (int j = 0; j < N; j++) {
      x[j] = b[j] / (*A);
    }
  }
};

class BASICS_EXPORT tensorialTools {
  //  Methods definition
 public:
  explicit tensorialTools(int dim = 3);  // constructor
  ~tensorialTools();  // destructor
  double dev(const MatrixXd * tensor, MatrixXd * dev,
	     const double tensor_33 = 0);
  void sym(const MatrixXd * tensor, MatrixXd * sym);
  void skew(const MatrixXd * tensor, MatrixXd * skew);
  double trace(const MatrixXd * tensor, const double tensor_33 = 0);
  void transpose(const MatrixXd * tensor, MatrixXd * transposed);
  void SOTensor2Voigt(const MatrixXd* tensor, VectorXd * voigt,
                      bool strain = false);
  void SOVoigt2Tensor(const VectorXd* voigt, MatrixXd* tensor,
                      bool strain = false);
  void FOVoigt2Tensor(const MatrixXd* FOVoigt,
		      double* tensor);
  void FOTensor2Voigt(const double* tensor,
		      MatrixXd* FOVoigt);
  void tensorDotProduct(const MatrixXd* tensorA,
			const MatrixXd* tensorB, MatrixXd * product);
  void squareMatrixDotProduct(const MatrixXd* tensorA,
                              const MatrixXd* tensorB,
                              const int size,
                              MatrixXd * product);
  double tensorDoubleDotProduct(const MatrixXd* tensorA,
				const MatrixXd* tensorB);
  void exponentialMap(const MatrixXd* tensorA, MatrixXd * exp);
  void FOVoigtFromProduct(const MatrixXd* tensorA,
			  const MatrixXd* tensorB,
			  MatrixXd * FOVoigt);
  void FOVoigtDotSOVoigt(const MatrixXd* FOVoigt,
			 const VectorXd* SOVoigt,
                         VectorXd * Voigt);
  void inverse(const MatrixXd *A, const int order, MatrixXd *Y);
  void getMinor(const MatrixXd *src, MatrixXd *dest,
		int row, int col, const int order);
  double det(const MatrixXd *mat, const int order);
  void eulerToR(const VectorXd * euler, MatrixXd * R);
  void RtoEuler(const MatrixXd * R, VectorXd * euler);
  void print(const MatrixXd *mat, const int order);
  void clear(VectorXd* mat, const int dim);
  // rotation of a SOT Bij = Rik Akl Rjl = R * A * R^T
  void RARt(const MatrixXd * R, const MatrixXd * A, MatrixXd * B);
  void setIdentity(MatrixXd *A);
  void setIdentityVoigt(MatrixXd* A);

  void setDevProjection(MatrixXd *A);
  void setSphProjection(MatrixXd *A);
  inline int getVoigtSize() {return this->voigtSize; }
  void schmidtTensor(const VectorXd * n, const VectorXd *s,
		     MatrixXd * schmidt);
  double SOTNorm(const MatrixXd * A);
  
// Y.Zhang
  void scalarMatrixProduct(const double* scalar,
			   const MatrixXd* Matrix,
			   const int size, MatrixXd* product);

  void matrixVectorProduct(const MatrixXd* MatriceA,
                           const VectorXd* VectorB,
                           const int size, VectorXd* product);
											
  double vectorDotVectorProduct(const VectorXd* VectorA,
				const VectorXd* VectorB,
				const int size);
  //Cubic base transformations
  void DevSOTensor2Cubic(const MatrixXd* tensor, VectorXd * cubic);
  void DevSOCubic2Tensor(const VectorXd* cubic, MatrixXd * tensor);
  void DevF0CubicFromProduct(const MatrixXd* tensorA,
			     const MatrixXd* tensorB,
			     MatrixXd * cubic);
  void DevFOCubic2Tensor(const MatrixXd* cubic,
			 double * tensor);
  void FOCubic2Tensor(const MatrixXd* cubic,
		      double * tensor);
  void FOTensor2Cubic(const double * tensor,
		      MatrixXd* FOCubic);
  inline std::vector<MatrixXd *> ** getKelvinBase()
  { return &cubicBase; };
  inline void setKelvinBaseInitialized(const std::string & writer) {
    whoWroteTheBase = writer;
  };
  inline bool sameWriter(const std::string & writer) {
    return (whoWroteTheBase.compare(writer) == 0);
  }
 
  inline bool getKelvinBaseInitialized()
  { return kelvinBaseInitialized; };

 protected:

 private:
  //  Members definition
 public:

  std::vector< std::vector<int> > myVoigtToClassic;
  int **myClassicToVoigt;
  MatrixXd * devProjection;  // Voigt FO tensor

 protected:
  // Some members common to all tensorialTools
  int dim;  // spatial dimension
  
  MatrixXd * SOTAuxiliar0;  //  Auxiliar tensor required for some operations
  MatrixXd * SOTAuxiliar1;  //  Auxiliar tensor required for some operations
  int voigtSize;
  
  // Cubic base Eigen matrix
  std::vector<MatrixXd *> *cubicBase;
  bool kelvinBaseInitialized;
  std::string whoWroteTheBase;

 private:
};
}  // namespace MPCP
#endif  // SRC_BASICS_TENSORIALTOOLS_H_
