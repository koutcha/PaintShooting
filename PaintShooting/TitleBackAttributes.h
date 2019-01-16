#pragma once
#include "Vector3.h"
#include <array>
#include "AdditionalAttributeSetter.h"
class TitleBackAttributes :
	public AdditionalAttributeSetter
{
public:
	TitleBackAttributes(const Vector3f& color0,const Vector3f& color1);
	~TitleBackAttributes();

	// AdditionalAttributeSetter ‚ğ‰î‚µ‚ÄŒp³‚³‚ê‚Ü‚µ‚½
	virtual void setAttributes(const Shader &) const override;
	virtual void unbindAttributes() const override;

	const std::array<Vector3f, 2> getColors()const;
private:
	std::array<Vector3f, 2> colors;
};

