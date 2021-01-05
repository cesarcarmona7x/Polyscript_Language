#include "stdafx.h"
bool GLHandle::InitializeGL(HWND hwnd,GameSettings settings){
	HDC hdc;
	hdc=GetWindowDC(hwnd);
	PIXELFORMATDESCRIPTOR pfd;
	memset(&pfd,0,sizeof(PIXELFORMATDESCRIPTOR));
	pfd.nSize=sizeof(PIXELFORMATDESCRIPTOR);
	pfd.nVersion=1;
	pfd.dwFlags=PFD_DOUBLEBUFFER|PFD_SUPPORT_OPENGL|PFD_DRAW_TO_WINDOW;
	pfd.iPixelType=PFD_TYPE_RGBA;
	pfd.cColorBits=32;
	pfd.cDepthBits=32;
	pfd.iLayerType=PFD_MAIN_PLANE;
	int nPixelFormat=ChoosePixelFormat(hdc,&pfd);
	if(nPixelFormat==0)return false;
	BOOL result=SetPixelFormat(hdc,nPixelFormat,&pfd);
	if(!result)return false;
	HGLRC glTemp=wglCreateContext(hdc);
	wglMakeCurrent(hdc,glTemp);
	GLenum err=glewInit();
	if(err!=GLEW_OK){
		MessageBox(NULL,L"No se pudo iniciar GLEW",L"Error GLEW",MB_OK);
		return false;
	}
	int attribs[]={
		WGL_CONTEXT_MAJOR_VERSION_ARB,3,
		WGL_CONTEXT_MINOR_VERSION_ARB,1,
		WGL_CONTEXT_FLAGS_ARB,0,
		0
	};
	if(wglewIsSupported("WGL_ARB_create_context")==1){
		renderContext=wglCreateContextAttribsARB(hdc,0,attribs);
		wglMakeCurrent(NULL,NULL);
		wglDeleteContext(glTemp);
		wglMakeCurrent(hdc,renderContext);
	}
	else{
		renderContext=glTemp;
	}
	if(!renderContext)return false;
	CoCreateInstance(CLSID_WICImagingFactory,NULL,CLSCTX_INPROC_SERVER,IID_IWICImagingFactory,(LPVOID*)imgfactory.GetAddressOf());
	return true;
}
GLHandle::~GLHandle(){
	glBindBuffer(GL_ARRAY_BUFFER,0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,0);
	//glDeleteBuffers();
	glBindVertexArray(0);
	//glDeleteVertexArrays();
	wglMakeCurrent(NULL,NULL);
	if(renderContext){
		wglDeleteContext(renderContext);
		renderContext=NULL;
	}
}