#include <Model.h> 


int CompositeShellSectionData::CreateNewLaminateData(LaminateType iSectionLaminateType, MaterialType iSectionMaterialType, bool isVariableThickness)
{

	LaminateData temp{ iSectionLaminateType, iSectionMaterialType, 1, isVariableThickness};
	COMOPT_ASSERT(GetNumberOfLaminates() == 0, "The new laminate will overwrite the old one", 1)
	_spLaminatesDataVector->clear();
	_spLaminatesDataVector->push_back(temp);

	return 1;
}

int CompositeBeamSectionData::CreateNewLaminateData(LaminateType iSectionLaminateType, MaterialType iSectionMaterialType, bool isVariableThickness)
{
	int laminateNumber = _NumberOfLaminates; //Number of the laminate
	LaminateData temp{ iSectionLaminateType, iSectionMaterialType, _NumberOfLaminates++, isVariableThickness };
	_spLaminatesDataVector->push_back(temp);

	return laminateNumber; 

}


void SectionData::CreateSectionOfSingleLaminatesType(LaminateType iSectionLaminateType, MaterialType iSectionMaterialType, bool isVariableThickness)
{
	for (int i = 0; i < _NumberOfLaminates; i++)
	{
		CreateNewLaminateData(iSectionLaminateType, iSectionMaterialType, isVariableThickness);
	}

}

int SectionData::CreateNewScalarVariableData(double iInitialValue)
{
	int scalareNumber = _NumberOfScalars; //Number of scalar variable
	_spScalarDataVector->push_back(iInitialValue);
	_NumberOfScalars++;

	return scalareNumber; 

}

Model::Model()
{
	_spSectionsVector = std::make_shared<std::vector<std::shared_ptr<SectionBase>>>(1);
}

int Model::CreateNewSection(std::string iSectionName, SectionBase::DesignType iDesigType, const std::unique_ptr<SectionData>& iSctionData)
{

	COMOPT_ASSERT(!(iDesigType == SectionBase::UnknownDesignType), "Unknown design type has been used",1);
	bool assert = (iDesigType == SectionBase::ScalarParametersDesign || iDesigType == SectionBase::MixedDesign) & (iSctionData->GetNumberOfScalarVariables() != 0);
	COMOPT_ASSERT(assert, "A scalar parametric design cannot be initialized without any scalar variables", 1);

	// Creation of the new section
	std::shared_ptr<SectionBase> spNewSection = nullptr;
	if (iSectionName == "CompositeShellSection")
	{
		spNewSection = std::make_shared<CompositeShellSection>(iDesigType);

	}
	else if (iSectionName == "CompositeBeamSection")
	{
		spNewSection = std::make_shared<CompositeBeamSection>(iDesigType);
	}

	// For the moment, only single material with one sublaminates number (=5) is considered
	typedef LaminateBase<Laminate<MaterialType::SingleMaterial, false, false, 5>> General;
	typedef LaminateBase<Laminate<MaterialType::SingleMaterial, true, false, 5>> OnlyBalanced;
	typedef LaminateBase<Laminate<MaterialType::SingleMaterial, false, true, 5>> OnlySymmetric;
	typedef LaminateBase<Laminate<MaterialType::SingleMaterial, false, false, 5>> SymmetricAndBalanced;

	std::shared_ptr<DesignVariableBase> spNewDesignVariable = nullptr;

	for (int i = 0; i < iSctionData->GetNumberOfLaminates(); i++)
	{
		LaminateType laminateType;
		iSctionData->GetLaminateType(i, laminateType);
		bool bVariableThickness;
		iSctionData->IsLaminateHasVariableThickness(i, bVariableThickness);

		switch (laminateType)
		{
			case LaminateType::General:
				spNewDesignVariable = std::make_shared<General>(i, bVariableThickness);
				break;
			case LaminateType::OnlyBalanced:
				spNewDesignVariable = std::make_shared<OnlyBalanced>(i, bVariableThickness);
				break;
			case LaminateType::OnlySymmetric:
				spNewDesignVariable = std::make_shared<OnlySymmetric>(i, bVariableThickness);
				break;
			case LaminateType::SymmetricAndBalanced:
				spNewDesignVariable = std::make_shared<SymmetricAndBalanced>(i, bVariableThickness);
				break;
			default: 
				spNewDesignVariable = std::make_shared<General>(i, bVariableThickness);
				break;
		}

		if (nullptr != spNewDesignVariable)
		{
			spNewSection->AddDesignVariableToSection(spNewDesignVariable);
			spNewDesignVariable = nullptr;
		}
	}

	for (int i = 0; i < iSctionData->GetNumberOfScalarVariables(); i++)
	{
		double scalarVariable;
		iSctionData->GetScalarVariable(i, scalarVariable);
		spNewDesignVariable = std::make_shared<ScalarVariable>(i, scalarVariable);
		if (nullptr != spNewDesignVariable) spNewSection->AddDesignVariableToSection(spNewDesignVariable);
		spNewDesignVariable = nullptr;
	}
	
	_spSectionsVector->push_back(spNewSection);
	return _spSectionsVector->size();

}


