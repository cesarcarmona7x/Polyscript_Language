// stdafx.h : include file for standard system include files,
// or project specific include files that are used frequently, but
// are changed infrequently
//

#pragma once

#include "targetver.h"

#define WIN32_LEAN_AND_MEAN             // Exclude rarely-used stuff from Windows headers
// Windows Header Files:
#include <windows.h>
#include <windowsx.h>
#include <CommCtrl.h>
#include <commdlg.h>
#pragma comment(lib,"Comctl32.lib")
// C RunTime Header Files
#include <sstream>
#include <fstream>
#include <istream>
#include <stdlib.h>
#include <malloc.h>
#include <thread>
#include <memory.h>
#include <cmath>
#include <math.h>
#include <tchar.h>
#include <functional>
#include <random>
#include "Resource.h"
#include <vector>
#include <iostream>
#include <string>
#include <string.h>
#include <cstdlib>
#include <cstdarg>
#include <stdlib.h>
#include <memory>
#include <map>
#include <algorithm>
#include <codecvt>
#include <d2d1.h>
#include <d2d1_1.h>
#include <d2d1helper.h>
#include <dwrite.h>
#include <d3d10_1.h>
#include <d3d11.h>
#include <comdef.h>
#include <wincodec.h>
#include <wrl.h>
#include <wrl/client.h>
#pragma comment(lib,"d2d1")
#pragma comment(lib,"d3d10")
#pragma comment(lib,"d3d10_1")
#pragma comment(lib,"dwrite")
#pragma comment(lib,"windowscodecs")
using Microsoft::WRL::ComPtr;
using namespace D2D1;
#include <GL/glew.h>
#include <GL/wglew.h>
#include <GL/GL.h>
#include <GL/GLU.h>
#pragma comment(lib,"glew32.lib")
#pragma comment(lib,"opengl32.lib")
#include <ft2build.h>
#include FT_FREETYPE_H
#include FT_GLYPH_H
#pragma comment(lib,"freetype.lib")
#include "FTGL/ftgl.h"
#pragma comment(lib,"ftgl.lib")
#include <boost/regex.hpp>
#include <boost/xpressive/xpressive.hpp>
#include <boost/algorithm/algorithm.hpp>
#include <boost/algorithm/string.hpp>
#include <boost/algorithm/string/predicate.hpp>
#include <boost/algorithm/string_regex.hpp>
#include <boost/any.hpp>
#include <boost/lexical_cast.hpp>
#include <boost/numeric/conversion/cast.hpp>
static std::wstring random_string(size_t length){
	std::wstring charset(L"0123456789abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ");
	std::random_device rd;
	std::mt19937 generator(rd());
	std::shuffle(charset.begin(),charset.end(),generator);
	return charset.substr(0,length);
}
// TODO: reference additional headers your program requires here
#include "GameSettings.h"
#include "d2dhandle.h"
#include "GLShader.h"
#include "freetype_renderer.h"
#include "glhandle.h"
#include "D2DUI.h"
using namespace D2DUI;
#include "OpenGLUI.h"
using namespace OpenGLUI;
#include "PolyscriptVariable.h"
#include "PolyscriptParser.h"
#include "TextRendererDX.h"
#include "TextRendererGL.h"