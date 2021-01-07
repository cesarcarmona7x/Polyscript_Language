#ifndef FIGURE_INCLUDED
#define FIGURE_INCLUDED
#include "stdafx.h"
struct PolyscriptFigureDesc{
	PolyscriptFigureDesc(PolyscriptFigureType type=POLYSCRIPT_FIGURE_TYPE_SQUARE,PolyscriptDrawMode drawMode=POLYSCRIPT_DRAW_MODE_FILL,float radiusX=0.f,float radiusY=0.f,float left=0.f,float top=0.f,float right=0.f,float bottom=0.f,float x=0.f,float y=0.f,float width=0.f,float height=0.f,PolyscriptColor color=PolyscriptColor(),PolyscriptBrush brush=PolyscriptBrush(),PolyscriptBitmap bitmap=PolyscriptBitmap(),PolyscriptGradient gradient=PolyscriptGradient(),float opacity=0.f,std::vector<PolyscriptPoint>points=std::vector<PolyscriptPoint>()){
		this->type=type;
		this->drawMode=drawMode;
		this->radiusX=radiusX;
		this->radiusY=radiusY;
		this->left=left;
		this->top=top;
		this->right=right;
		this->bottom=bottom;
		this->x=x;
		this->y=y;
		this->width=width;
		this->height=height;
		this->color=color;
		this->brush=brush;
		this->bitmap=bitmap;
		this->gradient=gradient;
		this->opacity=opacity;
		this->points=points;
	}
	PolyscriptFigureType type;
	PolyscriptDrawMode drawMode;
	float radiusX;
	float radiusY;
	float left;
	float top;
	float right;
	float bottom;
	float x;
	float y;
	float width;
	float height;
	PolyscriptColor color;
	PolyscriptBrush brush;
	PolyscriptBitmap bitmap;
	PolyscriptGradient gradient;
	float opacity;
	std::vector<PolyscriptPoint> points;
};
class PolyscriptFigure{
public:
	PolyscriptFigure(PolyscriptFigureDesc desc=PolyscriptFigureDesc());
	~PolyscriptFigure(){}
	void recreateResources(std::shared_ptr<D2DHandle>& d2d);
	void recreateResources(std::shared_ptr<GLHandle>& gl);
	void draw(std::shared_ptr<D2DHandle>& d2d);
	void draw(std::shared_ptr<GLHandle>& gl);
private:
	PolyscriptFigureDesc desc;
	ComPtr<ID2D1PathGeometry> polygon;
	ComPtr<ID2D1GeometrySink>polygonsink;
	ComPtr<ID2D1SolidColorBrush> solidbrush1,solidbrush2;//En caso de BothDrawTop o BothFillTop, el primer valor se usará abajo y el segundo arriba
	ComPtr<ID2D1LinearGradientBrush> gradientbrush1,gradientbrush2;//En caso de BothDrawTop o BothFillTop, el primer valor se usará abajo y el segundo arriba
	ComPtr<ID2D1BitmapBrush> bmpbrush1,bmpbrush2;//En caso de BothDrawTop o BothFillTop, el primer valor se usará abajo y el segundo arriba
	GLubyte* Indices;
	GLfloat TexCoord[8];
	std::vector<GLfloat>Vertices;
};
#endif