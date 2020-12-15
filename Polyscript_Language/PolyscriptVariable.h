#ifndef POLYSCRIPTVARIABLE_INCLUDED
#define POLYSCRIPTVARIABLE_INCLUDED
#include "stdafx.h"
struct PolyscriptGradientStop;
class PolyscriptVariable;
class PolyscriptColor;
class PolyscriptBrush;
class PolyscriptVariable{
public:
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