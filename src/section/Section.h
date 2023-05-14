#ifndef COMOPT_SECTION_H
#define COMOPT_SECTION_H

#include <designVariables/DesignVariables.h>
#include <section/StiffnessConverter.h>
#include <vector>
#include <memory>
#include <string>
#include <utils/Error.h> //Standard error handling macros


class SectionBase
{
public:

	// Design types enum
	enum DesignType
	{
		UnknownDesignType,
		LaminateDesign,
		ScalarParametersDesign,
		MixedDesign
	};


	enum SectionType
	{
		UnknownSectionType,
		CompositeShell,
		CompositeBeam
	};
			
	virtual ~SectionBase() {};
	SectionBase() : _DesignType(UnknownDesignType), _NumberOfDesignVariables(0), 
		_NumberOfLaminates(0), _NumberOfScalarVariables(0) {};
	SectionBase(DesignType iDesigType) : _DesignType(iDesigType), _NumberOfDesignVariables(0),
		_NumberOfLaminates(0), _NumberOfScalarVariables(0) {};

	// Non-virtual methods
	DesignType GetDesignType() const { return _DesignType; }
	int GetNumberOfLaminates() const { return _NumberOfScalarVariables; }
	int GetNumberOfScalarVariables() const { return _NumberOfLaminates; }
	int GetNumberOfDesignVariables() const { return _NumberOfScalarVariables + _NumberOfLaminates; }

	int AddDesignVariableToSection(std::shared_ptr<DesignVariableBase> iDesignVariable);
	const std::shared_ptr<DesignVariableBase>& GetDesignVariable(int iDesignVariableNumber) { return _spSectionDesignVariables->at(iDesignVariableNumber); }

	// Virtual methods
	virtual SectionType GetSectionType() { return UnknownSectionType; }

	// Pure virtual methods
	virtual std::string GetSectionName() = 0;
	//virtual void TransformLaminationParametersToStiffness() = 0;
	//virtual void TransformStiffnessToLaminationParameters() = 0;
private:
	std::shared_ptr<std::vector<std::shared_ptr<DesignVariableBase>>> _spSectionDesignVariables;
	DesignType _DesignType;
	int _NumberOfDesignVariables;
	int _NumberOfLaminates;
	int _NumberOfScalarVariables;
	//std::unique_ptr<StiffnessConverter> _spSectionConverter;
};

class CompositeShellSection : public SectionBase
{
public:
	CompositeShellSection(DesignType iDesigType): SectionBase(iDesigType) {}
	virtual ~CompositeShellSection() {}
	std::string GetSectionName() { return "Composite shell section"; }
	SectionType GetSectionType() {return CompositeShell;}
	//void TransformLaminationParametersToStiffness();
	//void TransformStiffnessToLaminationParameters();
};

class CompositeBeamSection : public SectionBase
{
public:
	CompositeBeamSection(DesignType iDesigType) : SectionBase(iDesigType) {}
	virtual ~CompositeBeamSection() {}
	std::string GetSectionName() { return "Composite beam section"; }
	SectionType GetSectionType() { return CompositeBeam; }
	//void TransformLaminationParametersToStiffness();
	
	
	
	//void TransformStiffnessToLaminationParameters();
};




#endif