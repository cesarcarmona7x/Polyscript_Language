#include "stdafx.h"
PolyscriptColor::PolyscriptColor(float r,float g,float b,float a){
	this->name=L"PolyscriptColor";
	this->r=r;
	this->g=g;
	this->b=b;
	this->a=a;
}
PolyscriptBrush::PolyscriptBrush(PolyscriptColor color){
	this->name=L"PolyscriptBrush";
	this->color=&color;
}