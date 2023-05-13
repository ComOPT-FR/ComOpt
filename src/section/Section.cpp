#include <Section.h>

int SectionBase::AddDesignVariableToSection(std::shared_ptr<DesignVariableBase> iDesignVariable)
{
	_spSectionDesignVariables->push_back(iDesignVariable);
	DesignVariableBase::DesignVariableType designVarType = iDesignVariable->GetDesignVariableType();
	if (designVarType == DesignVariableBase::DesignVariableType::LaminateType) _NumberOfLaminates++;
	else if (designVarType == DesignVariableBase::DesignVariableType::ScalarType) _NumberOfScalarVariables++;
	return _NumberOfDesignVariables++;
}