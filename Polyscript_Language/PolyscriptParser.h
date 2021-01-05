#ifndef POLYSCRIPT_PARSER_INCLUDED
#define POLYSCRIPT_PARSER_INCLUDED
#include "stdafx.h"
class PolyscriptParser{
public:
	//Aquí se almacenarán todas las variables de Polyscript
	std::map<std::wstring,PolyscriptVariable> variableBank;
	//Aquí se almacenarán todas las referencias de Polyscript
	std::map<std::wstring,PolyscriptRef> referenceBank;
	//Aquí se almacenarán todas las figuras compiladas
	std::vector<PolyscriptFigure> figureBank;
	PolyscriptParser(){
		const std::wstring figs_lexico[]={L"Square",L"Rectangle",L"RoundedRectangle",L"Triangle",L"Ellipse",L"Line",L"Polygon"};
		const std::wstring param_lexico[]={L"radiusX",L"radiusY",L"color",L"left",L"top",L"right",L"bottom",L"x",L"y",L"width",L"height",L"type",L"brush",L"bitmap",L"gradient",L"opacity",L"drawMode"};
		const std::wstring dib_lexico[]={L"Draw",L"Fill",L"BothDrawTop",L"BothFillTop"};
		const std::wstring reser_lexico[]={L"BeginFigure",L"EndFigure",L"BeginPoint",L"EndPoint",L"for",L"if",L"switch",L"do",L"while",L"break",L"case",L"AND",L"OR"};
		for(int i=0;i<7;i++){
			figuras_lexico.push_back(figs_lexico[i]);
		}
		for(int i=0;i<17;i++){
			parametros_lexico.push_back(param_lexico[i]);
		}
		for(int i=0;i<4;i++){
			dibujado_lexico.push_back(dib_lexico[i]);
		}
		for(int i=0;i<9;i++){
			palabras_reservadas.push_back(reser_lexico[i]);
		}
	}
	~PolyscriptParser(){}
	std::vector<std::wstring>fileLines;
	void readFile(wchar_t* polyfile);
	void parseFile();
	void saveFile(wchar_t* polyfile);
	std::vector<std::wstring> figuras_lexico;
	std::vector<std::wstring> parametros_lexico;
	std::vector<std::wstring> dibujado_lexico;
	std::vector<std::wstring> palabras_reservadas;
};
#endif