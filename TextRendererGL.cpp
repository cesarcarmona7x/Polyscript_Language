#include "stdafx.h"
TextRendererGL::TextRendererGL(){
	textBox=std::shared_ptr<GLTextBox>(new GLTextBox());
	textBox->setPadding(0,0);
	textBox->setEditable(true);
	textBox->setEnabled(true);
	textBox->setFont(L"Courier New");
	textBox->setForeground(0.f,0.f,0.f,1.f);
	textBox->setHorizontalTextAlignment(GLHorizontalConstants::LEFT);
	textBox->setVerticalTextAlignment(GLVerticalConstants::TOP);
	textBox->setLocale(L"en-US");
	textBox->setOpacity();	
	textBox->setTextSize(12.f);
	textBox->setVisible(true);
	OpenGLUISystem::getInstance().addDrawToQueue(1,*textBox->drawing);
}
void TextRendererGL::recreateTextRendererGL(std::shared_ptr<GLHandle>&gl,GameSettings settings,PolyscriptParser parser){
	if(textBox->getText().length()==0){
		std::wstring text=L"";
		if(parser.fileLines.size()!=0){
			for(int i=0;i<parser.fileLines.size();i++){
				text+=parser.fileLines.at(i)+std::wstring(L"\n");
			}
		}
		textBox->setText(text);
	}
	textBox->setBounds(0,0,settings.winwidth/2,settings.winheight);
	textBox->recreateResources(gl);
}