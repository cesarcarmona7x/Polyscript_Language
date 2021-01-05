#ifndef TEXTRENDERERDX_INCLUDED
#define TEXTRENDERERDX_INCLUDED
#include "stdafx.h"
class TextRendererDX{
public:
	TextRendererDX();
	void recreateTextRendererDX(std::shared_ptr<D2DHandle>& d2d,GameSettings settings,PolyscriptParser parser);
	void renderTextRendererDX(std::shared_ptr<D2DHandle>& d2d);
	std::shared_ptr<TextBox>textBox;
	std::shared_ptr<TextLabel>lines;
};
#endif