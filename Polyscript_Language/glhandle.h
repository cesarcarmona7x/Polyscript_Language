#ifndef GLHANDLE_INCLUDED
#define GLHANDLE_INCLUDED
#include "stdafx.h"
class GLHandle{
public:
	GLHandle(){}
	~GLHandle();
	bool InitializeGL(HWND hwnd,GameSettings settings);
	HGLRC renderContext;
	ComPtr<IWICImagingFactory> imgfactory;
	ComPtr<IWICBitmap> wicbitmap;
	ComPtr<IWICBitmapFrameDecode> bmpframedecoder;
	ComPtr<IWICFormatConverter> formatconverter;
	ComPtr<IWICBitmapScaler> bmpscaler;
	ComPtr<IWICBitmapDecoder>bmpdecoder;
	ComPtr<IWICBitmapLock>bmplock;
};
#endif