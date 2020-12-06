#ifndef TEXTRENDERERGL_INCLUDED
#define TEXTRENDERERGL_INCLUDED
#include "stdafx.h"
class TextRendererGL{
public:
	TextRendererGL();
	~TextRendererGL(){}
	void recreateTextRendererGL(std::shared_ptr<GLHandle>& gl,GameSettings settings,PolyscriptParser parser);
	void renderTextRendererGL(std::shared_ptr<GLHandle>& gl);
	std::shared_ptr<GLTextBox>textBox;
	//std::shared_ptr<GLTextLabel>lines;
};
#endif