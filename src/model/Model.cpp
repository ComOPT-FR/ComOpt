#include <Model.h> 

int CompositeShellSectionData::CreateNewLaminate(LaminateData::LaminateType iLaminateType, bool isVariableThickness)
{

	LaminateData temp{ iLaminateType, 1, true, isVariableThickness};
	COMOPT_ASSERT(GetNumberOfLaminates() == 0, "The new laminate will overwrite the old one", 1)
	_spLaminatesDataVector->clear();
	_spLaminatesDataVector->push_back(temp);

	return 1;

}

int CompositeBeamSectionData::CreateNewLaminate(LaminateData::LaminateType iLaminateType, bool isVariableThickness)
{

	LaminateData temp{ iLaminateType, _LastCreatedLaminateNmber++, true, isVariableThickness };
	_spLaminatesDataVector->push_back(temp);

	return _LastCreatedLaminateNmber;

}

void CompositeSectionData::CreateSectionOfSingleLaminatesType(LaminateData::LaminateType iLaminateType, bool isVariableThickness)
{
	for (int i = 0; i++; i < _NumberOfLaminates)
	{
		CreateNewLaminate(iLaminateType, isVariableThickness);
	}

}