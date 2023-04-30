#include "tensorialTools.h"
#include <vector>
#include <math.h>
#include <iostream>
#include <cstdlib>

using namespace Eigen;

namespace MPCP {

// constructor
tensorialTools::tensorialTools(int d) {
    dim = d;
    // voigt size only works in 1D, 2D and 3D
    voigtSize = (dim == 3) ? 6 : (dim == 2 ? 3 : 1);

    // Voigt convention:
    // TODO(Gerry) To put in a separate method
    //  myVoigtToClassic = create_voigt_to_classic();
    std::vector<int> indexes(2);
    indexes[0] = 0;
    indexes[1] = 0;
    myVoigtToClassic.push_back(indexes);

    indexes[0] = 1;
    indexes[1] = 1;
    myVoigtToClassic.push_back(indexes);

    if (dim == 3) {
        indexes[0] = 2;
        indexes[1] = 2;
        myVoigtToClassic.push_back(indexes);
    }

    indexes[0] = 0;
    indexes[1] = 1;
    myVoigtToClassic.push_back(indexes);

    if (dim == 3) {
        indexes[0] = 0;
        indexes[1] = 2;
        myVoigtToClassic.push_back(indexes);

        indexes[0] = 1;
        indexes[1] = 2;
        myVoigtToClassic.push_back(indexes);
    }

    // TODO(Gerry) To put in a separate method
    myClassicToVoigt = new int*[dim];
    for (int i = 0; i < dim; ++i) myClassicToVoigt[i] = new int[dim];

    if (dim == 3) {
        myClassicToVoigt[0][0] = 0;
        myClassicToVoigt[1][1] = 1;
        myClassicToVoigt[2][2] = 2;
        myClassicToVoigt[0][1] = 3;
        myClassicToVoigt[1][0] = 3;
        myClassicToVoigt[0][2] = 4;
        myClassicToVoigt[2][0] = 4;
        myClassicToVoigt[1][2] = 5;
        myClassicToVoigt[2][1] = 5;
    } else if (dim == 2) {
        myClassicToVoigt[0][0] = 0;
        myClassicToVoigt[1][1] = 1;
        myClassicToVoigt[0][1] = 2;
        myClassicToVoigt[1][0] = 2;

    } else {
        std::cerr << "MPCP::tensorialTools : not implemented for dim =" << dim
                  << "\n";
        std::exit(1);
    }

    SOTAuxiliar0  = new MatrixXd(dim, dim);
    SOTAuxiliar1  = new MatrixXd(dim, dim);
    devProjection = new MatrixXd(voigtSize, voigtSize);


    // Initialize deviatoric projection
    for (int i = 0; i < voigtSize; ++i) {
      int row_i = myVoigtToClassic[i][0];
      int col_i = myVoigtToClassic[i][1];
      for (int j = 0; j < voigtSize; ++j) {
	int row_j = myVoigtToClassic[j][0];
	int col_j = myVoigtToClassic[j][1];
	(*devProjection)(i, j) =
	  ((row_i == col_i) ? 0.5 : 1.) *
	  (((row_i == row_j && col_i == col_j) ? 1. : 0.) +
	   ((row_i == col_j && col_i == row_j) ? 1. : 0.)) -
	  ((row_i == col_i && row_j == col_j) ? 1. / 3. : 0.);
      }
    }

    if (dim < 3) {
	kelvinBaseInitialized = false;
    } else {
	kelvinBaseInitialized = true;
	cubicBase = new std::vector<MatrixXd *>();;
	for (int i = 0; i < voigtSize; ++i)
	  cubicBase->push_back(new MatrixXd(dim, dim));
    }

}

// retrieve Identity Tensor
void tensorialTools::setIdentityVoigt(MatrixXd *A) {
  for (int i = 0; i < voigtSize; ++i) {
    int row_i = myVoigtToClassic[i][0];
    int col_i = myVoigtToClassic[i][1];
    for (int j = 0; j < voigtSize; ++j) {
      int row_j = myVoigtToClassic[j][0];
      int col_j = myVoigtToClassic[j][1];
      (*A)(i, j) = ((row_i == row_j && col_i == col_j) ? 1. : 0.);
    }
  }
}

// retrieve deviatoric projection 
void tensorialTools::setDevProjection(MatrixXd *A) {
  for (int i = 0; i < voigtSize; ++i) {
    int row_i = myVoigtToClassic[i][0];
    int col_i = myVoigtToClassic[i][1];
    for (int j = 0; j < voigtSize; ++j) {
      int row_j = myVoigtToClassic[j][0];
      int col_j = myVoigtToClassic[j][1];
      (*A)(i, j) = ((row_i == col_i) ? 0.5 : 1.) *
	((((row_i == row_j && col_i == col_j) ? 1. : 0.) +
	  ((row_i == col_j && col_i == row_j) ? 1.
	   : 0.)) - (2. / 3.) *  ((row_i == col_i && row_j == col_j) ? 1. : 0.));
    }
  }
}

// retrieve spherical projection 
void tensorialTools::setSphProjection(MatrixXd *A) {
  for (int i = 0; i < voigtSize; ++i) {
    int row_i = myVoigtToClassic[i][0];
    int col_i = myVoigtToClassic[i][1];
    for (int j = 0; j < voigtSize; ++j) {
      int row_j = myVoigtToClassic[j][0];
      int col_j = myVoigtToClassic[j][1];
      (*A)(i, j) =
	(1. / 3.) * ((row_i == col_i && row_j == col_j) ? 1. : 0.);
    }
  }
}
// compute the deviatoric part of a tensor
double tensorialTools::dev(const MatrixXd* tensor, MatrixXd* dev,
                           const double tensor_33) {
  double trace = tensor->trace();
  *dev = *tensor;
  for (int i = 0; i < dim; ++i)
    (*dev)(i, i) -= trace / 3.;
  return (dim == 3 ? 0 : -trace / 3.);
}

// compute the symmetric part of a tensor
void tensorialTools::sym(const MatrixXd* tensor, MatrixXd* sym) {
  (*sym) = 0.5*((*tensor) + tensor->transpose());
}

// compute the skew part of a tensor
void tensorialTools::skew(const MatrixXd* tensor, MatrixXd* skew) {
  (*skew) = 0.5*(*tensor - (*tensor).transpose());
}

// Create Zeros 3rd order tensor with variable dimensions
void tensorialTools::clear(VectorXd* mat,const int dim) {
  mat->setZero();
}

// Compute the trace of a tensor
double tensorialTools::trace(const MatrixXd* tensor, const double tensor_33) {
  return tensor->trace() + tensor_33;
}

// return the transpose of a tensor
void tensorialTools::transpose(const MatrixXd* tensor, MatrixXd* transposed) {
  *transposed = (*tensor).transpose();
}

// Transform a second order tensor into voigt notation
void tensorialTools::SOTensor2Voigt(const MatrixXd* tensor, VectorXd* voigt,
                                    bool strain) {
  for (int i = 0; i < voigtSize; ++i) {
    int row = myVoigtToClassic[i][0];
    int column = myVoigtToClassic[i][1];
    // strain = TRUE means we need to double the non diagonal values of the
    // tensor
    // i >= dim means that in Voigt notation we're at non diagonal
    // terms
    (*voigt)(i) = (*tensor)(row, column) *
      ((strain && i >= dim) ? 2. : 1.);
  }
}

// Transform a second order tensor from voigt notation to classic notation
void tensorialTools::SOVoigt2Tensor(const VectorXd* voigt, MatrixXd* tensor,
                                    bool strain) {
  for (int i = 0; i < dim; ++i)
    for (int j = 0; j < dim; ++j)
      (*tensor)(i, j) = (*voigt)(myClassicToVoigt[i][j]) /
	((i != j && strain) ? 2. : 1.);
}

// Transform a forth order tensor from voigt notation to classic notation
void tensorialTools::FOVoigt2Tensor(const MatrixXd* FOVoigt,
				    double* tensor) {
  int dim3 = dim*dim*dim;
  int dim2 = dim*dim;
  for (int i = 0; i < dim; ++i) {
    for (int j = 0; j < dim; ++j) {
      for (int k = 0; k < dim; ++k) {
	for (int l = 0; l < dim; ++l) {
	 tensor[i*dim3 + j*dim2 + k*dim + l] =
	    (*FOVoigt)(myClassicToVoigt[i][j], myClassicToVoigt[k][l]);
	}
      }
    }
  }
}

// Transform a forth order tensor from classic notation to voigt notation
void tensorialTools::FOTensor2Voigt(const double * tensor,
				    MatrixXd* FOVoigt) {
  int dim3 = dim*dim*dim;
  int dim2 = dim*dim;
  for (int i = 0; i < voigtSize; ++i) {
    int row_i = myVoigtToClassic[i][0];
    int column_i = myVoigtToClassic[i][1];
    for (int j = 0; j < voigtSize; ++j) {
      int row_j = myVoigtToClassic[j][0];
      int column_j = myVoigtToClassic[j][1];
      (*FOVoigt)(i, j) =
	tensor[row_i*dim3 + column_i*dim2 + row_j*dim + column_j]/4.;
      (*FOVoigt)(i, j) =
	tensor[row_i*dim3 + column_i*dim2 + column_j*dim + row_j]/4.;
      (*FOVoigt)(i, j) =
	tensor[column_i*dim3 + row_i*dim2 + row_j*dim + column_j]/4.;
      (*FOVoigt)(i, j) =
	tensor[row_j*dim3 + column_j*dim2 + row_i*dim + column_i]/4.;
    }
  }
}

// scalar product with vector or Tensor 
void tensorialTools::scalarMatrixProduct(const double* scalar,
	const MatrixXd* Matrix,
	const int size, MatrixXd* product) {
  (*product) = scalar[0] * (*Matrix);
}

// dot product between two tensors
void tensorialTools::tensorDotProduct(const MatrixXd* tensorA,
                                      const MatrixXd* tensorB,
				      MatrixXd* product) {
    squareMatrixDotProduct(tensorA, tensorB, dim, product);
}

// dot product between two square matrices of size "size"
void tensorialTools::squareMatrixDotProduct(const MatrixXd* tensorA,
                                            const MatrixXd* tensorB,
                                            const int size,
					    MatrixXd* product) {
  (*product) = (*tensorA) * (*tensorB);
}

// Matrice. Vector product:  M.V (size "size")
void tensorialTools::matrixVectorProduct(const MatrixXd* MatriceA,
					 const VectorXd* VectorB,
					 const int size,
					 VectorXd* product) {
  (*product) = (*MatriceA) * (*VectorB);
}


// Vector dot Vector product:  V.V (size "size")
double tensorialTools::vectorDotVectorProduct(const VectorXd* VectorA,
                                            const VectorXd* VectorB,
                                            const int size) {
  return VectorA->dot(*VectorB);
}

// Computes the double contraction between two SOT
double tensorialTools::
tensorDoubleDotProduct(const MatrixXd* tensorA,
		       const MatrixXd* tensorB) {
  return ((*tensorA).cwiseProduct(*tensorB)).sum();
}

// computes the exponential map of a tensor based on equation 59
// Marin E.B. and Dawson P.R., On modelling the elasto-viscoplastic response
// of methals using polycrystal plasticity, CMAME, 165, 1998, 1-2:
// exp(Aij) = deltaij = sin(w)/w Aij (1-cos(w))/(w*w) Aik Akj
void tensorialTools::exponentialMap(const MatrixXd* tensorA, MatrixXd* exp) {
  if (dim != 3) {
    std::cerr << "MPCP::tensorialTools::exponentialMap not implemented for"
	      << "dim = " << dim << "\n";
    std::exit(1);
  }
  
  // Computation of w
  double w = sqrt(0.5 * tensorDoubleDotProduct(tensorA, tensorA));
  tensorDotProduct(tensorA, tensorA, SOTAuxiliar0);
  
  if (w) {
    for (int i = 0; i < dim; ++i)
      for (int j = 0; j < dim; ++j)
	(*exp)(i, j) =
	  i == j ? 1.
	  : 0. + sin(w) / w * (*tensorA)(i, j) +
	  (1. - cos(w)) / (w * w) *
	  (*SOTAuxiliar0)(i, j);
  } else {
    for (int i = 0; i < dim; ++i)
      for (int j = 0; j < dim; ++j)
	(*exp)(i, j) = i == j ? 1. : 0.;
  }
}

// Computes the cross product between two SOTensors in classic notation
// and returns a FOVoigt tensor
void tensorialTools::FOVoigtFromProduct(const MatrixXd* tensorA,
                                        const MatrixXd* tensorB,
                                        MatrixXd* FOVoigt) {
  for (int i = 0; i < voigtSize; ++i) {
    int row_i = myVoigtToClassic[i][0];
    int column_i = myVoigtToClassic[i][1];

    for (int j = 0; j < voigtSize; ++j) {
      int row_j = myVoigtToClassic[j][0];
      int column_j = myVoigtToClassic[j][1];
      (*FOVoigt)(i, j) =
	(*tensorA)(row_i, column_i) * (*tensorB)(row_j, column_j);
    }
  }
}

// Computes the contraction between a FOVoigt and a SOVoigt and returns a
// SOVoigt
void tensorialTools::FOVoigtDotSOVoigt(const MatrixXd* FOVoigt,
                                       const VectorXd* SOVoigt,
				       VectorXd* Voigt) {
  (*Voigt) = (*FOVoigt) * (*SOVoigt);
}

// Computes the inverse of a matrix A, the result is put in Y
void tensorialTools::inverse(const MatrixXd* A, const int order,
			     MatrixXd* Y) {
  (*Y) = (*A).inverse();
}

// calculate the cofactor of element (row,col)
void tensorialTools::getMinor(const MatrixXd* src, MatrixXd* dest,
			      int row, int col, const int order) {
  // indicate which col and row is being copied to dest
  int colCount = 0;
  int rowCount = 0;
  for (int i = 0; i < order; ++i) {
    if (i != row) {
      colCount = 0;
      for (int j = 0; j < order; ++j) {
	// when j is not the element
	if (j != col) {
	  (*dest)(rowCount, colCount) = (*src)(i, j);
	  colCount++;
	}
      }
      rowCount++;
    }
  }
}

// Calculate the determinant recursively.
double tensorialTools::det(const MatrixXd* mat, const int order) {
  return (*mat).determinant();
}

// Computes the rotation tensor R from the three Euler angles
void tensorialTools::eulerToR(const VectorXd* euler, MatrixXd* R) {
  double phi1 = (*euler)(0);  // first rotation around Z [0, 2pi]
  double psi  = (*euler)(1);   // second rotation around X' [0, pi]
  double phi2 = (*euler)(2);  // third rotation around Z'' [0, 2pi]

  (*R)(0,0) = ( cos(phi1) * cos(phi2) ) - ( sin(phi1) * cos(psi) * sin(phi2) );
  (*R)(0,1) = ( -cos(phi1) * sin(phi2) ) - ( sin(phi1) * cos(psi) * cos(phi2) );
  (*R)(0,2) = sin(phi1) * sin(psi);

  (*R)(1,0) = ( sin(phi1) * cos(phi2) ) + ( cos(phi1) * cos(psi) * sin(phi2) );
  (*R)(1,1) = ( -sin(phi1) * sin(phi2) ) + ( cos(phi1) * cos(psi) * cos(phi2) );
  (*R)(1,2) = -cos(phi1) * sin(psi);
	 
  (*R)(2,0) = sin(phi2) * sin(psi);
  (*R)(2,1) = cos(phi2) * sin(psi);
  (*R)(2,2) = cos(psi);
}

// Computes the three Euler angles from the rotation tensor R
void tensorialTools::RtoEuler(const MatrixXd* R, VectorXd* euler) {
  double phi1, psi, phi2;
  double R8 = (*R)(2,2);
    
  if (R8 > 1){
    R8=1.0;
    //std::cout << "R[8]= " << R[8] << std::endl;
  } else {
    if (R8 < (-1)){
      R8=-1.0;
      //std::cout << "R[8]= " << R[8] << std::endl;
    }
  }
    
  psi = acos(R8);
  if (R8 == 1){
    phi1 = atan2(-(*R)(0,1), (*R)(0,0));
    phi2 = 0 ;
  } else {
    if (R8 == -1){
      phi1 = atan2((*R)(0,1), (*R)(0,0));
      phi2 = 0 ;
    } else {
      phi1 = atan2((*R)(0,2),-(*R)(1,2));
      phi2 = atan2((*R)(2,0),(*R)(2,1));
    }
  }

  double pi = acos(-1.0);
  if (phi1 < 0){
    phi1 = phi1 + 2*pi;
  }
  if (phi2 < 0){
    phi2 = phi2 + 2*pi;
  } 
  (*euler)(0) = phi1;
  (*euler)(1) = psi;
  (*euler)(2) = phi2; 
}

//  Print the tensor
void tensorialTools::print(const MatrixXd* mat, const int order) {
    std::cout << "Matrix: " << *mat;
    std::cout << std::endl << std::endl;
}

// rotation of a SOT Bij = Rik Akl Rjl = R * A * R^T
void tensorialTools::RARt(const MatrixXd* R, const MatrixXd* A,
			  MatrixXd* B) {
  (*B) = (*R) * (*A) * ((*R).transpose());
}

// This sets the tensor to the identity
void tensorialTools::setIdentity(MatrixXd* A) {
  A->setIdentity();
}

// Given the normal n and the slip direction s, this function computes the
// schmidt tensor for a slip system
void tensorialTools::schmidtTensor(const VectorXd* n, const VectorXd* s,
                                   MatrixXd* schmidt) {
  (*schmidt) = (*n) * (s->transpose());
}

// Norm L2 of a second order tensor
double tensorialTools::SOTNorm(const MatrixXd* A) {
  return A->norm();
}

// Transforms a deviatoric second order tensor to cubic notation
void tensorialTools::DevSOTensor2Cubic(const MatrixXd* tensor, VectorXd * cubic){
  (*cubic)(0) = (*tensor).cwiseProduct(*(*cubicBase)[0]).sum();
  (*cubic)(1) = (*tensor).cwiseProduct(*(*cubicBase)[1]).sum();
  (*cubic)(2) = (*tensor).cwiseProduct(*(*cubicBase)[2]).sum();
  (*cubic)(3) = (*tensor).cwiseProduct(*(*cubicBase)[3]).sum();
  (*cubic)(4) = (*tensor).cwiseProduct(*(*cubicBase)[4]).sum();
}

// Transforms a deviatoric second order cubic tensor(vector) to classic notation
void tensorialTools::DevSOCubic2Tensor(const VectorXd* cubic, MatrixXd * tensor){
  (*tensor ) = (*cubic)(0)*(*(*cubicBase)[0]);
  (*tensor) += (*cubic)(1)*(*(*cubicBase)[1]);
  (*tensor) += (*cubic)(2)*(*(*cubicBase)[2]);
  (*tensor) += (*cubic)(3)*(*(*cubicBase)[3]);
  (*tensor) += (*cubic)(4)*(*(*cubicBase)[4]);
}

// Deviatoric 4 order tensor in cubic notation form the dyadic prouct of two classic tensors
void tensorialTools::DevF0CubicFromProduct(const MatrixXd* tensorA,
					   const MatrixXd* tensorB,
					   MatrixXd* cubic){
  VectorXd cubicA(5), cubicB(5);
  DevSOTensor2Cubic(tensorA, &cubicA);
  DevSOTensor2Cubic(tensorB, &cubicB);

  (*cubic) = cubicA * cubicA.transpose();

}

// Transforms a deviatoric fourth order cubic tensor to classic notation
void tensorialTools::DevFOCubic2Tensor(const MatrixXd* cubic,
				       double * tensor){
  int dim3 = dim*dim*dim;
  int dim2 = dim*dim;
  int b=5;
  for (int i=0; i < 3; i++) {
    for (int j=0; j < 3; j++) {
      for (int k=0; k < 3; k++) {
	for (int l=0; l < 3; l++) {
	  tensor[i*dim3 + j*dim2 + k*dim + l] = 0.;
	  for (int b1=0; b1 < b; b1++) {
	    for (int b2=0; b2 < b; b2++) {
	      tensor[i*dim3 + j*dim2 + k*dim + l] += (*cubic)(b1, b2)*
		(*(*cubicBase)[b1])(i, j) * (*(*cubicBase)[b2])(k, l);
	    }
	  }
	}
      }
    }
  }
}

// Transforms a fourth order cubic tensor to classic notation
void tensorialTools::FOCubic2Tensor(const MatrixXd* cubic,
				    double * tensor){
  int b=6;
  int dim3 = dim*dim*dim;
  int dim2 = dim*dim;
  for (int i=0; i < 3; i++) {
    for (int j=0; j < 3; j++) {
      for (int k=0; k < 3; k++) {
	for (int l=0; l < 3; l++) {
	  tensor[i*dim3 + j*dim2 + k*dim + l] = 0.;
	  for (int b1=0; b1 < b; b1++) {
	    for (int b2=0; b2 < b; b2++) {
	      tensor[i*dim3 + j*dim2 + k*dim + l] += (*cubic)(b1, b2)*
		(*(*cubicBase)[b1])(i, j) * (*(*cubicBase)[b2])(k, l);
	    }
	  }
	}
      }
    }
  }
}

//Trasforms a fourth order cubic tensor to a cubic base
void tensorialTools::FOTensor2Cubic(const double * tensor,
				    MatrixXd* FOCubic) {
  int dim3 = dim*dim*dim;
  int dim2 = dim*dim;
  for (int i=0; i<6; i++){
    for (int j=0; j<6; j++){
      (*FOCubic)(i, j) = 0;
      for (int k=0; k<3; k++){
	for (int l=0; l<3; l++){
	  for (int m=0; m<3; m++){
	    for (int n=0; n<3; n++){
	      (*FOCubic)(i, j) += tensor[k*dim3 + l*dim2 + m*dim + n]*
		(*(*cubicBase)[i])(k, l) *
		(*(*cubicBase)[j])(m, n);  
	    }
	  }
	}
      }
    }
  }
}

// destructor
tensorialTools::~tensorialTools() {
    for (int i = 0; i < dim; ++i) delete[] myClassicToVoigt[i];
    delete[] myClassicToVoigt;
    delete SOTAuxiliar0;
    delete SOTAuxiliar1;
    delete devProjection;

    if (kelvinBaseInitialized) {
      for (int i = 0; i < voigtSize; ++i)
	delete (*cubicBase)[i];
      delete cubicBase;
    }
}

}  // namespace MPCP
