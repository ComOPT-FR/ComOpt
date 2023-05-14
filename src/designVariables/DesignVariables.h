#ifndef COMOPT_DESIGNVARIABLES_H
#define COMOPT_DESIGNVARIABLES_H

class DesignVariableBase
{
public:
	enum DesignVariableType
	{
		UnknownType,
		LaminateType,
		ScalarType
	};
	virtual ~DesignVariableBase() {};
	DesignVariableType GetDesignVariableType() { return GetDesignVariableTypeImpl(); };
private:
	virtual DesignVariableType GetDesignVariableTypeImpl() = 0;
protected:
	int _DesignVariableNumber;

};

template<typename childclass>
class LaminateBase : public DesignVariableBase
{
public:
	LaminateBase(int iLaminateNmber, bool iIsVariableThickness)
	{
		childclass* pDesignVariable = static_cast<childclass*> (this);
		pDesignVariable->SetLaminateData(iLaminateNmber, iIsVariableThickness);
	}
	virtual ~LaminateBase() {}

private:

	DesignVariableBase::DesignVariableType GetDesignVariableTypeImpl()
	{
		childclass* pDesignVariable = static_cast<childclass*> (this);
		DesignVariableBase::DesignVariableType DesignVariableType =  pDesignVariable->GetDesignVariableType();
		return DesignVariableType;
	}


};


template<bool Type, bool isBalanced, bool isSymmetric = true, int NSUBLAM = 5>
class Laminate : public LaminateBase<Laminate<Type, isBalanced, isSymmetric, NSUBLAM>>
{
public:


	void SetLaminateData(int iLaminateNmber, bool iIsVariableThickness)
	{
		this->_DesignVariableNumber = iLaminateNmber;
		_IsVariableThickness = iIsVariableThickness;
	}
	DesignVariableBase::DesignVariableType GetDesignVariableType() { 
		return this->LaminateType;
	}
    ~Laminate() {};

private:
	bool _IsVariableThickness;
};


class ScalarVariable : public DesignVariableBase
{
private:
	DesignVariableBase::DesignVariableType GetDesignVariableTypeImpl() { return ScalarType; }
public:
	ScalarVariable(int iScalarNmber, double iScalarValue)
	{
		this->_DesignVariableNumber = iScalarNmber;
		_Value = iScalarValue;
	}
	~ScalarVariable() {};
private:
	double _Value;

};

#endif