#pragma once
class Shader;
class AdditionalAttributeSetter
{
public:
	virtual ~AdditionalAttributeSetter() {};
	virtual void setAttributes(const Shader&)const = 0;
	virtual void unbindAttributes()const = 0;
};

