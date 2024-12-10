#pragma once

#include "../StylesStore.h"

class StyledComponent
{
public:
	explicit StyledComponent(const StylesStore& stylesStore);

protected:
	const StylesStore& stylesStore;
};
