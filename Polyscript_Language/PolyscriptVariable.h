#ifndef POLYSCRIPTVARIABLE_INCLUDED
#define POLYSCRIPTVARIABLE_INCLUDED
#include "stdafx.h"
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
	std::wstring name;
protected:
	float r, g, b, a;//For type PolyscriptColor
	PolyscriptColor* color;//For type PolyscriptBrush
	float float_value;//For type PolyscriptFloat
	int int_value;//For type PolyscriptInt
	std::wstring location;//For type PolyscriptBitmap
};
class PolyscriptColor:public PolyscriptVariable{
public:
	PolyscriptColor(float r=1.f,float g=1.f,float b=1.f,float a=1.f);
	~PolyscriptColor(){}
private:

};
class PolyscriptBrush:public PolyscriptVariable{
public:
	PolyscriptBrush(PolyscriptColor color=PolyscriptColor());
private:
};
class PolyscriptFloat:public PolyscriptVariable{
public:
	PolyscriptFloat(float value=0.0f){
		this->name=L"PolyscriptFloat";
		this->float_value=value;
	}
	~PolyscriptFloat(){}
private:
};
class PolyscriptInt:public PolyscriptVariable{
public:
	PolyscriptInt(int value=0){
		this->name=L"PolyscriptInt";
		this->int_value=int_value;
	}
	~PolyscriptInt(){}
private:
};

class PolyscriptBitmap:public PolyscriptVariable{
public:
	PolyscriptBitmap(std::wstring location=L""){
		this->name=L"PolyscriptBitmap";
		this->location=location;
	}

private:
};
class PolyscriptGradient:public PolyscriptVariable{
public:

};
typedef std::reference_wrapper<PolyscriptVariable> PolyscriptRef;
typedef std::shared_ptr<PolyscriptVariable> PolyscriptPtr;
#endif