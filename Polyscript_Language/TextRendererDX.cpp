#include "stdafx.h"
TextRendererDX::TextRendererDX(){
	textBox=std::shared_ptr<TextBox>(new TextBox());
	textBox->setPadding(0,0);
	textBox->setEditable(false);
	textBox->setEnabled(true);
	textBox->setFont(L"Courier New");
	textBox->setForeground(0.f,0.f,0.f,1.f);
	textBox->setHorizontalTextAlignment(HorizontalConstants::LEFT);
	textBox->setVerticalTextAlignment(VerticalConstants::TOP);
	textBox->setLocale(L"en-US");
	textBox->setOpacity();	
	textBox->setTextSize(12.f);
	textBox->setVisible(true);
	D2DUISystem::getInstance().addDrawToQueue(1,*textBox->drawing);
	/*lines=std::shared_ptr<TextLabel>(new TextLabel());
	lines->setFont(L"Courier New");
	lines->setForeground(0.f,0.f,0.f,1.f);
	lines->setOpacity();
	lines->setLocale(L"en-US");
	lines->setPadding(5,0);
	lines->setTextSize(12.f);
	lines->setVisible(true);
	lines->setHorizontalTextAlignment(HorizontalConstants::RIGHT);
	lines->setVerticalTextAlignment(VerticalConstants::TOP);
	D2DUISystem::getInstance().addDrawToQueue(1,*lines->drawing);*/
}
void TextRendererDX::recreateTextRendererDX(std::shared_ptr<D2DHandle>&d2d,GameSettings settings,PolyscriptParser parser){
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
	//textBox->setPadding(0,0,0,0);
	textBox->recreateResources(d2d);
}