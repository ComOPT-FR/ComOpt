#ifndef COMOPT_MODEL_H
#define COMOPT_MODEL_H

#include <section/Section.h> 
#include <vector>
#include <memory>
#include <string>
#include <utils/Error.h> //Standard error handling macros


class ApproximationFunction;

struct LaminateData
{
	enum LaminateType
	{
		OnlySymmetric,
		OnlyBalanced,
		SymmetricAndBalanced,
		General
	} _LaminateType;

	enum MaterialType
	{
		SingleMaterial,
		MultiMaterial

	} _MaterialType;
	int          _LaminateNumber;
	bool         _IsThicknessVariable;

};

class ResponseData
{
public:
	ResponseData(const int& iNumberOfVariables, std::string iName) :
		_Name(iName), _NumberOfVariables(iNumberOfVariables) {}

	inline void SetResponseNumberOfVariables(const int& iNumberOfVariables) { _NumberOfVariables = iNumberOfVariables; }
	inline int GetResponseNumberOfVariables() const { return _NumberOfVariables; }

	inline void SetResponseName(const std::string& iResponseName) { _Name = iResponseName; }
	inline std::string GetResponseName() const { return _Name; }

	inline void SetResponseValue(const double& iResponseValue) { _Value = iResponseValue; }
	inline double GetResponseValue() const { return _Value; }

	inline void SetResponseSensitivities(const std::vector<double>& iSensitivities) { _Sensitivities = iSensitivities; }
	inline void GetResponseSensitivities(std::vector<double>& oSensitivities) const { oSensitivities = _Sensitivities; }

private:
	std::string _Name;
	double _Value;
	int _NumberOfVariables;
	std::vector<double> _Sensitivities;

};

typedef LaminateData::LaminateType LaminateType;
typedef LaminateData::MaterialType MaterialType; 

class SectionProfile
{

public:
	virtual int GetNumberOfLaminates() = 0;
	virtual std::string GetProfileName() = 0;

};

// An example for the specialization of Section profile

class ISectionProfile : public SectionProfile
{
	int GetNumberOfLaminates() { return 3; }
	std::string GetProfileName() { return "I Section"; }
};


class SectionData
{
public:

	~SectionData() {};

	// Non-Virtual methods
	inline int  GetSectionLaminatesData(std::shared_ptr<std::vector<LaminateData>>& oSectionData) const { oSectionData = _spLaminatesDataVector; return _NumberOfLaminates;}
	inline int  GetSectionScalarData(std::shared_ptr<std::vector<double>>& oScalarData) const { oScalarData = _spScalarDataVector; return _NumberOfScalars; }

	inline void GetLaminateType(int iLaminateNumber, LaminateType& oLaminateType) const;
	inline void GetScalarVariable(int iScalarVariableNumber, double& oScalarVariable) const;
	inline void IsLaminateHasVariableThickness(int iLaminateNumber, bool& oIsVariableThickness) const;

	inline int  GetNumberOfLaminates() const { return _NumberOfLaminates; }
	inline int  GetNumberOfScalarVariables() const { return _NumberOfScalars; }
	inline int  Size() const { return _NumberOfLaminates + _NumberOfScalars; }

	// Virtual methods
	virtual SectionBase::SectionType GetSectionType() const { return SectionBase::SectionType::UnknownSectionType; };
	virtual void CreateSectionOfSingleLaminatesType(LaminateType iSectionLaminateType, MaterialType iMaterialType, bool isVariableThickness = false);

	// Pure virtual methods
	virtual int CreateNewLaminateData(LaminateType iSectionLaminateType, MaterialType iMaterialType, bool isVariableThickness = false) = 0;
	int CreateNewScalarVariableData(double iInitialValue = 0.0); // Should we initialize the scalar data value as 0.0?
protected:
	// TODO: Should we associate a name with each scalar object for a better accessibility?
	std::shared_ptr<std::vector<LaminateData>> _spLaminatesDataVector;
	std::shared_ptr<std::vector<double>> _spScalarDataVector;
	int _NumberOfLaminates;
	int _NumberOfScalars;

protected:
	SectionData() 
	{
		_spLaminatesDataVector = nullptr;
		_spScalarDataVector = nullptr;
	}
private:
	inline void GetLaminateData(int iLaminateNumber, LaminateData& oLaminateData) const;

};

class CompositeShellSectionData : public SectionData
{
public:
	~CompositeShellSectionData() {};

	CompositeShellSectionData() : SectionData() {}

	SectionBase::SectionType GetSectionType() const { return SectionBase::SectionType::CompositeShell; }

	int CreateNewLaminateData(LaminateType iSectionLaminateType, MaterialType iMaterialType, bool isVariableThickness);


};

class CompositeBeamSectionData : public SectionData

{
public:

	virtual ~CompositeBeamSectionData() {};

	// This constructor sets the number of laminates based on the beam profile
	CompositeBeamSectionData(const std::shared_ptr<SectionProfile>& iSectionProfile)
		: SectionData(), _SectionProfile(iSectionProfile) {}
	SectionBase::SectionType GetSectionType() const { return SectionBase::SectionType::CompositeBeam; }
	int CreateNewLaminateData(LaminateType iSectionLaminateType, MaterialType iMaterialType, bool isVariableThickness);

private:
	std::shared_ptr<SectionProfile> _SectionProfile;
};





class Model
{
public:

	Model();
	~Model() {};

	//This method should read a standard input file, create the new sections and responses data 
	void ReadSectionsAndResponsesDataFromInputFile(); 

	// Read responses and sensitivities data from results file
	void ReadResponsesAndSensitivitiesValues();

	// Create new approximation function based on the responses and sensitivites values
	void CreateApproximationFunction();

	// This method creates the sections list from the list "_spSectionsDataVector" that has been built from the input file
	void GetSectionsList(std::shared_ptr<std::vector<std::shared_ptr<SectionBase>>>& oSectionsList) const { oSectionsList = _spSectionsVector; }
	
	// Create new sections data
	int  CreateNewSection(std::string iSectionName, SectionBase::DesignType iDesigType, const std::unique_ptr<SectionData>& iSctionData);
	void GetSectionList(std::shared_ptr<std::vector<std::shared_ptr<SectionBase>>>& oSectionsList) const { oSectionsList = _spSectionsVector; }

	// Interaction between analysis and design
	//===============================================================================
	// Analysis ---> Design
	void GetStiffnessFromAnalysis(int SectionNumber, void* iStiffness) const;
	void GetThicknessFromAnalysis(int SectionNumber, double* iThickness) const;
	void GetScalarVariablesFromAnalysis(int SectionNumber, void* iScalarVariables) const;
	void GetSensitivitiesFromAnalysis(const void* iSensitivities) const;
	void GetResponsesFromAnalysis(const void* iResponses) const;

	// Design ---> Analysis
	void GetStiffnessFromDesign(int SectionNumber, void* iStiffness) const;
	void GetThicknessFromDesign(int SectionNumber, double* iThickness) const;
	void GetScalarVariablesFromDesign(int SectionNumber, void* iScalarVariables) const;
	//===============================================================================

	// Run the main optimization Process
	void RunOptimizationProcess();

private:
	std::shared_ptr<std::vector<std::shared_ptr<SectionBase>>> _spSectionsVector;          //This vector should bee referenced by the optimizer
	std::shared_ptr<ApproximationFunction>                     _spApproximationFunction;

	std::vector<SectionData>                                   _spSectionsDataVector;      // This vector contains all Section Data objects
	std::vector<ResponseData>                                  _spResponsesDataVector;     // This vector contains data about responses
};


void SectionData::GetLaminateData(int iLaminateNumber, LaminateData& oLaminateData) const
{
	COMOPT_ASSERT(iLaminateNumber < _NumberOfLaminates, "The laminate number is greater than the size of the number of laminates", 0)
	oLaminateData = _spLaminatesDataVector->at(iLaminateNumber);
}

void SectionData::GetLaminateType(int iLaminateNumber, LaminateType& oLaminateType) const
{
	LaminateData laminateData;
	GetLaminateData(iLaminateNumber, laminateData);
	oLaminateType = laminateData._LaminateType;
}

void SectionData::IsLaminateHasVariableThickness(int iLaminateNumber, bool& oIsVariableThickness) const
{
	LaminateData laminateData;
	GetLaminateData(iLaminateNumber, laminateData);
	oIsVariableThickness = laminateData._IsThicknessVariable;
}
 

void SectionData::GetScalarVariable(int iScalarVariableNumber, double& oScalarVariable) const
{ 
	COMOPT_ASSERT(iScalarVariableNumber < _NumberOfScalars, "The laminate number is greater than the size of the number of scalar variables", 0)
	oScalarVariable = _spScalarDataVector->at(iScalarVariableNumber);
}


#endif