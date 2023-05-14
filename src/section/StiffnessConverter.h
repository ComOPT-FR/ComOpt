#ifndef COMOPT_STIFFNESSCONVERTER_H
#define COMOPT_STIFFNESSCONVERTER_H

#include <iostream>
#include <Eigen/Dense>
#include <utils/Error.h> //Standard error handling macros

class StiffnessConverter
{
public:

	StiffnessConverter(const Eigen::Matrix<double, Eigen::Dynamic, 1>& iLaminationParameters) :
		_LaminationParametersSize(iLaminationParameters.size()), _LaminationParameters(iLaminationParameters)
	{
		State |= LAMINATION_PARAMETERS;
	};
	StiffnessConverter(const Eigen::Matrix<double, 6, 6>& iStiffnessMatrix) : _StiffnessMatrix(iStiffnessMatrix)
	{
		State |= STIFFNESS;
	};
	virtual ~StiffnessConverter() {};
	virtual void ConvertLaminationParametersToStiffness() = 0;
	virtual void ConvertStiffnessToLaminationParameters() = 0;
protected:
	int _LaminationParametersSize;
	int State;
	Eigen::Matrix<double, Eigen::Dynamic, 1> _LaminationParameters;
	Eigen::Matrix<double, 6, 6> _StiffnessMatrix;
private:
	enum StateMasks
	{
		LAMINATION_PARAMETERS = 1,
		STIFFNESS = 2,
	};
};

class CompositeShellStiffnessConverter : public StiffnessConverter {
public:
	CompositeShellStiffnessConverter(const Eigen::Matrix<double, Eigen::Dynamic, 1>& iLaminationParameters) :
		StiffnessConverter(iLaminationParameters) {};
	CompositeShellStiffnessConverter(const Eigen::Matrix<double, 6, 6>& iStiffnessMatrix) : 
		StiffnessConverter(iStiffnessMatrix) {};
	void ConvertLaminationParametersToStiffness();
	void ConvertStiffnessToLaminationParameters();

};

class CompositeBeamStiffnessConverter : public StiffnessConverter {
public:
	CompositeBeamStiffnessConverter();
	void ConvertLaminationParametersToStiffness();
	void ConvertStiffnessToLaminationParameters();


};
#endif