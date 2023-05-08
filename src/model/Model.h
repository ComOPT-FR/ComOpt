#ifndef COMOPT_MODEL_H
#define COMOPT_MODEL_H

#include <Section.h> 
#include <vector>
#include <memory>
#include <string>
#include <error.h> //Standard error handling macros

class ScalarDesignData;

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

struct LaminateData
{
	enum LaminateType
	{
		UnknownLaminateType,
		OnlySymmetric,
		OnlyBalanced,
		SymmetricAndBalanced,
		General
	} _LaminateType;

	int          _LaminateNumber;
	bool         _IsThicknessVariable;
	bool         _IsIncludedInDesign;

};

class CompositeSectionData
{
public:
	enum SectionType
	{
		UnknownSectionType,
		CompositeShell,
		CompositeBeam
	};

	~CompositeSectionData() {};

	// Non-Virtual methods
	void GetSectionLaminatesData(std::shared_ptr<std::vector<LaminateData>>& oSectionData) { oSectionData = _spLaminatesDataVector; };
	void ExcludeLaminateFromDesign(int iLaminateNumber) { GetLaminateData(iLaminateNumber)._IsIncludedInDesign == false; }
	int  GetNumberOfLaminates() { return _NumberOfLaminates; };
	LaminateData& GetLaminateData(int iLaminateNumber) { return _spLaminatesDataVector->at(iLaminateNumber); }

	// Virtual methods
	virtual SectionType GetSectionType() { return UnknownSectionType; };
	virtual void CreateSectionOfSingleLaminatesType(LaminateData::LaminateType iLaminateType, bool isVariableThickness = false);

	// Pure virtual methods
	virtual int CreateNewLaminate(LaminateData::LaminateType iLaminateType, bool isVariableThickness = false) = 0;
	CompositeSectionData() {};

protected:
	std::shared_ptr<std::vector<LaminateData>> _spLaminatesDataVector;
	int _NumberOfLaminates;

protected:
	CompositeSectionData(int iNumberOfLaminates) : _NumberOfLaminates(iNumberOfLaminates) {}

};

class CompositeShellSectionData : public CompositeSectionData
{
public:
	~CompositeShellSectionData() {};

	CompositeShellSectionData() : CompositeSectionData(1)
	{
		_spLaminatesDataVector = std::make_shared<std::vector<LaminateData>>(1);
	}

	SectionType GetSectionType() { return CompositeShell; }

	int CreateNewLaminate(LaminateData::LaminateType iLaminateType, bool isVariableThickness);


};

class CompositeBeamSectionData : public CompositeSectionData

{
public:
	virtual ~CompositeBeamSectionData() {};

	// This constructor sets the number of laminates based on the user input
	CompositeBeamSectionData(int iNumberOfLaminates, const std::shared_ptr<SectionProfile>& iSectionProfile)
	:CompositeSectionData(iNumberOfLaminates), _LastCreatedLaminateNmber(0), _SectionProfile(iSectionProfile)
	{		
		_spLaminatesDataVector = std::make_shared<std::vector<LaminateData>>(iNumberOfLaminates);
	}
	// This constructor sets the number of laminates based on the beam
	CompositeBeamSectionData(const std::shared_ptr<SectionProfile>& iSectionProfile)
	: _LastCreatedLaminateNmber(0), _SectionProfile(iSectionProfile)
	{
		InitializeSectionData();
	}


	SectionType GetSectionType() { return CompositeBeam; }
	int CreateNewLaminate(LaminateData::LaminateType iLaminateType, bool isVariableThickness);

private:
	void InitializeSectionData() 
	{
		if (NULL_SMART != _SectionProfile) _NumberOfLaminates = _SectionProfile->GetNumberOfLaminates();
	}
	std::shared_ptr<SectionProfile> _SectionProfile;
	int _LastCreatedLaminateNmber;
};

class Model
{
private:

public:

	// Design types enum
	enum DesignType
	{
		UnknownDesignType,
		LaminateDesign,
		ScalarParametersDesign,
		MixedDesign
	};

	~Model() {};

	//This method should read a standard input file, create the new sections and store them in "_spSectionsVector"
	void BuildSectionsDataFromInputFile(); 
	void CreateNewSection(DesignType iDesigType, std::shared_ptr<CompositeSectionData> iCompositeSctionData, std::shared_ptr <ScalarDesignData> iScalarDesignData);
	

private:
	std::shared_ptr<std::vector<Section>> _spSectionsVector; //This vector should bee referenced by the optimizer object

};



#endif