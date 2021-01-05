#ifndef OPENGLUI_INCLUDED
#define OPENGLUI_INCLUDED
#include "stdafx.h"
namespace OpenGLUI{
	typedef enum GL_WINDOW_STATES{
		NONE=0,
		ENABLED=1,
		EDITABLE=2,
		HOVERED=3,
		FOCUSED=4,
		PRESSED=5,
		CHECKED=6,
		SELECTED=7,
		VISIBLE=8,
		DROPDOWN_VISIBLE=9
	}GL_WINDOW_STATES;
	typedef enum GLHorizontalConstants{
		LEFT=0,
		CENTER_HORIZONTAL=1,
		RIGHT=2
	}GLHorizontalConstants;
	typedef enum GLVerticalConstants{
		TOP=3,
		CENTER_VERTICAL=4,
		BOTTOM=5
	}GLVerticalConstants;
	class OpenGLUIDrawableItem;
	class OpenGLUISystem;
	class GLLayoutBase;
	class GLWindowBase{
	public:
		GLWindowBase(){
		}
		~GLWindowBase(){}
		virtual void setOpacity(float opacity=1.0f){
			this->opacity=opacity;
		}
		virtual float getOpacity(){
			return opacity;
		}
		virtual void setLocale(wchar_t* locale){
			this->locale=locale;
		}
		virtual wchar_t* getLocale(){
			return locale;
		}
		virtual void setId(wchar_t* id){this->wndId=id;}
		virtual void setIndex(int index){this->index=index;}
		wchar_t* getId(){return wndId;}
		int getIndex(){return index;}
		static bool compareHeight(GLWindowBase& a,GLWindowBase& b){
			return a.height<b.height;
		}
		virtual void setBackground(int R,int G,int B,int A=255){
			iRGBA_bg[0]=R;
			iRGBA_bg[1]=G;
			iRGBA_bg[2]=B;
			iRGBA_bg[3]=A;
			fRGBA_bg[0]=R/255.0f;
			fRGBA_bg[1]=G/255.0f;
			fRGBA_bg[2]=B/255.0f;
			fRGBA_bg[3]=A/255.0f;
		}
		virtual int* getBackgroundInt(){
			return iRGBA_bg;
		}
		virtual void setBackground(float R,float G,float B,float A=1.0f){
			fRGBA_bg[0]=R;
			fRGBA_bg[1]=G;
			fRGBA_bg[2]=B;
			fRGBA_bg[3]=A;
			iRGBA_bg[0]=(int)(R*255.0f);
			iRGBA_bg[1]=(int)(G*255.0f);
			iRGBA_bg[2]=(int)(B*255.0f);
			iRGBA_bg[3]=(int)(A*255.0f);
		}
		virtual float* getBackgroundFloat(){
			return fRGBA_bg;
		}
		virtual void setSize(int width,int height){
			GLWindowBase::width=width;
			GLWindowBase::height=height;
		}
		virtual int getWidth(){
			return width;
		}
		virtual int getHeight(){
			return height;
		}
		virtual void setPosition(int row,int col){
			this->row=row;
			this->col=col;
		}
		virtual void setParentLayout(std::shared_ptr<GLLayoutBase> layout);
		virtual std::shared_ptr<GLLayoutBase> getParentLayout();
		//Sets the text for the window.
		virtual void setText(std::wstring text){
			this->text=text;
		}
		virtual std::wstring getText(){
			return text;
		}
		//Sets the font for the window.
		virtual void setFont(wchar_t* font){
			this->font=font;
		}
		//Retrieves the font being used by the window as a wchar_t* value.
		virtual wchar_t* getFont(){
			return font;
		}
		//Defines the coordinates of the window.
		virtual void setBounds(int left,int top,int right,int bottom){
			bounds.left=left;
			bounds.top=top;
			bounds.right=right;
			bounds.bottom=bottom;
			setSize(right-left,bottom-top);
		}
		//Retrieves the bounding rectangle for the window.
		virtual RECT getBounds(){
			return bounds;
		}
		//Sets an image file as an icon to the left of the button.
		virtual void setIcon(wchar_t* icon){
			this->icon=icon;
		}
		//Retrieves the file used as icon for the button, if any.
		virtual wchar_t* getIcon(){
			return icon;
		}
		//Sets the text's horizontal alignment.
		virtual void setHorizontalTextAlignment(GLHorizontalConstants h){
			hAlignment=h;
		}
		//Retrieves the horizontal text alignment as HorizontalConstants.
		virtual GLHorizontalConstants getHorizontalTextAlignment(){
			return hAlignment;
		}
		//Set the text's vertical alignment.
		virtual void setVerticalTextAlignment(GLVerticalConstants v){
			vAlignment=v;
		}
		//Retrieves the vertical text alignment as VerticalConstants.
		virtual GLVerticalConstants getVerticalTextAlignment(){
			return vAlignment;
		}
		//Defines the colour of the text. Values go from 0.0 to 1.0
		virtual void setForeground(float R,float G,float B,float A=1.0f){
			fRGBA[0]=R;
			fRGBA[1]=G;
			fRGBA[2]=B;
			fRGBA[3]=A;
			iRGBA[0]=(int)(R*255.0f);
			iRGBA[1]=(int)(G*255.0f);
			iRGBA[2]=(int)(B*255.0f);
			iRGBA[3]=(int)(A*255.0f);
		}
		//Defines the colour of the text. Values go from 0 to 255.
		virtual void setForeground(int R,int G,int B,int A=255){
			iRGBA[0]=R;
			iRGBA[1]=G;
			iRGBA[2]=B;
			iRGBA[3]=A;
			fRGBA[0]=R/255.0f;
			fRGBA[1]=G/255.0f;
			fRGBA[2]=B/255.0f;
			fRGBA[3]=A/255.0f;
		}
		//Retrieves the foreground's RGBA values, as an int array.
		virtual int* getForegroundInt(){
			return iRGBA;
		}
		//Retrieves the foreground's RGBA values, as a float array.
		virtual float* getForegroundFloat(){
			return fRGBA;
		}
		//Sets the size of the text, in floating point pixels
		virtual void setTextSize(float px){
			this->textsize=px;
		}
		//Retrieves the text size, as a float.
		virtual float getTextSize(){
			return textsize;
		}
		/* Sets the margin for the window to write text.
		If the window has an icon, it will be placed according the padding,
		and the text will be placed after the right margin of the button.*/
		virtual void setPadding(int left,int top,int right=-1,int bottom=-1){
			leftPadding=left;
			topPadding=top;
			rightPadding=right;
			bottomPadding=bottom;
			if(rightPadding==-1){
				rightPadding=leftPadding;
			}
			if(bottomPadding==-1){
				bottomPadding=topPadding;
			}
		}
		//Enables or disables a window.
		virtual void setEnabled(bool enabled){
			bool foundinstates=false;
			if(enabled){
				for(int i=0;i<states.size();i++){
					if(states.at(i)==ENABLED){
						foundinstates=true;
						break;
					}
				}
				if(!foundinstates){
					states.push_back(ENABLED);
				}
			}
			else{
				for(int i=0;i<states.size();i++){
					if(states.at(i)==ENABLED){
						states.erase(states.begin()+i);
						break;
					}
				}
			}
		}
		//Retrieves whether the window is enabled or not.
		virtual bool isEnabled(){
			bool enabled=false;
			for(int i=0;i<states.size();i++){
				if(states.at(i)==ENABLED){
					enabled=true;
					break;
				}
			}
			return enabled;
		}
		//Displays mouse-over effects.
		virtual void setHovered(bool hovered){
			bool foundinstates=false;
			if(hovered){
				for(int i=0;i<states.size();i++){
					if(states.at(i)==HOVERED){
						foundinstates=true;
						break;
					}
				}
				if(!foundinstates){
					states.push_back(HOVERED);
				}
			}
			else{
				for(int i=0;i<states.size();i++){
					if(states.at(i)==HOVERED){
						states.erase(states.begin()+i);
						break;
					}
				}
			}
		}
		//Retrieves whether the mouse is over the window or not.
		virtual bool isHovered(){
			bool hovered=false;
			for(int i=0;i<states.size();i++){
				if(states.at(i)==HOVERED){
					hovered=true;
					break;
				}
			}
			return hovered;
		}
		//Displays mouse click effects.
		virtual void setPressed(bool pressed){
			bool foundinstates=false;
			if(pressed){
				for(int i=0;i<states.size();i++){
					if(states.at(i)==PRESSED){
						foundinstates=true;
						break;
					}
				}
				if(!foundinstates){
					states.push_back(PRESSED);
				}
			}
			else{
				for(int i=0;i<states.size();i++){
					if(states.at(i)==PRESSED){
						states.erase(states.begin()+i);
						break;
					}
				}
			}
		}
		//Retrieves whether the window is being pressed or not.
		virtual bool isPressed(){
			bool pressed=false;
			for(int i=0;i<states.size();i++){
				if(states.at(i)==PRESSED){
					pressed=true;
					break;
				}
			}
			return pressed;
		}
		//Selects the current button from the group.
		virtual void setSelected(bool selected){}
		//Retrieves if the button is the one being selected.
		virtual bool isSelected(){
			bool selected=false;
			for(int i=0;i<states.size();i++){
				if(states.at(i)==SELECTED){
					selected=true;
					break;
				}
			}
			return selected;
		}
		//Allows you (or not) to write over a TextBox window.
		virtual void setEditable(bool editable){
			bool foundinstates=false;
			if(editable){
				for(int i=0;i<states.size();i++){
					if(states.at(i)==EDITABLE){
						foundinstates=true;
						break;
					}
				}
				if(!foundinstates){
					states.push_back(EDITABLE);
				}
			}
			else{
				for(int i=0;i<states.size();i++){
					if(states.at(i)==EDITABLE){
						states.erase(states.begin()+i);
						break;
					}
				}
			}
		}
		//Retrieves whether the TextBox's text can be manipulated.
		virtual bool isEditable(){
			bool editable=false;
			for(int i=0;i<states.size();i++){
				if(states.at(i)==EDITABLE){
					editable=true;
					break;
				}
			}
			return editable;
		}
		//Enables a TextBox to write over it.
		virtual void setFocus(bool focus){
			bool foundinstates=false;
			if(focus){
				for(int i=0;i<states.size();i++){
					if(states.at(i)==FOCUSED){
						foundinstates=true;
						break;
					}
				}
				if(!foundinstates){
					states.push_back(FOCUSED);
				}
			}
			else{
				for(int i=0;i<states.size();i++){
					if(states.at(i)==FOCUSED){
						states.erase(states.begin()+i);
						break;
					}
				}
			}
		}
		//Retrieves whether the TextBox is selected for writing.
		virtual bool hasFocus(){
			bool focused=false;
			for(int i=0;i<states.size();i++){
				if(states.at(i)==FOCUSED){
					focused=true;
					break;
				}
			}
			return focused;
		}
		//Retrieves the left margin of the window, as an int.
		virtual int getLeftPadding(){
			return leftPadding;
		}
		//Retrieves the top margin of the window, as an int.
		virtual int getTopPadding(){
			return topPadding;
		}
		//Retrieves the right margin of the window, as an int.
		virtual int getRightPadding(){
			return rightPadding;
		}
		//Retrieves the bottom margin of the window, as an int.
		virtual int getBottomPadding(){
			return bottomPadding;
		}
		//Displays or hides the window.
		virtual void setVisible(bool visible){
			bool foundinstates=false;
			if(visible){
				for(int i=0;i<states.size();i++){
					if(states.at(i)==VISIBLE){
						foundinstates=true;
						break;
					}
				}
				if(!foundinstates){
					states.push_back(VISIBLE);
				}
			}
			else{
				for(int i=0;i<states.size();i++){
					if(states.at(i)==VISIBLE){
						states.erase(states.begin()+i);
						break;
					}
				}
			}
		}
		//Retrieves whether the window is showing or not.
		virtual bool isVisible(){
			bool visible=false;
			for(int i=0;i<states.size();i++){
				if(states.at(i)==VISIBLE){
					visible=true;
					break;
				}
			}
			return visible;
		}
		//Checks or unchecks a CheckBox window.
		virtual void setChecked(bool checked){
			bool foundinstates=false;
			if(checked){
				for(int i=0;i<states.size();i++){
					if(states.at(i)==CHECKED){
						foundinstates=true;
						break;
					}
				}
				if(!foundinstates){
					states.push_back(CHECKED);
				}
			}
			else{
				for(int i=0;i<states.size();i++){
					if(states.at(i)==CHECKED){
						states.erase(states.begin()+i);
						break;
					}
				}
			}
		}
		//Retrieves whether a CheckBox is checked or not.
		virtual bool isChecked(){
			bool checked=false;
			for(int i=0;i<states.size();i++){
				if(states.at(i)==CHECKED){
					checked=true;
					break;
				}
			}
			return checked;
		}
		//Draws the contents of the window over a GLEW render context
		virtual void draw(std::shared_ptr<GLHandle>& gl)=0;
		bool operator== (GLWindowBase& other){
			if(this->wndId==NULL||other.wndId==NULL){
				return false;
			}
			else{
				return (wcscmp(wndId,other.wndId)==0);
			}
		}
		int row,col;
		int height;
		int width;
		virtual void recreateResources(std::shared_ptr<GLHandle>& gl)=0;
		std::shared_ptr<GLLayoutBase> parent;
		OpenGLUIDrawableItem* drawing;
		FTGLPixmapFont* glfont;
		FTSimpleLayout* textlayout;
		std::vector<FTGL::FontColorRange>rangesColor;
	protected:
		std::vector<OpenGLUI::GL_WINDOW_STATES>states;
		wchar_t* wndId;
		wchar_t* locale;
		int index;
		float opacity;
		float fRGBA_bg[4];
		int iRGBA_bg[4];
		wchar_t* font;
		float textsize;
		int iRGBA[4];
		float fRGBA[4];
		RECT bounds;
		int leftPadding;
		int topPadding;
		int rightPadding;
		int bottomPadding;
		GLfloat TexCoord[8];
		GLfloat Vertices[12];
		BYTE* bmp_data;
		int channels_bmp_data;
		BYTE* bmp_focused_data;
		int channels_bmp_focused_data;
		BYTE* bmp_pressed_data;
		int channels_bmp_pressed_data;
		BYTE* bmp_disabled_data;
		int channels_bmp_disabled_data;
		BYTE* bmp_hovered_data;
		int channels_bmp_hovered_data;
		BYTE* bmp_checked_data;
		int channels_bmp_checked_data;
		BYTE* bmp_hovered_checked_data;
		int channels_bmp_hovered_checked_data;
		BYTE* bmp_pressed_checked_data;
		int channels_bmp_pressed_checked_data;
		BYTE* bmp_disabled_checked_data;
		int channels_bmp_disabled_checked_data;
		BYTE* bmp_selected_data;
		int channels_bmp_selected_data;
		BYTE* bmp_hovered_selected_data;
		int channels_bmp_hovered_selected_data;
		BYTE* bmp_pressed_selected_data;
		int channels_bmp_pressed_selected_data;
		BYTE* bmp_disabled_selected_data;
		int channels_bmp_disabled_selected_data;
		GLShader textShader;
		GLubyte Indices[6];
		std::map<wchar_t,Character>glyphs;
		GLVerticalConstants vAlignment;
		GLHorizontalConstants hAlignment;
		std::wstring text;
		wchar_t* icon;
	};
	class GLBorder{
	public:
		GLBorder(){
			setStroke(NONE);
			setColor(0.f,0.f,0.f,1.f);
		}
		~GLBorder(){}
		enum Stroke{
			NONE=0,
			SOLID=1,
			DASHED=2,
			INSET=3,
			OUTSET=4,
			DOTTED=5
		};
		void setColor(int R,int G,int B,int A=0);
		void setColor(float R,float G,float B,float A=0.0f);
		float* getColorFloat();
		int* getColorInt();
		float fRGBA[4];
		int iRGBA[4];
		Stroke stroke;
		float strokeWidth;
		void setStroke(Stroke s);
		Stroke getStroke();
		void setStrokeWidth(float px);
		float getStrokeWidth();
		void draw(std::shared_ptr<GLHandle>& gl);
	};
	class GLLayoutBase{
	public:
		GLLayoutBase(){}
		~GLLayoutBase(){}
		enum Orientation{
			VERTICAL=0,
			HORIZONTAL=1
		};
		enum VGravity{
			TOP=1,
			CENTERV=2,
			BOTTOM=3
		};
		enum HGravity{
			LEFT=1,
			CENTERH=2,
			RIGHT=3
		};
		virtual void setPosition(int row,int col){
			this->row=row;
			this->col=col;
		}
		virtual void setId(wchar_t* id){this->layId=id;}
		virtual void setIndex(int index){this->index=index;}
		virtual wchar_t* getId(){return layId;}
		virtual int getIndex(){return index;}
		virtual void reorderComponents()=0;
		virtual void setSize(int width,int height){
			this->width=width;
			this->height=height;
		}
		virtual int getWidth(){
			return width;
		}
		virtual int getHeight(){
			return height;
		}
		virtual void setVisible(bool visible);
		virtual bool isVisible();
		virtual void add(GLWindowBase& window)=0;
		virtual void add(GLWindowBase& window,int row,int col)=0;
		virtual void add(GLLayoutBase& layout)=0;
		virtual void add(GLLayoutBase& layout,int row,int col)=0;
		virtual void remove(wchar_t* id)=0;
		virtual void remove(int row,int col)=0;
		virtual void setRadius(float x,float y=-1){
			this->radius_x=x;
			this->radius_y=y;
			if(radius_y==-1){
				radius_y=radius_x;
			}
		}
		virtual float getXRadius(){
			return radius_x;
		}
		virtual float getYRadius(){
			return radius_y;
		}
		virtual void setBounds(int left,int top,int right,int bottom){
			bounds.left=left;
			bounds.top=top;
			bounds.right=right;
			bounds.bottom=bottom;
			setSize(right-left,bottom-top);
		}
		virtual void setOrientation(Orientation o){
			this->o=o;
		}
		virtual Orientation getOrientation(){
			return o;
		}
		virtual void setBackground(int R=0,int G=0,int B=0,int A=0){
			iRGBA[0]=R;
			iRGBA[1]=G;
			iRGBA[2]=B;
			iRGBA[3]=A;
			fRGBA[0]=R/255.0f;
			fRGBA[1]=G/255.0f;
			fRGBA[2]=B/255.0f;
			fRGBA[3]=A/255.0f;
		}
		virtual void setBackground(float R=0.f,float G=0.f,float B=0.f,float A=0.f){
			fRGBA[0]=R;
			fRGBA[1]=G;
			fRGBA[2]=B;
			fRGBA[3]=A;
			iRGBA[0]=(int)(R*255.0f);
			iRGBA[1]=(int)(G*255.0f);
			iRGBA[2]=(int)(B*255.0f);
			iRGBA[3]=(int)(A*255.0f);
		}
		virtual RECT getBounds(){
			return bounds;
		}
		virtual void setViewportBounds(int left,int top,int right,int bottom){
			viewportbounds.left=left;
			viewportbounds.top=top;
			viewportbounds.right=right;
			viewportbounds.bottom=bottom;
		}
		virtual RECT getViewportBounds(){
			return viewportbounds;
		}
		virtual void setScrollbarVisible(bool visible)=0;
		virtual void setPadding(int left,int top,int right=-1,int bottom=-1){
			leftPadding=left;
			topPadding=top;
			rightPadding=right;
			bottomPadding=bottom;
			if(rightPadding==-1){
				rightPadding=leftPadding;
			}
			if(bottomPadding==-1){
				bottomPadding=topPadding;
			}
		}
		virtual void setSpacing(int spacing){
			this->spacing=spacing;
		}
		virtual int getSpacing(){
			return spacing;
		}
		virtual int getLeftPadding(){
			return leftPadding;
		}
		virtual int getTopPadding(){
			return topPadding;
		}
		virtual int getRightPadding(){
			return rightPadding;
		}
		virtual int getBottomPadding(){
			return bottomPadding;
		}
		virtual void setBorder(GLBorder border){
			leftBorder=border;
			topBorder=border;
			rightBorder=border;
			bottomBorder=border;
		}
		virtual void setLeftBorder(GLBorder border){
			leftBorder=border;
		}
		virtual void setTopBorder(GLBorder border){
			topBorder=border;
		}
		virtual void setRightBorder(GLBorder border){
			rightBorder=border;
		}
		virtual void setBottomBorder(GLBorder border){
			bottomBorder=border;
		}
		virtual void setGravity(HGravity h,VGravity v){
			this->hg=h;
			this->vg=v;
		}
		virtual HGravity getHorizontalGravity(){
			return hg;
		}
		virtual VGravity getVerticalGravity(){
			return vg;
		}
		virtual GLBorder getLeftBorder(){
			return leftBorder;
		}
		virtual GLBorder getTopBorder(){
			return topBorder;
		}
		virtual GLBorder getRightBorder(){
			return rightBorder;
		}
		virtual GLBorder getBottomBorder(){
			return bottomBorder;
		}
		bool operator== (GLLayoutBase& other){
			if(this->layId==NULL||other.layId==NULL){
				return false;
			}
			else{
				return wcscmp(this->layId,other.layId)==0;
			}
		}
		virtual void setParentLayout(std::shared_ptr<GLLayoutBase> parent);
		virtual std::shared_ptr<GLLayoutBase> getParentLayout();
		virtual void draw(std::shared_ptr<GLHandle>& gl)=0;
		int row;
		int col;
		int width;
		int height;
		std::vector<std::reference_wrapper<GLWindowBase>>windows;
		std::vector<std::reference_wrapper<GLLayoutBase>>layouts;
		virtual void recreateResources(std::shared_ptr<GLHandle>& gl)=0;
		OpenGLUIDrawableItem* drawing;
	protected:
		std::shared_ptr<GLLayoutBase> parent;
		RECT bounds,viewportbounds;
		HGravity hg;
		VGravity vg;
		wchar_t* layId;
		int index;
		int iRGBA[4];
		float fRGBA[4];
		float radius_x;
		float radius_y;
		GLBorder leftBorder;
		GLBorder topBorder;
		GLBorder rightBorder;
		GLBorder bottomBorder;
		Orientation o;
		int leftPadding;
		int topPadding;
		int rightPadding;
		int bottomPadding;
		int spacing;
	};
	class OpenGLUIDrawableItem{
	public:
		OpenGLUIDrawableItem(){}
		OpenGLUIDrawableItem(GLWindowBase& parentWindow,bool visible){
			drawWindow.push_back(parentWindow);
			this->visible=visible;
		}
		OpenGLUIDrawableItem(GLLayoutBase& parentLayout,bool visible){
			drawLayout.push_back(parentLayout);
			this->visible=visible;
		}
		~OpenGLUIDrawableItem(){}
		std::vector<std::reference_wrapper<GLWindowBase>> drawWindow;
		std::vector<std::reference_wrapper<GLLayoutBase>> drawLayout;
		void setVisible(bool visible){this->visible=visible;}
		bool isVisible(){return visible;}
	private:
		bool visible;
	};
	class OpenGLUISystem{
	public:
		/*
		Layer description (objects in the same layer will be drawn by order of addition):
		1. Used for drawing normal objects.
		2. Used for drawing dropdowns.
		3. Used for drawing modal objects (MsgBox, InfoBox, PopupNotification, etc.) and its children.
		4. Used for drawing dropdowns inside modal objects (see above).
		*/
		void addDrawToQueue(int layer,OpenGLUIDrawableItem& draw);
		//Call this in your loop update. Once adding at least one item to the drawing system, you won't need to manually add the draw, customDraw or drawDropdown for each element.
		void draw(std::shared_ptr<GLHandle>& d2d);
		std::map<int,std::vector<std::reference_wrapper<OpenGLUIDrawableItem>>>drawOrder;
		static OpenGLUISystem& getInstance();
	private:
		OpenGLUISystem(){
		}
		~OpenGLUISystem(){}
	};
	class GLTextBox:public GLWindowBase{
	public:
		GLTextBox(std::wstring text=L"");
		~GLTextBox(){}
		int caretPos;
		BOOL trail,inside;
		RECT caretRect;
		virtual void setVisible(bool visible){
			GLWindowBase::setVisible(visible);
			drawing->setVisible(visible);
		}
		virtual void measureCaret(int x,int y);
		virtual void measureCaret(int pos);
		virtual void recreateResources(std::shared_ptr<GLHandle>& gl);
		virtual void draw(std::shared_ptr<GLHandle>& gl);
	private:
		virtual void setChecked(bool checked){}
		virtual bool isChecked(){return false;}
		virtual void setIcon(wchar_t* icon){}
		virtual wchar_t* getIcon(){return L"";}
		virtual void setPressed(bool pressed){}
		virtual bool isPressed(){return false;}
		virtual void setHovered(bool hovered){}
		virtual bool isHovered(){return false;}
		virtual void setSelected(bool selected){}
		virtual bool isSelected(){return false;}
	};
};
#endif