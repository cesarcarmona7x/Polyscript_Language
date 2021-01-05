#ifndef POLYSCRIPTVARIABLE_INCLUDED
#define POLYSCRIPTVARIABLE_INCLUDED
#include "stdafx.h"
enum PolyscriptFigureType{
	POLYSCRIPT_FIGURE_TYPE_SQUARE,
	POLYSCRIPT_FIGURE_TYPE_RECTANGLE,
	POLYSCRIPT_FIGURE_TYPE_ROUNDEDRECTANGLE,
	POLYSCRIPT_FIGURE_TYPE_TRIANGLE,
	POLYSCRIPT_FIGURE_TYPE_ELLIPSE,
	POLYSCRIPT_FIGURE_TYPE_LINE,
	POLYSCRIPT_FIGURE_TYPE_POLYGON
};
static std::map<std::wstring,PolyscriptFigureType> figureAssocs=boost::assign::map_list_of
	(L"Square",POLYSCRIPT_FIGURE_TYPE_SQUARE)
	(L"Rectangle",POLYSCRIPT_FIGURE_TYPE_RECTANGLE)
	(L"RoundedRectangle",POLYSCRIPT_FIGURE_TYPE_ROUNDEDRECTANGLE)
	(L"Triangle",POLYSCRIPT_FIGURE_TYPE_TRIANGLE)
	(L"Ellipse",POLYSCRIPT_FIGURE_TYPE_ELLIPSE)
	(L"Line",POLYSCRIPT_FIGURE_TYPE_LINE)
	(L"Polygon",POLYSCRIPT_FIGURE_TYPE_POLYGON);
enum PolyscriptDrawMode{
	POLYSCRIPT_DRAW_MODE_DRAW,
	POLYSCRIPT_DRAW_MODE_FILL,
	POLYSCRIPT_DRAW_MODE_BOTH_DRAW_TOP,
	POLYSCRIPT_DRAW_MODE_BOTH_FILL_TOP
};
static std::map<std::wstring,PolyscriptDrawMode> drawModeAssocs=boost::assign::map_list_of
	(L"Draw",POLYSCRIPT_DRAW_MODE_DRAW)
	(L"Fill",POLYSCRIPT_DRAW_MODE_FILL)
	(L"BothDrawTop",POLYSCRIPT_DRAW_MODE_BOTH_DRAW_TOP)
	(L"BothFillTop",POLYSCRIPT_DRAW_MODE_BOTH_FILL_TOP);
struct PolyscriptPoint{
	float x;
	float y;
};
struct PolyscriptGradientStop;
class PolyscriptVariable;
class PolyscriptColor;
class PolyscriptBrush;
class PolyscriptVariable{
public:
	bool operator== (PolyscriptVariable& other){
		return this==&other;
	}
	bool operator!= (PolyscriptVariable& other){
		return this!=&other;
	}
	PolyscriptVariable(){
		this->name=L"PolyscriptVariable";
	}
	~PolyscriptVariable(){}
	int getData(){return 0;}
	float* getDataColor(){
		float rgba[4]={r,g,b,a};
		return rgba;
	}
	PolyscriptColor* getDataBrush(){
		return color;
	}
	int getDataInt(){
		return int_value;
	}
	float getDataFloat(){
		return float_value;
	}
	std::wstring getDataBitmap(){
		return location;
	}
	std::vector<PolyscriptGradientStop> getDataGradientStop(){
		return gradientStops;
	}
	std::wstring name;
protected:
	float r, g, b, a;//For type PolyscriptColor
	PolyscriptColor* color;//For type PolyscriptBrush
	float float_value;//For type PolyscriptFloat
	int int_value;//For type PolyscriptInt
	std::wstring location;//For type PolyscriptBitmap
	std::vector<PolyscriptGradientStop> gradientStops;//For type PolyscriptGradient
};
class PolyscriptColor:public PolyscriptVariable{
public:
	PolyscriptColor(float r=1.f,float g=1.f,float b=1.f,float a=1.f);
	~PolyscriptColor(){}
};
struct PolyscriptGradientStop{
	PolyscriptColor color;
	float positionX;
	float positionY;
};
class PolyscriptBrush:public PolyscriptVariable{
public:
	PolyscriptBrush(PolyscriptColor color=PolyscriptColor());
	~PolyscriptBrush(){}
};
class PolyscriptFloat:public PolyscriptVariable{
public:
	PolyscriptFloat(float value=0.0f){
		this->name=L"PolyscriptFloat";
		this->float_value=value;
	}
	~PolyscriptFloat(){}
};
class PolyscriptInt:public PolyscriptVariable{
public:
	PolyscriptInt(int value=0){
		this->name=L"PolyscriptInt";
		this->int_value=int_value;
	}
	~PolyscriptInt(){}
};

class PolyscriptBitmap:public PolyscriptVariable{
public:
	PolyscriptBitmap(std::wstring location=L""){
		this->name=L"PolyscriptBitmap";
		this->location=location;
	}
};
class PolyscriptGradient:public PolyscriptVariable{
public:
	PolyscriptGradient(std::vector<PolyscriptGradientStop> gradientStops=std::vector<PolyscriptGradientStop>()){
		this->name=L"PolyscriptGradient";
		this->gradientStops=gradientStops;
	}
	~PolyscriptGradient(){}
};
typedef std::reference_wrapper<PolyscriptVariable> PolyscriptRef;
#endif