#include "stdafx.h"
#define MAX_LOADSTRING 100
enum PolyscriptRenderType{
	POLYSCRIPT_RENDER_DIRECT2D=0,
	POLYSCRIPT_RENDER_OPENGL=1
};
bool rendererInitializedDX=false;
bool rendererInitializedGL=false;
bool textcolorthreadrunning=false;
PolyscriptRenderType renderType=POLYSCRIPT_RENDER_DIRECT2D;
wchar_t* curDir=new wchar_t[MAX_PATH];
PolyscriptParser parser;
GameSettings settings;
std::shared_ptr<D2DHandle> d2d=std::make_shared<D2DHandle>();
std::shared_ptr<GLHandle> gl=std::make_shared<GLHandle>();
TextRendererDX rendererDirectX;
TextRendererGL rendererOpenGL;
// Global Variables:
HINSTANCE hInst;								// current instance
wchar_t* szTitle=L"Polyscript";					// The title bar text
wchar_t* szWindowClass=L"MyWindowClass";			// the main window class name
bool needsRecreation=true;
HWND hwnd;
HWND leftPanel;
HWND rightPanel;
WNDPROC editProc;
ComPtr<ID2D1SolidColorBrush> reservedBrush;
ComPtr<ID2D1SolidColorBrush> paramBrush;
ComPtr<ID2D1SolidColorBrush> drawModeBrush;
ComPtr<ID2D1SolidColorBrush> typeBrush;
ComPtr<ID2D1SolidColorBrush> variableBrush;
float reservedColor[4]={0.f,0.f,1.f,1.f};
float paramColor[4]={128.f/255.f,128.f/255.f,128.f/255.f,1.f};
float drawModeColor[4]={111.f/255.f,0.f,138.f/255.f,1.f};
float typeColor[4]={0.f,128.f/255.f,0.f,1.f};
float variableColor[4]={43.f/255.f,145.f/255.f,171.f/255.f,1.f};
std::thread textcolorthread;
void textColorThread(){
	using namespace boost::xpressive;
	while(textcolorthreadrunning){
		if(renderType==POLYSCRIPT_RENDER_DIRECT2D&&needsRecreation){
			rendererDirectX.textBox->rangesFontStyle.clear();
			rendererDirectX.textBox->rangesColor.clear();
			if(rendererDirectX.textBox->getText().length()!=0){
				const std::wstring text=rendererDirectX.textBox->getText();
				wsmatch matcher;
				wsregex variabledeclaration=wsregex::compile(L"^([A-Za-z0-9_ñ]+)[\\s|\\*|&]?[\\=].*");
				std::wstring line;
				std::wistringstream stream(text);
				std::vector<std::wstring>variableNames;
				std::vector<std::wstring>valueNames;
				while(std::getline(stream,line)){
					if(regex_match(line,matcher,variabledeclaration)){
						std::wstring variableName(matcher[1]);
						variableNames.push_back(variableName);
					}//Buscar posibles variables y sus valores
				}//Dividir el texto por líneas para facilitar el trabajo
				if(variableNames.size()!=0){
					for(int i=0;i<variableNames.size();i++){
						int pos1=0;
						int pos2=0;
						while((pos1=text.find(variableNames.at(i),pos1))!=std::wstring::npos){
							pos2=pos1+variableNames.at(i).length();
							DWRITE_TEXT_RANGE rng;
							rng.startPosition=pos1;
							rng.length=pos2-pos1;
							FontColorRange color={rng,variableBrush};
							rendererDirectX.textBox->rangesColor.push_back(color);
							pos1=pos2;
						}
					}
				}//Resaltar nombres de variable
				for(int i=0;i<parser.palabras_reservadas.size();i++){
					int pos1=0;
					int pos2=0;
					while((pos1=text.find(parser.palabras_reservadas.at(i),pos1))!=std::wstring::npos){
						pos2=pos1+parser.palabras_reservadas.at(i).length();
						DWRITE_TEXT_RANGE rng;
						rng.startPosition=pos1;
						rng.length=pos2-pos1;
						FontColorRange color={rng,reservedBrush};
						rendererDirectX.textBox->rangesColor.push_back(color);
						pos1=pos2;
					}
				}//Resaltar palabras reservadas
				for(int i=0;i<parser.parametros_lexico.size();i++){
					std::wstring regparam(std::wstring(L"[\\(|,].*(")+parser.parametros_lexico.at(i)+std::wstring(L").*[\\=]"));
					wsregex isparam=wsregex::compile(regparam);
					match_results<std::wstring::const_iterator>mr;
					std::wstring::const_iterator startPos=text.begin();
					while(regex_search(startPos,text.end(),mr,isparam)){
						int pos1=std::distance<std::wstring::const_iterator>(text.begin(),mr[1].first);
						int pos2=std::distance<std::wstring::const_iterator>(text.begin(),mr[1].second);
						DWRITE_TEXT_RANGE rng;
						rng.startPosition=pos1;
						rng.length=pos2-pos1;
						FontStyleRange style={rng,DWRITE_FONT_STYLE_ITALIC};
						FontColorRange color={rng,paramBrush};
						rendererDirectX.textBox->rangesFontStyle.push_back(style);
						rendererDirectX.textBox->rangesColor.push_back(color);
						startPos=mr[0].second;
					}
				}//Resaltar parámetros
				for(int i=0;i<parser.dibujado_lexico.size();i++){
					std::wstring regdrawmod(std::wstring(L"[\\=|,].*[\"]?.*(")+parser.dibujado_lexico.at(i)+std::wstring(L").*[\"]?"));
					wsregex isdrawmode=wsregex::compile(regdrawmod);
					match_results<std::wstring::const_iterator>mr;
					std::wstring::const_iterator startPos=text.begin();
					while(regex_search(startPos,text.end(),mr,isdrawmode)){
						int pos1=std::distance<std::wstring::const_iterator>(text.begin(),mr[1].first)-1;
						int pos2=std::distance<std::wstring::const_iterator>(text.begin(),mr[1].second)+1;
						DWRITE_TEXT_RANGE rng;
						rng.startPosition=pos1;
						rng.length=pos2-pos1;
						FontColorRange color={rng,drawModeBrush};
						rendererDirectX.textBox->rangesColor.push_back(color);
						startPos=mr[0].second;				
					}
				}//Resaltar valores de drawMode
				for(int i=0;i<parser.figuras_lexico.size();i++){
					std::wstring regtype(std::wstring(L"[\\=|,].*[\"]?.*(")+parser.figuras_lexico.at(i)+std::wstring(L").*[\"]?"));
					wsregex istype=wsregex::compile(regtype);
					match_results<std::wstring::const_iterator>mr;
					std::wstring::const_iterator startPos=text.begin();
					while(regex_search(startPos,text.end(),mr,istype)){
						int pos1=std::distance<std::wstring::const_iterator>(text.begin(),mr[1].first)-1;
						int pos2=std::distance<std::wstring::const_iterator>(text.begin(),mr[1].second)+1;
						DWRITE_TEXT_RANGE rng;
						rng.startPosition=pos1;
						rng.length=pos2-pos1;
						FontColorRange color={rng,typeBrush};
						rendererDirectX.textBox->rangesColor.push_back(color);
						startPos=mr[0].second;				
					}
				}//Resaltar valores de type
			}//Revisar que el texto del TextBox no sea nulo.
		}//Modo Direct2D
		else if(renderType==POLYSCRIPT_RENDER_OPENGL&&needsRecreation){
			rendererOpenGL.textBox->rangesColor.clear();
			if(rendererOpenGL.textBox->getText().length()!=0){
				const std::wstring text=rendererOpenGL.textBox->getText();
				wsmatch matcher;
				wsregex variabledeclaration=wsregex::compile(L"^([A-Za-z0-9_ñ]+)[\\s|\\*|&]?[\\=].*");
				std::wstring line;
				std::wistringstream stream(text);
				std::vector<std::wstring>variableNames;
				std::vector<std::wstring>valueNames;
				int lineNumber=0;
				while(std::getline(stream,line)){
					if(regex_match(line,matcher,variabledeclaration)){
						std::wstring variableName(matcher[1]);
						variableNames.push_back(variableName);
					}//Buscar posibles variables y sus valores
					if(variableNames.size()!=0){
						for(int i=0;i<variableNames.size();i++){
							int pos1=0;
							int pos2=0;
							while((pos1=line.find(variableNames.at(i),pos1))!=std::wstring::npos){
								pos2=pos1+variableNames.at(i).length();
								FTGL::FontColorRange color;
								color.startPosition=pos1;
								color.length=(pos2-pos1)-1;
								color.lineNumber=lineNumber;
								memcpy(color.color,variableColor,sizeof(variableColor));
								rendererOpenGL.textBox->rangesColor.push_back(color);
								pos1=pos2;
							}
						}
					}//Resaltar nombres de variable
					for(int i=0;i<parser.palabras_reservadas.size();i++){
						int pos1=0;
						int pos2=0;
						while((pos1=line.find(parser.palabras_reservadas.at(i),pos1))!=std::wstring::npos){
							pos2=pos1+parser.palabras_reservadas.at(i).length();
							FTGL::FontColorRange color;
							color.startPosition=pos1;
							color.lineNumber=lineNumber;
							color.length=(pos2-pos1)-1;
							memcpy(color.color,reservedColor,sizeof(reservedColor));
							rendererOpenGL.textBox->rangesColor.push_back(color);
							pos1=pos2;
						}
					}//Resaltar palabras reservadas
					for(int i=0;i<parser.parametros_lexico.size();i++){
						std::wstring regparam(std::wstring(L"[\\(|,].*(")+parser.parametros_lexico.at(i)+std::wstring(L").*[\\=]"));
						wsregex isparam=wsregex::compile(regparam);
						match_results<std::wstring::const_iterator>mr;
						std::wstring::const_iterator startPos=line.begin();
						while(regex_search(startPos,line.end(),mr,isparam)){
							int pos1=std::distance<std::wstring::const_iterator>(line.begin(),mr[1].first);
							int pos2=std::distance<std::wstring::const_iterator>(line.begin(),mr[1].second);
							FTGL::FontColorRange color;
							color.startPosition=pos1;
							color.lineNumber=lineNumber;
							color.length=(pos2-pos1)-1;
							memcpy(color.color,paramColor,sizeof(paramColor));
							rendererOpenGL.textBox->rangesColor.push_back(color);
							startPos=mr[0].second;
						}
					}//Resaltar parámetros
					for(int i=0;i<parser.dibujado_lexico.size();i++){
						std::wstring regdrawmod(std::wstring(L"[\\=|,].*[\"]?.*(")+parser.dibujado_lexico.at(i)+std::wstring(L").*[\"]?"));
						wsregex isdrawmode=wsregex::compile(regdrawmod);
						match_results<std::wstring::const_iterator>mr;
						std::wstring::const_iterator startPos=line.begin();
						while(regex_search(startPos,line.end(),mr,isdrawmode)){
							int pos1=std::distance<std::wstring::const_iterator>(line.begin(),mr[1].first)-1;
							int pos2=std::distance<std::wstring::const_iterator>(line.begin(),mr[1].second)+1;
							FTGL::FontColorRange color;
							color.startPosition=pos1;
							color.lineNumber=lineNumber;
							color.length=(pos2-pos1)-1;
							memcpy(color.color,drawModeColor,sizeof(drawModeColor));
							rendererOpenGL.textBox->rangesColor.push_back(color);
							startPos=mr[0].second;				
						}
					}//Resaltar valores de drawMode
					for(int i=0;i<parser.figuras_lexico.size();i++){
						std::wstring regtype(std::wstring(L"[\\=|,].*[\"]?.*(")+parser.figuras_lexico.at(i)+std::wstring(L").*[\"]?"));
						wsregex istype=wsregex::compile(regtype);
						match_results<std::wstring::const_iterator>mr;
						std::wstring::const_iterator startPos=line.begin();
						while(regex_search(startPos,line.end(),mr,istype)){
							int pos1=std::distance<std::wstring::const_iterator>(line.begin(),mr[1].first)-1;
							int pos2=std::distance<std::wstring::const_iterator>(line.begin(),mr[1].second)+1;
							FTGL::FontColorRange color;
							color.startPosition=pos1;
							color.lineNumber=lineNumber;
							color.length=(pos2-pos1)-1;
							memcpy(color.color,typeColor,sizeof(typeColor));
							rendererOpenGL.textBox->rangesColor.push_back(color);
							startPos=mr[0].second;				
						}
					}//Resaltar valores de type
					lineNumber++;
				}//Dividir el texto por líneas para facilitar el trabajo
			}//Revisar que el texto del TextBox no sea nulo.
		}//Modo OpenGL
		std::this_thread::sleep_for(std::chrono::milliseconds(1000/60));
	}
}//Hilo para colorear el texto
// Forward declarations of functions included in this code module:
ATOM				MyRegisterClass(HINSTANCE hInstance);
BOOL				InitInstance(HINSTANCE, int);
LRESULT CALLBACK	WndProc(HWND, UINT, WPARAM, LPARAM);

int APIENTRY WinMain(_In_ HINSTANCE hInstance,
					 _In_opt_ HINSTANCE hPrevInstance,
					 _In_ LPSTR    lpCmdLine,
					 _In_ int       nCmdShow)
{
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);
	// TODO: Place code here.
	MSG msg;
	msg.message=WM_NULL;
	MyRegisterClass(hInstance);
	settings.winwidth=1280;
	settings.winheight=720;
	settings.fullscreen=false;
	settings.screenwidth=GetSystemMetrics(SM_CXSCREEN);
	settings.screenheight=GetSystemMetrics(SM_CYSCREEN);
	INITCOMMONCONTROLSEX ccex;
	ccex.dwSize=sizeof(INITCOMMONCONTROLSEX);
	ccex.dwICC=ICC_BAR_CLASSES|ICC_STANDARD_CLASSES|ICC_LISTVIEW_CLASSES;
	InitCommonControlsEx(&ccex);
	// Perform application initialization:
	if (!InitInstance (hInstance, nCmdShow))
	{
		return FALSE;
	}
	CoInitializeEx(NULL,COINITBASE_MULTITHREADED);
	rendererInitializedDX=d2d->InitializeD2D(hwnd,settings);
	rendererInitializedGL=gl->InitializeGL(hwnd,settings);
	if(!rendererInitializedDX&&!rendererInitializedGL){
		MessageBox(NULL,L"Debe inicializarse al menos un renderizador. Usted no pudo inicializar ninguno.",L"Error de inicialización",MB_OK);
		return -1;
	}
	HDC hdc=GetWindowDC(hwnd);
	d2d->target->CreateSolidColorBrush(ColorF(0x0000FF),reservedBrush.GetAddressOf());
	d2d->target->CreateSolidColorBrush(ColorF(ColorF::Gray),paramBrush.GetAddressOf());
	d2d->target->CreateSolidColorBrush(ColorF(0x6F008A),drawModeBrush.GetAddressOf());
	d2d->target->CreateSolidColorBrush(ColorF(0x008000),typeBrush.GetAddressOf());
	d2d->target->CreateSolidColorBrush(ColorF(0x2B91AB),variableBrush.GetAddressOf());
	textcolorthreadrunning=true;
	textcolorthread=std::thread(textColorThread);
	textcolorthread.detach();
	// Main message loop:
	while (msg.message!=WM_QUIT)
	{
		if (PeekMessage(&msg,0,0,0,PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		else if(IsWindowEnabled(hwnd)){
			if(rendererInitializedDX&&renderType==POLYSCRIPT_RENDER_DIRECT2D){
				d2d->target->BeginDraw();
				d2d->target->Clear(ColorF(ColorF::White));
				if(needsRecreation){
					rendererDirectX.recreateTextRendererDX(d2d,settings,parser);
					needsRecreation=false;
				}
				D2DUISystem::getInstance().draw(d2d);
				d2d->target->EndDraw();
			}
			else if(rendererInitializedGL&&renderType==POLYSCRIPT_RENDER_OPENGL){
				glClearColor(1.0,1.0,1.0,1.0);
				glClear(GL_COLOR_BUFFER_BIT);
				wglSwapIntervalEXT(1);
				glEnable(GL_BLEND);
				glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);
				glMatrixMode(GL_PROJECTION);
				glLoadIdentity();
				glOrtho(-1,1,1,-1.0,0,1);
				glMatrixMode(GL_MODELVIEW);
				glLoadIdentity();
				if(needsRecreation){
					rendererOpenGL.recreateTextRendererGL(gl,settings,parser);
					needsRecreation=false;
				}
				OpenGLUISystem::getInstance().draw(gl);
				glDisable(GL_BLEND);
				glFlush();
				SwapBuffers(hdc);
				Sleep(5);
			}
		}
	}
	textcolorthreadrunning=false;
	return (int) msg.wParam;
}



//
//  FUNCTION: MyRegisterClass()
//
//  PURPOSE: Registers the window class.
//
ATOM MyRegisterClass(HINSTANCE hInstance)
{
	WNDCLASSEX wcex;

	wcex.cbSize = sizeof(WNDCLASSEX);

	wcex.style			= CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc	= WndProc;
	wcex.cbClsExtra		= 0;
	wcex.cbWndExtra		= 0;
	wcex.hInstance		= hInstance;
	wcex.hIcon			= LoadIcon(hInstance, MAKEINTRESOURCE(IDI_POLYSCRIPT_LANGUAGE));
	wcex.hCursor		= LoadCursor(NULL, IDC_ARROW);
	wcex.hbrBackground	= (HBRUSH)(COLOR_WINDOW+1);
	wcex.lpszMenuName	= MAKEINTRESOURCE(IDC_POLYSCRIPT_LANGUAGE);
	wcex.lpszClassName	= szWindowClass;
	wcex.hIconSm		= LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

	return RegisterClassEx(&wcex);
}

//
//   FUNCTION: InitInstance(HINSTANCE, int)
//
//   PURPOSE: Saves instance handle and creates main window
//
//   COMMENTS:
//
//        In this function, we save the instance handle in a global variable and
//        create and display the main program window.
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
	HWND hWnd;
	hInst = hInstance; // Store instance handle in our global variable
	RECT wr={0,0,1280,720};
	AdjustWindowRect(&wr,WS_OVERLAPPEDWINDOW,false);
	hWnd = CreateWindowEx(WS_EX_LAYERED,szWindowClass,L"Polyscript", WS_OVERLAPPEDWINDOW&~WS_MAXIMIZEBOX&~WS_SIZEBOX,
		0, 0, wr.right-wr.left, wr.bottom-wr.top, NULL, NULL, hInstance, NULL);
	SetLayeredWindowAttributes(hWnd,RGB(0,0,0),255,LWA_ALPHA);
	if (!hWnd){
		return FALSE;
	}
	ShowWindow(hWnd, nCmdShow);
	UpdateWindow(hWnd);
	hwnd=hWnd;
	return TRUE;
}
BYTE CheckOrUncheckRenderingMode(BYTE menuItemId,HMENU menu){
	DWORD fwdmenu;
	static BYTE attribs;
	switch(menuItemId){
	case IDM_RENDERDX:
		CheckMenuItem(menu,IDM_RENDERDX,MF_BYCOMMAND|MF_CHECKED);
		CheckMenuItem(menu,IDM_RENDERGL,MF_BYCOMMAND|MF_UNCHECKED);
		renderType=POLYSCRIPT_RENDER_DIRECT2D;
		needsRecreation=true;
		return IDM_RENDERDX;
	case IDM_RENDERGL:
		CheckMenuItem(menu,IDM_RENDERGL,MF_BYCOMMAND|MF_CHECKED);
		CheckMenuItem(menu,IDM_RENDERDX,MF_BYCOMMAND|MF_UNCHECKED);
		renderType=POLYSCRIPT_RENDER_OPENGL;
		needsRecreation=true;
		return IDM_RENDERGL;
	}
}
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	HMENU menu;
	OPENFILENAME openPolyFile;
	int wmId, wmEvent;
	wchar_t fileName[1000];
	PAINTSTRUCT ps;
	BOOL isTrailing;
	BOOL isInside;
	HDC hdc;
	int x,y;
	float textX, textY;
	wchar_t newcharacter;
	wchar_t* text;
	switch (message)
	{
	case WM_COMMAND:
		switch(wParam){
		case IDM_OPEN:
			GetCurrentDirectory(MAX_PATH,curDir);//Respalda el directorio original
			ZeroMemory(&openPolyFile,sizeof(OPENFILENAME));
			openPolyFile.lStructSize=sizeof(OPENFILENAME);
			openPolyFile.hwndOwner=hWnd;
			openPolyFile.hInstance=hInst;
			openPolyFile.lpstrFilter=L"Polyscript Files\0*.poly";
			openPolyFile.lpstrCustomFilter=NULL;
			openPolyFile.nFilterIndex=1;
			openPolyFile.lpstrFile=fileName;
			openPolyFile.lpstrFile[0]='\0';
			openPolyFile.nMaxFile=sizeof(fileName);
			openPolyFile.lpstrFileTitle=NULL;
			openPolyFile.lpstrInitialDir=NULL;
			openPolyFile.nMaxFileTitle=0;
			openPolyFile.lpstrTitle=L"Abrir archivo de Polyscript";
			openPolyFile.Flags=OFN_FILEMUSTEXIST|OFN_PATHMUSTEXIST|OFN_CREATEPROMPT|OFN_EXPLORER;
			if(GetOpenFileName(&openPolyFile)==TRUE){
				rendererDirectX.textBox->setText(L"");
				rendererOpenGL.textBox->setText(L"");
				parser.readFile(openPolyFile.lpstrFile);
				SetWindowText(hWnd,std::wstring(std::wstring(szTitle)+std::wstring(L" - ")+std::wstring(openPolyFile.lpstrFile)).c_str());
				needsRecreation=true;
			}
			SetCurrentDirectory(curDir);//Regresa al directorio original
			break;
		case IDM_CLOSE:
			ShowWindow(leftPanel,SW_HIDE);
			ShowWindow(rightPanel,SW_HIDE);
			break;
		case IDM_COMPILE:
			parser.parseFile();
			break;
		case IDM_RECREATE:
			needsRecreation=true;
			break;
		case IDM_EXIT:
			SendMessage(hWnd,WM_CLOSE,0,0);
			break;
		case IDM_RENDERDX:
		case IDM_RENDERGL:
			menu=GetMenu(hWnd);
			CheckOrUncheckRenderingMode((BYTE)LOWORD(wParam),menu);
			break;
		}
		break;
	case WM_PAINT:
		hdc = BeginPaint(hWnd, &ps);
		EndPaint(hWnd, &ps);
		break;
	case WM_LBUTTONDOWN:
		x=GET_X_LPARAM(lParam);
		y=GET_Y_LPARAM(lParam);
		switch(renderType){
		case POLYSCRIPT_RENDER_DIRECT2D:
			if(rendererDirectX.textBox->isEditable()){
				if((x>=rendererDirectX.textBox->getBounds().left&&x<=rendererDirectX.textBox->getBounds().right)&&(y>=rendererDirectX.textBox->getBounds().top&&y<=rendererDirectX.textBox->getBounds().bottom)){
					rendererDirectX.textBox->setFocus(true);
					DWRITE_HIT_TEST_METRICS htmetrics;
					rendererDirectX.textBox->textlayout->HitTestPoint(x,y,&rendererDirectX.textBox->trail,&rendererDirectX.textBox->inside,&htmetrics);
					rendererDirectX.textBox->caretPos=htmetrics.textPosition;
					rendererDirectX.textBox->measureCaret(rendererDirectX.textBox->caretPos);
				}
				else{
					rendererDirectX.textBox->setFocus(false);
				}
			}
			break;
		case POLYSCRIPT_RENDER_OPENGL:
			if(rendererOpenGL.textBox->isEditable()){
				if((x>=rendererOpenGL.textBox->getBounds().left&&x<=rendererOpenGL.textBox->getBounds().right)&&(y>=rendererOpenGL.textBox->getBounds().top&&y<=rendererOpenGL.textBox->getBounds().bottom)){
					rendererOpenGL.textBox->setFocus(true);
				}
				else{
					rendererOpenGL.textBox->setFocus(false);
				}
			}
			break;
		}
		break;
	case WM_CHAR:
		switch(renderType){
		case POLYSCRIPT_RENDER_DIRECT2D:
			if(rendererDirectX.textBox->hasFocus()){
				std::wstring txt=rendererDirectX.textBox->getText();
				switch(wParam){
				case 0x08:
					if(txt.length()>0){
						if(rendererDirectX.textBox->trail){
							txt.erase(rendererDirectX.textBox->caretPos,1);
						}
						else{
							txt.erase(rendererDirectX.textBox->caretPos-1,1);
						}
						if(rendererDirectX.textBox->caretPos>0){
							rendererDirectX.textBox->caretPos--;
						}
						else{
							rendererDirectX.textBox->trail=false;
						}
					}
					break;
				case 0x09:
					txt.insert(rendererDirectX.textBox->caretPos,L"    ");
					rendererDirectX.textBox->caretPos+=4;
					break;
				case 0x0D:
					txt.insert(rendererDirectX.textBox->caretPos+1,L"\n");
					rendererDirectX.textBox->caretPos++;

					break;
				default:
					newcharacter=(wchar_t)wParam;
					if(rendererDirectX.textBox->trail){
						txt.insert(txt.begin()+rendererDirectX.textBox->caretPos+1,1,newcharacter);
					}
					else{
						txt.insert(txt.begin()+rendererDirectX.textBox->caretPos,1,newcharacter);
					}
					rendererDirectX.textBox->caretPos++;
				}
				rendererOpenGL.textBox->setText(txt);
				rendererDirectX.textBox->setText(txt);
				rendererDirectX.textBox->measureCaret(rendererDirectX.textBox->caretPos);
			}
			break;
		case POLYSCRIPT_RENDER_OPENGL:
			if(rendererOpenGL.textBox->hasFocus()){
				std::wstring txt=rendererOpenGL.textBox->getText();
				switch(wParam){
				case 0x08:
					if(txt.length()>0)txt.pop_back();
					break;
				case 0x09:
					txt.append(L"    ");

					break;
				case 0x0D:
					txt.append(L"\n");
					break;
				default:
					newcharacter=(wchar_t)wParam;
					txt.append(1,newcharacter);
				}

				rendererOpenGL.textBox->setText(txt);
				rendererDirectX.textBox->setText(txt);
			}
			break;
		}
		break;
	case WM_CLOSE:
		DestroyWindow(hwnd);
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0;
}
