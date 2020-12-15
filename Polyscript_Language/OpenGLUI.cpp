#include "stdafx.h"
using namespace OpenGLUI;
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#define STB_IMAGE_RESIZE_IMPLEMENTATION
#include "stb_image_resize.h"
float PixelToGLPoint(float pixel,float dimensionSize){
	if(pixel<dimensionSize/2.f){
		return (pixel/(dimensionSize/2.f))-1.f;
	}
	else if(pixel==dimensionSize/2.f){
		return 0.f;
	}
	else{
		return (pixel-(dimensionSize/2.f))/(dimensionSize-(dimensionSize/2.f));
	}
}
const char* getErrorMessage(FT_Error err)
{
#undef FTERRORS_H_
#define FT_ERRORDEF( e, v, s )  case e: return s;
#define FT_ERROR_START_LIST     switch (err) {
#define FT_ERROR_END_LIST       }
#include FT_ERRORS_H
	return "(Unknown error)";
}
namespace OpenGLUI{
	OpenGLUISystem& OpenGLUISystem::getInstance(){
		static OpenGLUISystem OpenGLUI;
		return OpenGLUI;
	}
	void OpenGLUISystem::addDrawToQueue(int layer,OpenGLUIDrawableItem& draw){
		drawOrder[layer].push_back(draw);
	}
	void OpenGLUISystem::draw(std::shared_ptr<GLHandle>& gl){
		for(auto& map:drawOrder){
			for(auto& drawing:map.second){
				if(drawing.get().isVisible()){
					if(map.first%2==0){
						if(drawing.get().drawWindow.size()!=0){
							/*if(strcmp(typeid(drawing.get().drawWindow.at(0).get()).name(),"class OpenGLUI::ComboBox")==0){
							ComboBox& cb=(ComboBox&)drawing.get().drawWindow.at(0).get();
							if(cb.usesCustomDrawMode()){
							cb.customDraw(gl);
							cb.customDrawDropdown(gl);
							}
							else{
							cb.draw(d2d);
							cb.drawDropdown(d2d);
							}
							}*/
						}
					}//Check even layers for ComboBox drawing
					else{
						if(drawing.get().drawWindow.size()!=0){
							if(strcmp(typeid(drawing.get().drawWindow.at(0).get()).name(),"class OpenGLUI::ListBox")==0){
								/*ListBox& lb=(ListBox&)drawing.get().drawWindow.at(0).get();
								if(lb.usesCustomDrawMode()){lb.customDraw(d2d);}
								else{lb.draw(d2d);}*/
							}
							else{
								drawing.get().drawWindow.at(0).get().draw(gl);
							}
						}
						if(drawing.get().drawLayout.size()!=0){
							drawing.get().drawLayout.at(0).get().draw(gl);
						}
					}//Check odd layers for any other drawing
				}
			}
		}
	}
	void GLWindowBase::setParentLayout(std::shared_ptr<GLLayoutBase> parent){
		this->parent=parent;
	}
	std::shared_ptr<GLLayoutBase> GLWindowBase::getParentLayout(){
		return parent;
	}
	void GLLayoutBase::setParentLayout(std::shared_ptr<GLLayoutBase> parent){
		this->parent=parent;
	}
	std::shared_ptr<GLLayoutBase> GLLayoutBase::getParentLayout(){
		return parent;
	}
	void GLLayoutBase::setVisible(bool visible){
		drawing->setVisible(visible);
	}
	bool GLLayoutBase::isVisible(){
		return drawing->isVisible();
	}
	GLTextBox::GLTextBox(std::wstring text){
		states.push_back(NONE);
		GLTextBox::text=text;
		setHorizontalTextAlignment(GLHorizontalConstants::LEFT);
		setVerticalTextAlignment(GLVerticalConstants::CENTER_VERTICAL);
		setForeground(0,0,0);
		setTextSize(12.0f);
		setFont(L"Microsoft Sans Serif");
		drawing=new OpenGLUIDrawableItem(*this,false);
		//setImageQuality();
	}
	void GLTextBox::measureCaret(int x,int y){
		DWORD caretWidth=1;
		SystemParametersInfo(SPI_GETCARETWIDTH,0,&caretWidth,0);
		DWORD halfCaret=caretWidth/2u;
		float px,py;
		/*DWRITE_TEXT_METRICS txtmetrics;
		textlayout->GetMetrics(&txtmetrics);
		DWRITE_HIT_TEST_METRICS htmetrics;
		textlayout->HitTestPoint(x,y,&trail,&inside,&htmetrics);
		measureCaret(htmetrics.textPosition);*/
	}
	void GLTextBox::measureCaret(int pos){
		DWORD caretWidth=1;
		SystemParametersInfo(SPI_GETCARETWIDTH,0,&caretWidth,0);
		DWORD halfCaret=caretWidth/2u;
		float px,py;
		/*DWRITE_TEXT_METRICS txtmetrics;
		textlayout->GetMetrics(&txtmetrics);
		DWRITE_HIT_TEST_METRICS htmetrics;
		textlayout->HitTestTextPosition(pos,trail,&px,&py,&htmetrics);
		caretRect.left=getBounds().left+getLeftPadding()+px-halfCaret;
		caretRect.top=getBounds().top+getTopPadding()+htmetrics.top;
		caretRect.right=getBounds().left+getLeftPadding()+px+(caretWidth-halfCaret);
		caretRect.bottom=caretRect.top+htmetrics.height;*/
	}
	void GLTextBox::recreateResources(std::shared_ptr<GLHandle>& gl){
		bmp_data=NULL;
		bmp_focused_data=NULL;
		bmp_disabled_data=NULL;
		int width,height,nChannels;
		UINT bufferSize=0;
		GLfloat TexCoord[]={
			0,0,
			1,0,
			1,1,
			0,1
		};
		memcpy(this->TexCoord,TexCoord,sizeof(TexCoord));
		GLubyte Indices[]={
			0,1,2,
			2,3,0
		};
		memcpy(this->Indices,Indices,sizeof(Indices));
		GLfloat Vertices[]={
			PixelToGLPoint(getBounds().left,1280),PixelToGLPoint(getBounds().top,720),0.f,
			PixelToGLPoint(getBounds().right,1280),PixelToGLPoint(getBounds().top,720),0.f,
			PixelToGLPoint(getBounds().right,1280),PixelToGLPoint(getBounds().bottom,720),0.f,
			PixelToGLPoint(getBounds().left,1280),PixelToGLPoint(getBounds().bottom,720),0.f
		};
		/*GLfloat Vertices[]={
			getBounds().left,getBounds().top,0.f,
			getBounds().right,getBounds().top,0.f,
			getBounds().right,getBounds().bottom,0.f,
			getBounds().left,getBounds().bottom,0.f
		};*/
		memcpy(this->Vertices,Vertices,sizeof(Vertices));
		int w=getBounds().right-getBounds().left;
		int h=getBounds().bottom-getBounds().top;
		bmp_focused_data=stbi_load("Images/UI/textbox_focus.png",&width,&height,&channels_bmp_focused_data,0);
		unsigned char* resizedFocus=(unsigned char*)malloc(w*h*channels_bmp_focused_data);
		stbir_resize_uint8(bmp_focused_data,width,height,0,resizedFocus,getBounds().right-getBounds().left,getBounds().bottom-getBounds().top,0,channels_bmp_focused_data);
		bmp_focused_data=resizedFocus;
		bmp_data=stbi_load("Images/UI/textbox.png",&width,&height,&channels_bmp_data,0);
		unsigned char* resizedEnabled=(unsigned char*)malloc(w*h*channels_bmp_data);
		stbir_resize_uint8(bmp_data,width,height,0,resizedEnabled,getBounds().right-getBounds().left,getBounds().bottom-getBounds().top,0,channels_bmp_data);
		bmp_data=resizedEnabled;
		bmp_disabled_data=stbi_load("Images/UI/textbox_disabled.png",&width,&height,&channels_bmp_disabled_data,0);
		unsigned char* resizedDisabled=(unsigned char*)malloc(w*h*channels_bmp_disabled_data);
		stbir_resize_uint8(bmp_disabled_data,width,height,0,resizedDisabled,getBounds().right-getBounds().left,getBounds().bottom-getBounds().top,0,channels_bmp_disabled_data);
		bmp_disabled_data=resizedDisabled;
		glfont=new FTGLPixmapFont("Fonts/courier.ttf");
		if(glfont->Error()){
			OutputDebugStringA(getErrorMessage(glfont->Error()));
			OutputDebugString(L"\n");
		}
		glfont->FaceSize((int)(textsize*(96.f/72.f)));
		textlayout=new FTSimpleLayout();
		textlayout->SetLineLength(getBounds().right-getBounds().left);
		textlayout->SetFont(glfont);
		textlayout->SetAlignment(FTGL::ALIGN_LEFT);
		textlayout->SetLineSpacing(0.6f);
		textlayout->SetWrapTextOption(FTGL::WRAP_ONLYNEWLINE);
	}
	void GLTextBox::draw(std::shared_ptr<GLHandle>& gl){
		textlayout->ClearTextColorRanges();
		for(int i=0;i<rangesColor.size();i++){
			textlayout->AddTextColorRange(rangesColor.at(i));
		}
		GLuint texture=0;
		UINT bufferSize=0;
		glEnable(GL_TEXTURE_2D);
		glPushMatrix();
		glGenTextures(1,&texture);
		glBindTexture(GL_TEXTURE_2D,texture);
		glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE); // Should be glTexEnvi instead of glTexEnvf
		glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_S,GL_MIRRORED_REPEAT);
		glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_T,GL_MIRRORED_REPEAT);
		glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR_MIPMAP_LINEAR);
		glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR_MIPMAP_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_GENERATE_MIPMAP, GL_TRUE);
		if(isEnabled()){
			if(hasFocus()){
				glTexImage2D(GL_TEXTURE_2D,0,(channels_bmp_focused_data==4)?GL_RGBA:GL_RGB,getBounds().right-getBounds().left,getBounds().bottom-getBounds().top,0,(channels_bmp_focused_data==4)?GL_RGBA:GL_RGB,GL_UNSIGNED_BYTE,bmp_focused_data);
			}
			else{
				glTexImage2D(GL_TEXTURE_2D,0,(channels_bmp_data==4)?GL_RGBA:GL_RGB,getBounds().right-getBounds().left,getBounds().bottom-getBounds().top,0,(channels_bmp_data==4)?GL_RGBA:GL_RGB,GL_UNSIGNED_BYTE,bmp_data);
			}
		}
		else{
			glTexImage2D(GL_TEXTURE_2D,0,(channels_bmp_disabled_data==4)?GL_RGBA:GL_RGB,getBounds().right-getBounds().left,getBounds().bottom-getBounds().top,0,(channels_bmp_disabled_data)?GL_RGBA:GL_RGB,GL_UNSIGNED_BYTE,bmp_disabled_data);
		}
		// Now we are done with image loading
		// Before we draw something we call
		glBindTexture(GL_TEXTURE_2D, texture);
		// and then we perform our draws, this code draws a quad
		glBegin(GL_QUADS);
		glNormal3f(0.0, 0.0, 1.0);
		glTexCoord2f(0.0, 0.0);
		glVertex3f(Vertices[0],Vertices[1],Vertices[2]);
		glTexCoord2f(1.0, 0.0);
		glVertex3f(Vertices[3],Vertices[4],Vertices[5]);
		glTexCoord2f(1.0, 1.0);
		glVertex3f(Vertices[6],Vertices[7],Vertices[8]);
		glTexCoord2f(0.0, 1.0);
		glVertex3f(Vertices[9],Vertices[10],Vertices[11]);
		glEnd();
		glDisable(GL_TEXTURE_2D);
		glPopMatrix();
		glPushMatrix();
		glEnable(GL_SCISSOR_TEST);
		float widthText=(getBounds().right-getRightPadding())-(getBounds().left+getLeftPadding());
		float heightText=(getBounds().bottom-getBottomPadding())-(getBounds().top+getTopPadding());
		glScissor(getBounds().left,getBounds().bottom,widthText,heightText);
		glPushAttrib(GL_ALL_ATTRIB_BITS);
		glPixelTransferf(GL_RED_BIAS, fRGBA[0] - 1);
		glPixelTransferf(GL_GREEN_BIAS, fRGBA[1] - 1);
		glPixelTransferf(GL_BLUE_BIAS, fRGBA[2] - 1);
		glPixelTransferf(GL_ALPHA_BIAS,(fRGBA[3]*opacity)-1);
		textlayout->Render(getText().c_str(),getText().length(),FTPoint(0,687-getBounds().top,0),FTGL::RENDER_ALL);
		glPopAttrib();
		glDisable(GL_SCISSOR_TEST);
		glPopMatrix();
	}
};