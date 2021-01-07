#include "stdafx.h"
void PolyscriptParser::readFile(wchar_t* polyfile){
	std::wifstream file(polyfile);
	file.imbue(std::locale(file.getloc(),new std::codecvt_utf8<wchar_t,0x10ffff,std::consume_header>));
	std::wstring line;
	fileLines.clear();
	while(std::getline(file,line)){
		fileLines.push_back(line);
	}
	file.close();
}
void PolyscriptParser::saveFile(wchar_t* polyfile){
	std::wofstream file(polyfile,std::ios::out|std::ios::binary);
	file.imbue(std::locale(file.getloc(),new std::codecvt_utf8<wchar_t,0x10ffff,std::generate_header>));
	std::wstring fileContent=L"";
	for(int i=0;i<fileLines.size();i++){
		fileContent.append(fileLines.at(i)+L"\n");
	}
	file<<fileContent;
	file.close();
}
void PolyscriptParser::parseFile(){
	//Se usará el namespace boost::xpressive para ahorar espacio en la línea cuando se usen instrucciones como wsregex, wsmatch, wsregex::compile o regex_match
	using namespace boost::xpressive;
	PolyscriptFigureDesc figdesc;
	wsmatch matcher;
	wsregex beginfigure_basic=wsregex::compile(L"^(BeginFigure).*");
	wsregex beginpoint=wsregex::compile(L"BeginPoint\\:");
	wsregex endpoint=wsregex::compile(L"EndPoint");
	wsregex endfigure=wsregex::compile(L"EndFigure");
	wsregex variabledeclaration=wsregex::compile(L"^([A-Za-z0-9_]+).*[\\=].*([\"]?).*([A-Za-z0-9_]*).*([\"]?)");
	wsregex referencedeclaration=wsregex::compile(L"^([A-Za-z0-9_]+)&.*[\\=].?[\\*].*");
	wsregex isnumeric=wsregex::compile(L"[\\-+]?([\\d]+|[\\d]*\\.[\\d]+)");
	wsregex colorregex=wsregex::compile(L"((([0-1]+)|([0-1]*)\\.([0-9]+)([,]?)){4})");
	wsregex brushregex=wsregex::compile(L"^(brush)[\\(].*[\\)]$");
	wsregex bitmapregex=wsregex::compile(L"^(bitmap)[\\(].*[\\)]$");
	wsregex gradientregex=wsregex::compile(L"^(gradient)[\\(].*[\\)]$");
	wsregex isint=wsregex::compile(L"[\\-+]?([\\d]+)");
	wsregex isfloat=wsregex::compile(L"[\\-+]?([\\d]*\\.[\\d]+)");
	bool foundbeginfigure=false,foundendfigure=false;
	bool foundbeginpoint=false,foundendpoint=false;
	bool foundpoints=false;
	bool founderrorinsidenested=false;
	int linefoundbeginfigure=0,linefoundendfigure=0,linefoundbeginpoint=0,linefoundendpoint=0;
	figureBank.clear();
	int countbeginfigure=std::count_if(fileLines.begin(),fileLines.end(),[](std::wstring line){
		return boost::contains(line,L"BeginFigure");
	});
	int countendfigure=std::count_if(fileLines.begin(),fileLines.end(),[](std::wstring line){
		return boost::contains(line,L"EndFigure");
	});
	int countbeginpoint=std::count_if(fileLines.begin(),fileLines.end(),[](std::wstring line){
		return boost::contains(line,L"BeginPoint");
	});
	int countendpoint=std::count_if(fileLines.begin(),fileLines.end(),[](std::wstring line){
		return boost::contains(line,L"EndPoint");
	});
	if(countbeginfigure==countendfigure){
		if(countbeginpoint==countbeginpoint){
			if(fileLines.size()!=0){
				for(int i=0;i<fileLines.size();i++){
					std::wstring fileLine=fileLines.at(i);
					if(foundbeginpoint&&!foundpoints){
						wsregex pointregex=wsregex::compile(L"(x\\=\".*\").*\\,.*(y\\=\".*\").*");
						if(regex_match(fileLine,matcher,pointregex)){
							wprintf(std::wstring(std::wstring(L"[Compilación correcta] Se encontró un par de puntos (x,y) en la línea ")+std::to_wstring(i+1)+std::wstring(L".\n")).c_str());
							std::vector<std::wstring>pointvariables;
							boost::algorithm::split_regex(pointvariables,fileLine,boost::wregex(L"[\"]\\s?[,]\\s?"));
							PolyscriptPoint point;
							bool foundx=false,foundy=false;
							for(int ii=0;ii<pointvariables.size();ii++){
								while(pointvariables.at(ii).find(L"\"")!=std::wstring::npos){
									pointvariables.at(ii).replace(pointvariables.at(ii).find(L"\""),std::wstring(L"\"").size(),L"");
								}
								std::vector<std::wstring>param_and_value;
								boost::algorithm::split_regex(param_and_value,pointvariables.at(ii),boost::wregex(L"\\="));
								if(regex_match(param_and_value.at(1),matcher,isnumeric)){
									if(param_and_value.at(0)==L"x"){
										point.x=_wtof(param_and_value.at(1).c_str());
										wprintf(L"Se ha encontrado el valor del punto en x: %f.\n",point.x);
										foundx=true;
									}//Asignar el valor al punto x
									else if(param_and_value.at(0)==L"y"){
										point.y=_wtof(param_and_value.at(1).c_str());
										wprintf(L"Se ha encontrado el valor del punto en y: %f.\n",point.y);
										foundy=true;
									}//Asignar el valor al punto y
								}//Si el valor de x o y es numérico
								else{
									wprintf(std::wstring(std::wstring(L"[Error de compilación] Tienes un error en la línea ")+std::to_wstring(i+1)+std::wstring(L": ")+param_and_value.at(1)+std::wstring(L" no es un valor válido para x.\n")).c_str());
									founderrorinsidenested=true;
									break;
								}//Si el valor de x o y no es numérico
							}//Iterar los parámetros obtenidos
							if(founderrorinsidenested)break;
							if(foundx&&foundy){
								wprintf(L"[Compilación correcta] Se ha encontrado correctamente el par de puntos (x,y).\n");
								foundpoints=true;
								figdesc.points.push_back(point);
							}
						}//Si encontró una línea con puntos (x,y)
						else{
							wprintf(std::wstring(std::wstring(L"[Error de compilación] Tienes un error en la línea ")+std::to_wstring(i+1)+std::wstring(L": Sólo se aceptan puntos mientras se encuentre en un bloque BeginPoint/EndPoint.\n")).c_str());
							break;
						}//El resto de instrucciones son inválidas dentro de BeginPoint/EndPoint
					}//Si está dentro de una etiqueta BeginPoint y no ha encontrado los puntos (x,y)
					else{
						if(regex_match(fileLine,matcher,beginfigure_basic)){
							if(foundbeginfigure){
								wprintf(std::wstring(std::wstring(L"[Error de compilación] Tienes un error en la línea ")+std::to_wstring(i+1)+std::wstring(L": BeginFigure no se puede ejecutar varias veces seguidas.\n")).c_str());
								break;
							}//Repetición de BeginFigure varias veces
							else{
								figdesc=PolyscriptFigureDesc();
								linefoundbeginfigure=i+1;
								foundbeginfigure=true;
								foundendfigure=false;
								wprintf(std::wstring(std::wstring(L"[Inicio de Figura] Se ha iniciado una figura en la línea ")+std::to_wstring(i+1)+std::wstring(L".\n")).c_str());
								wsregex wellformedbeginfigure=wsregex::compile(L"^(BeginFigure).*[\\(].*(type).*[\\=].*[\"].*[\"].*[,].*(draw).*[\\=].*[\"].*[\"].*[,].*[\\)].*[:]$");
								if(regex_match(fileLine,matcher,wellformedbeginfigure)){
									wprintf(L"[Compilación correcta] La línea de BeginFigure es correcta.\n");
									int pos1=fileLine.find(L"(")+1;
									int pos2=fileLine.rfind(L")");
									std::wstring parenthesisContent=fileLine.substr(pos1,pos2-pos1);
									std::vector<std::wstring> figure_params;
									boost::algorithm::split_regex(figure_params,parenthesisContent,boost::wregex(L"[\"]\\s?[,]\\s?"));
									for(int j=0;j<figure_params.size();j++){
										std::vector<std::wstring>param_and_value;
										boost::algorithm::split_regex(param_and_value,figure_params.at(j),boost::wregex(L"\\s?\\=\\s?"));
										if(boost::contains(param_and_value.at(1),L"\"")){
											param_and_value.at(1).replace(param_and_value.at(1).find(L"\""),std::wstring(L"\"").size(),L"");
										}
										if(std::find(parametros_lexico.begin(),parametros_lexico.end(),param_and_value.at(0))!=parametros_lexico.end()){
											wprintf(std::wstring(param_and_value.at(0)+std::wstring(L" es un parámetro válido.\n")).c_str());
											if(param_and_value.at(0)==L"type"){
												if(std::find(figuras_lexico.begin(),figuras_lexico.end(),param_and_value.at(1))!=figuras_lexico.end()){
													wprintf(std::wstring(param_and_value.at(1)+std::wstring(L" es un valor válido para type.\n")).c_str());
													figdesc.type=figureAssocs[param_and_value.at(1)];
												}//Si type tiene un valor de figuras_lexico
												else if(variableBank.count(param_and_value.at(1))!=0){
													wprintf(std::wstring(std::wstring(L"Se encontró una variable existente en la línea ")+std::to_wstring(i+1)+std::wstring(L": ")+param_and_value.at(1)+std::wstring(L". Se asignará su valor al parámetro type.\n")).c_str());
												}//Si type es una variable ya existente
												else{
													wprintf(std::wstring(std::wstring(L"[Error de compilación] Tienes un error en la línea ")+std::to_wstring(i+1)+std::wstring(L": ")+param_and_value.at(1)+std::wstring(L" no es un valor válido para type.\n")).c_str());
													founderrorinsidenested=true;
													break;
												}//Si type no tiene un valor de figuras_lexico
											}//El parámetro es de tipo type
											else if(param_and_value.at(0)==L"drawMode"){
												if(std::find(dibujado_lexico.begin(),dibujado_lexico.end(),param_and_value.at(1))!=dibujado_lexico.end()){
													wprintf(std::wstring(param_and_value.at(1)+std::wstring(L" es un valor válido para drawMode.\n")).c_str());
													figdesc.drawMode=drawModeAssocs[param_and_value.at(1)];
													if(figdesc.type==POLYSCRIPT_FIGURE_TYPE_LINE&&figdesc.drawMode!=POLYSCRIPT_DRAW_MODE_DRAW){
														wprintf(std::wstring(std::wstring(L"[Error de semántica] Tienes un error en la línea ")+std::to_wstring(i+1)+std::wstring(L": Line sólo permite ser usado junto con Draw.\n")).c_str());
														founderrorinsidenested=true;
														break;
													}//Line sólo se puede usar con draw
												}//Si drawMode tiene un valor de dibujado_lexico
												else if(variableBank.count(param_and_value.at(1))!=0){
													wprintf(std::wstring(std::wstring(L"Se encontró una variable existente en la línea ")+std::to_wstring(i+1)+std::wstring(L": ")+param_and_value.at(1)+std::wstring(L". Se asignará su valor al parámetro drawMode.\n")).c_str());
												}//Si drawMode es una variable ya existente
												else{
													wprintf(std::wstring(std::wstring(L"[Error de compilación] Tienes un error en la línea ")+std::to_wstring(i+1)+std::wstring(L": ")+param_and_value.at(1)+std::wstring(L" no es un valor válido para drawMode.\n")).c_str());
													founderrorinsidenested=true;
													break;
												}//Si drawMode no tiene un valor de dibujado_lexico
											}//El parámetro es de tipo drawMode
											else if(param_and_value.at(0)==L"radiusX"){
												if(regex_match(param_and_value.at(1),matcher,isnumeric)){
													if(figdesc.type==POLYSCRIPT_FIGURE_TYPE_ROUNDEDRECTANGLE||figdesc.type==POLYSCRIPT_FIGURE_TYPE_ELLIPSE){
														wprintf(std::wstring(param_and_value.at(1)+std::wstring(L" es un valor válido para radiusX.\n")).c_str());
														figdesc.radiusX=_wtof(param_and_value.at(1).c_str());
													}//RadiusX es compatible con RoundedRectangle y Ellipse
													else{
														wprintf(L"[Error de semántica] Tienes un error en la línea %d: radiusX sólo es compatible con RoundedRectangle y Ellipse.\n",(i+1));
														founderrorinsidenested=true;
														break;
													}//Tira error en los demás casos
												}//Si radiusX es de tipo numérico
												else if(variableBank.count(param_and_value.at(1))!=0){
													if(figdesc.type==POLYSCRIPT_FIGURE_TYPE_ROUNDEDRECTANGLE||figdesc.type==POLYSCRIPT_FIGURE_TYPE_ELLIPSE){
														wprintf(std::wstring(std::wstring(L"Se encontró una variable existente en la línea ")+std::to_wstring(i+1)+std::wstring(L": ")+param_and_value.at(1)+std::wstring(L". Se asignará su valor al parámetro radiusX.\n")).c_str());
														figdesc.radiusX=variableBank[param_and_value.at(1)].getDataFloat();
													}//RadiusX es compatible con RoundedRectangle y Ellipse
													else{
														wprintf(L"[Error de semántica] Tienes un error en la línea %d: radiusX sólo es compatible con RoundedRectangle y Ellipse.\n",(i+1));
														founderrorinsidenested=true;
														break;
													}//Tira error en los demás casos
												}//Si radiusX es una variable ya existente
												else{
													wprintf(std::wstring(std::wstring(L"[Error de compilación] Tienes un error en la línea ")+std::to_wstring(i+1)+std::wstring(L": ")+param_and_value.at(1)+std::wstring(L" no es un valor válido para radiusX.\n")).c_str());
													founderrorinsidenested=true;
													break;
												}//Si radiusX no es de tipo numérico
											}//El parámetro es de tipo radiusX
											else if(param_and_value.at(0)==L"radiusY"){
												if(regex_match(param_and_value.at(1),matcher,isnumeric)){
													if(figdesc.type==POLYSCRIPT_FIGURE_TYPE_ROUNDEDRECTANGLE||figdesc.type==POLYSCRIPT_FIGURE_TYPE_ELLIPSE){
														wprintf(std::wstring(param_and_value.at(1)+std::wstring(L" es un valor válido para radiusY.\n")).c_str());
														figdesc.radiusY=_wtof(param_and_value.at(1).c_str());
													}//RadiusY es compatible con RoundedRectangle y Ellipse
													else{
														wprintf(L"[Error de semántica] Tienes un error en la línea %d: radiusY sólo es compatible con RoundedRectangle y Ellipse.\n",(i+1));
														founderrorinsidenested=true;
														break;
													}//Tira error en los demás casos
												}//Si radiusY es de tipo numérico
												else if(variableBank.count(param_and_value.at(1))!=0){
													if(figdesc.type==POLYSCRIPT_FIGURE_TYPE_ROUNDEDRECTANGLE||figdesc.type==POLYSCRIPT_FIGURE_TYPE_ELLIPSE){
														wprintf(std::wstring(std::wstring(L"Se encontró una variable existente en la línea ")+std::to_wstring(i+1)+std::wstring(L": ")+param_and_value.at(1)+std::wstring(L". Se asignará su valor al parámetro radiusY.\n")).c_str());
														figdesc.radiusY=variableBank[param_and_value.at(1)].getDataFloat();
													}//RadiusY es compatible con RoundedRectangle y Ellipse
													else{
														wprintf(L"[Error de semántica] Tienes un error en la línea %d: radiusY sólo es compatible con RoundedRectangle y Ellipse.\n",(i+1));
														founderrorinsidenested=true;
														break;
													}//Tira error en los demás casos
												}//Si radiusY es una variable ya existente
												else{
													wprintf(std::wstring(std::wstring(L"[Error de compilación] Tienes un error en la línea ")+std::to_wstring(i+1)+std::wstring(L": ")+param_and_value.at(1)+std::wstring(L" no es un valor válido para radiusY.\n")).c_str());
													founderrorinsidenested=true;
													break;
												}//Si radiusY no es de tipo numérico
											}//El parámetro es de tipo radiusY
											else if(param_and_value.at(0)==L"color"){
												if(regex_match(param_and_value.at(1),matcher,colorregex)){
													wprintf(std::wstring(param_and_value.at(1)+std::wstring(L" es un valor válido para color.\n")).c_str());
													std::vector<std::wstring> colors;
													boost::algorithm::split_regex(colors,param_and_value.at(1),boost::wregex(L"[,]"));
													figdesc.color=PolyscriptColor(_wtof(colors.at(0).c_str()),_wtof(colors.at(1).c_str()),_wtof(colors.at(2).c_str()),_wtof(colors.at(3).c_str()));
												}//Si color es de tipo float[4]
												else if(variableBank.count(param_and_value.at(1))!=0){
													wprintf(std::wstring(std::wstring(L"Se encontró una variable existente en la línea ")+std::to_wstring(i+1)+std::wstring(L": ")+param_and_value.at(1)+std::wstring(L". Se asignará su valor al parámetro color.\n")).c_str());
													float* colors=variableBank[param_and_value.at(1)].getDataColor();
													figdesc.color=PolyscriptColor(colors[0],colors[1],colors[2],colors[3]);
												}//Si color es una variable ya existente
												else{
													wprintf(std::wstring(std::wstring(L"[Error de compilación] Tienes un error en la línea ")+std::to_wstring(i+1)+std::wstring(L": ")+param_and_value.at(1)+std::wstring(L" no es un valor válido para color.\n")).c_str());
													founderrorinsidenested=true;
													break;
												}//Si color no es de tipo float[4]
											}//El parámetro es de tipo color
											else if(param_and_value.at(0)==L"left"){
												if(regex_match(param_and_value.at(1),matcher,isnumeric)){
													if(figdesc.type==POLYSCRIPT_FIGURE_TYPE_ELLIPSE||figdesc.type==POLYSCRIPT_FIGURE_TYPE_LINE||figdesc.type==POLYSCRIPT_FIGURE_TYPE_TRIANGLE||figdesc.type==POLYSCRIPT_FIGURE_TYPE_POLYGON){
														wprintf(L"[Error de semántica] Tienes un error en la línea %d: left no es compatible con Ellipse, Line, Triangle ni Polygon.\n",(i+1));
														founderrorinsidenested=true;
														break;
													}//left no es compatible con Ellipse, Line, Triangle y Polygon
													else{
														wprintf(std::wstring(param_and_value.at(1)+std::wstring(L" es un valor válido para left.\n")).c_str());
														figdesc.left=_wtof(param_and_value.at(1).c_str());
													}//left es compatible con los demás casos
												}//Si left es un valor numérico
												else if(variableBank.count(param_and_value.at(1))!=0){
													if(figdesc.type==POLYSCRIPT_FIGURE_TYPE_ELLIPSE||figdesc.type==POLYSCRIPT_FIGURE_TYPE_LINE||figdesc.type==POLYSCRIPT_FIGURE_TYPE_TRIANGLE||figdesc.type==POLYSCRIPT_FIGURE_TYPE_POLYGON){
														wprintf(L"[Error de semántica] Tienes un error en la línea %d: left no es compatible con Ellipse, Line, Triangle ni Polygon.\n",(i+1));
														founderrorinsidenested=true;
														break;
													}//left no es compatible con Ellipse, Line, Triangle y Polygon
													else{
														wprintf(std::wstring(std::wstring(L"Se encontró una variable existente en la línea ")+std::to_wstring(i+1)+std::wstring(L": ")+param_and_value.at(1)+std::wstring(L". Se asignará su valor al parámetro left.\n")).c_str());
														figdesc.left=variableBank[param_and_value.at(1)].getDataFloat();
													}//left es compatible con los demás casos
												}//Si left es una variable ya existente
												else{
													wprintf(std::wstring(std::wstring(L"[Error de compilación] Tienes un error en la línea ")+std::to_wstring(i+1)+std::wstring(L": ")+param_and_value.at(1)+std::wstring(L" no es un valor válido para left.\n")).c_str());
													founderrorinsidenested=true;
													break;
												}//Si left no es un valor numérico
											}//El parámetro es de tipo left
											else if(param_and_value.at(0)==L"top"){
												if(regex_match(param_and_value.at(1),matcher,isnumeric)){
													if(figdesc.type==POLYSCRIPT_FIGURE_TYPE_ELLIPSE||figdesc.type==POLYSCRIPT_FIGURE_TYPE_LINE||figdesc.type==POLYSCRIPT_FIGURE_TYPE_TRIANGLE||figdesc.type==POLYSCRIPT_FIGURE_TYPE_POLYGON){
														wprintf(L"[Error de semántica] Tienes un error en la línea %d: top no es compatible con Ellipse, Line, Triangle ni Polygon.\n",(i+1));
														founderrorinsidenested=true;
														break;
													}//top no es compatible con Ellipse, Line, Triangle y Polygon
													else{
														wprintf(std::wstring(param_and_value.at(1)+std::wstring(L" es un valor válido para top.\n")).c_str());
														figdesc.top=_wtof(param_and_value.at(1).c_str());
													}//top es compatible con los demás casos
												}//Si top es un valor numérico
												else if(variableBank.count(param_and_value.at(1))!=0){
													if(figdesc.type==POLYSCRIPT_FIGURE_TYPE_ELLIPSE||figdesc.type==POLYSCRIPT_FIGURE_TYPE_LINE||figdesc.type==POLYSCRIPT_FIGURE_TYPE_TRIANGLE||figdesc.type==POLYSCRIPT_FIGURE_TYPE_POLYGON){
														wprintf(L"[Error de semántica] Tienes un error en la línea %d: top no es compatible con Ellipse, Line, Triangle ni Polygon.\n",(i+1));
														founderrorinsidenested=true;
														break;
													}//top no es compatible con Ellipse, Line, Triangle y Polygon
													else{
														wprintf(std::wstring(std::wstring(L"Se encontró una variable existente en la línea ")+std::to_wstring(i+1)+std::wstring(L": ")+param_and_value.at(1)+std::wstring(L". Se asignará su valor al parámetro top.\n")).c_str());
														figdesc.top=variableBank[param_and_value.at(1)].getDataFloat();
													}//top es compatible con los demás casos
												}//Si top es una variable ya existente
												else{
													wprintf(std::wstring(std::wstring(L"[Error de compilación] Tienes un error en la línea ")+std::to_wstring(i+1)+std::wstring(L": ")+param_and_value.at(1)+std::wstring(L" no es un valor válido para top.\n")).c_str());
													founderrorinsidenested=true;
													break;
												}//Si top no es un valor numérico
											}//El parámetro es de tipo top
											else if(param_and_value.at(0)==L"right"){
												if(regex_match(param_and_value.at(1),matcher,isnumeric)){
													if(figdesc.type==POLYSCRIPT_FIGURE_TYPE_ELLIPSE||figdesc.type==POLYSCRIPT_FIGURE_TYPE_LINE||figdesc.type==POLYSCRIPT_FIGURE_TYPE_TRIANGLE||figdesc.type==POLYSCRIPT_FIGURE_TYPE_POLYGON){
														wprintf(L"[Error de semántica] Tienes un error en la línea %d: right no es compatible con Ellipse, Line, Triangle ni Polygon.\n",(i+1));
														founderrorinsidenested=true;
														break;
													}//right no es compatible con Ellipse, Line, Triangle y Polygon
													else{
														wprintf(std::wstring(param_and_value.at(1)+std::wstring(L" es un valor válido para right.\n")).c_str());
														figdesc.right=_wtof(param_and_value.at(1).c_str());
													}//right es compatible con los demás casos
												}//Si right es un valor numérico
												else if(variableBank.count(param_and_value.at(1))!=0){
													if(figdesc.type==POLYSCRIPT_FIGURE_TYPE_ELLIPSE||figdesc.type==POLYSCRIPT_FIGURE_TYPE_LINE||figdesc.type==POLYSCRIPT_FIGURE_TYPE_TRIANGLE||figdesc.type==POLYSCRIPT_FIGURE_TYPE_POLYGON){
														wprintf(L"[Error de semántica] Tienes un error en la línea %d: right no es compatible con Ellipse, Line, Triangle ni Polygon.\n",(i+1));
														founderrorinsidenested=true;
														break;
													}//right no es compatible con Ellipse, Line, Triangle y Polygon
													else{
														wprintf(std::wstring(std::wstring(L"Se encontró una variable existente en la línea ")+std::to_wstring(i+1)+std::wstring(L": ")+param_and_value.at(1)+std::wstring(L". Se asignará su valor al parámetro right.\n")).c_str());
														figdesc.right=variableBank[param_and_value.at(1)].getDataFloat();
													}//right es compatible con los demás casos
												}//Si right es una variable ya existente
												else{
													wprintf(std::wstring(std::wstring(L"[Error de compilación] Tienes un error en la línea ")+std::to_wstring(i+1)+std::wstring(L": ")+param_and_value.at(1)+std::wstring(L" no es un valor válido para right.\n")).c_str());
													founderrorinsidenested=true;
													break;
												}//Si right no es un valor numérico
											}//El parámetro es de tipo right
											else if(param_and_value.at(0)==L"bottom"){
												if(regex_match(param_and_value.at(1),matcher,isnumeric)){
													if(figdesc.type==POLYSCRIPT_FIGURE_TYPE_ELLIPSE||figdesc.type==POLYSCRIPT_FIGURE_TYPE_LINE||figdesc.type==POLYSCRIPT_FIGURE_TYPE_TRIANGLE||figdesc.type==POLYSCRIPT_FIGURE_TYPE_POLYGON){
														wprintf(L"[Error de semántica] Tienes un error en la línea %d: bottom no es compatible con Ellipse, Line, Triangle ni Polygon.\n",(i+1));
														founderrorinsidenested=true;
														break;
													}//bottom no es compatible con Ellipse, Line, Triangle y Polygon
													else{
														wprintf(std::wstring(param_and_value.at(1)+std::wstring(L" es un valor válido para bottom.\n")).c_str());
														figdesc.bottom=_wtof(param_and_value.at(1).c_str());
													}//bottom es compatible con los demás casos
												}//Si bottom es un valor numérico
												else if(variableBank.count(param_and_value.at(1))!=0){
													if(figdesc.type==POLYSCRIPT_FIGURE_TYPE_ELLIPSE||figdesc.type==POLYSCRIPT_FIGURE_TYPE_LINE||figdesc.type==POLYSCRIPT_FIGURE_TYPE_TRIANGLE||figdesc.type==POLYSCRIPT_FIGURE_TYPE_POLYGON){
														wprintf(L"[Error de semántica] Tienes un error en la línea %d: bottom no es compatible con Line, Triangle ni Polygon.\n",(i+1));
														founderrorinsidenested=true;
														break;
													}//bottom no es compatible con Ellipse, Line, Triangle y Polygon
													else{
														wprintf(std::wstring(std::wstring(L"Se encontró una variable existente en la línea ")+std::to_wstring(i+1)+std::wstring(L": ")+param_and_value.at(1)+std::wstring(L". Se asignará su valor al parámetro radiusX.\n")).c_str());
														figdesc.bottom=variableBank[param_and_value.at(1)].getDataFloat();
													}//bottom es compatible con los demás casos
												}//Si bottom es una variable ya existente
												else{
													wprintf(std::wstring(std::wstring(L"[Error de compilación] Tienes un error en la línea ")+std::to_wstring(i+1)+std::wstring(L": ")+param_and_value.at(1)+std::wstring(L" no es un valor válido para bottom.\n")).c_str());
													founderrorinsidenested=true;
													break;
												}//Si bottom no es un valor numérico
											}//El parámetro es de tipo bottom
											else if(param_and_value.at(0)==L"x"){
												if(regex_match(param_and_value.at(1),matcher,isnumeric)){
													if(figdesc.type==POLYSCRIPT_FIGURE_TYPE_LINE||figdesc.type==POLYSCRIPT_FIGURE_TYPE_TRIANGLE||figdesc.type==POLYSCRIPT_FIGURE_TYPE_POLYGON){
														wprintf(L"[Error de semántica] Tienes un error en la línea %d: x no es compatible con Line, Triangle ni Polygon.\n",(i+1));
														founderrorinsidenested=true;
														break;
													}//x no es compatible con Line, Triangle y Polygon
													else{
														wprintf(std::wstring(param_and_value.at(1)+std::wstring(L" es un valor válido para x.\n")).c_str());
														figdesc.x=_wtof(param_and_value.at(1).c_str());
													}//x es compatible con los demás casos
												}//Si x es un valor numérico
												else if(variableBank.count(param_and_value.at(1))!=0){
													if(figdesc.type==POLYSCRIPT_FIGURE_TYPE_LINE||figdesc.type==POLYSCRIPT_FIGURE_TYPE_TRIANGLE||figdesc.type==POLYSCRIPT_FIGURE_TYPE_POLYGON){
														wprintf(L"[Error de semántica] Tienes un error en la línea %d: x no es compatible con Line, Triangle ni Polygon.\n",(i+1));
														founderrorinsidenested=true;
														break;
													}//x no es compatible con Line, Triangle y Polygon
													else{
														wprintf(std::wstring(std::wstring(L"Se encontró una variable existente en la línea ")+std::to_wstring(i+1)+std::wstring(L": ")+param_and_value.at(1)+std::wstring(L". Se asignará su valor al parámetro x.\n")).c_str());
														figdesc.x=variableBank[param_and_value.at(1)].getDataFloat();
													}//x es compatible con los demás casos
												}//Si x es una variable ya existente
												else{
													wprintf(std::wstring(std::wstring(L"[Error de compilación] Tienes un error en la línea ")+std::to_wstring(i+1)+std::wstring(L": ")+param_and_value.at(1)+std::wstring(L" no es un valor válido para x.\n")).c_str());
													founderrorinsidenested=true;
													break;
												}//Si x no es un valor numérico
											}//El parámetro es de tipo x
											else if(param_and_value.at(0)==L"y"){
												if(regex_match(param_and_value.at(1),matcher,isnumeric)){
													if(figdesc.type==POLYSCRIPT_FIGURE_TYPE_LINE||figdesc.type==POLYSCRIPT_FIGURE_TYPE_TRIANGLE||figdesc.type==POLYSCRIPT_FIGURE_TYPE_POLYGON){
														wprintf(L"[Error de semántica] Tienes un error en la línea %d: y no es compatible con Line, Triangle ni Polygon.\n",(i+1));
														founderrorinsidenested=true;
														break;
													}//y no es compatible con Line, Triangle y Polygon
													else{
														wprintf(std::wstring(param_and_value.at(1)+std::wstring(L" es un valor válido para y.\n")).c_str());
														figdesc.y=_wtof(param_and_value.at(1).c_str());
													}//y es compatible con los demás casos
												}//Si y es un valor numérico
												else if(variableBank.count(param_and_value.at(1))!=0){
													if(figdesc.type==POLYSCRIPT_FIGURE_TYPE_LINE||figdesc.type==POLYSCRIPT_FIGURE_TYPE_TRIANGLE||figdesc.type==POLYSCRIPT_FIGURE_TYPE_POLYGON){
														wprintf(L"[Error de semántica] Tienes un error en la línea %d: y no es compatible con Line, Triangle ni Polygon.\n",(i+1));
														founderrorinsidenested=true;
														break;
													}//y no es compatible con Line, Triangle y Polygon
													else{
														wprintf(std::wstring(std::wstring(L"Se encontró una variable existente en la línea ")+std::to_wstring(i+1)+std::wstring(L": ")+param_and_value.at(1)+std::wstring(L". Se asignará su valor al parámetro y.\n")).c_str());
														figdesc.y=variableBank[param_and_value.at(1)].getDataFloat();
													}//y es compatible con los demás casos
												}//Si y es una variable ya existente
												else{
													wprintf(std::wstring(std::wstring(L"[Error de compilación] Tienes un error en la línea ")+std::to_wstring(i+1)+std::wstring(L": ")+param_and_value.at(1)+std::wstring(L" no es un valor válido para y.\n")).c_str());
													founderrorinsidenested=true;
													break;
												}//Si y no es un valor numérico
											}//El parámetro es de tipo y
											else if(param_and_value.at(0)==L"width"){
												if(regex_match(param_and_value.at(1),matcher,isnumeric)){
													if(figdesc.type==POLYSCRIPT_FIGURE_TYPE_LINE||figdesc.type==POLYSCRIPT_FIGURE_TYPE_TRIANGLE||figdesc.type==POLYSCRIPT_FIGURE_TYPE_POLYGON){
														wprintf(L"[Error de semántica] Tienes un error en la línea %d: width no es compatible con Line, Triangle ni Polygon.\n",(i+1));
														founderrorinsidenested=true;
														break;
													}//width no es compatible con Line, Triangle y Polygon
													else{
														wprintf(std::wstring(param_and_value.at(1)+std::wstring(L" es un valor válido para width.\n")).c_str());
														figdesc.width=_wtof(param_and_value.at(1).c_str());
													}//width es compatible con los demás casos
												}//Si width es un valor numérico
												else if(variableBank.count(param_and_value.at(1))!=0){
													if(figdesc.type==POLYSCRIPT_FIGURE_TYPE_LINE||figdesc.type==POLYSCRIPT_FIGURE_TYPE_TRIANGLE||figdesc.type==POLYSCRIPT_FIGURE_TYPE_POLYGON){
														wprintf(L"[Error de semántica] Tienes un error en la línea %d: width no es compatible con Line, Triangle ni Polygon.\n",(i+1));
														founderrorinsidenested=true;
														break;
													}//width no es compatible con Line, Triangle y Polygon
													else{
														wprintf(std::wstring(std::wstring(L"Se encontró una variable existente en la línea ")+std::to_wstring(i+1)+std::wstring(L": ")+param_and_value.at(1)+std::wstring(L". Se asignará su valor al parámetro width.\n")).c_str());
														figdesc.width=variableBank[param_and_value.at(1)].getDataFloat();
													}//width es compatible con los demás casos
												}//Si width es una variable ya existente
												else{
													wprintf(std::wstring(std::wstring(L"[Error de compilación] Tienes un error en la línea ")+std::to_wstring(i+1)+std::wstring(L": ")+param_and_value.at(1)+std::wstring(L" no es un valor válido para width.\n")).c_str());
													founderrorinsidenested=true;
													break;
												}//Si width no es un valor numérico
											}//El parámetro es de tipo width
											else if(param_and_value.at(0)==L"height"){
												if(regex_match(param_and_value.at(1),matcher,isnumeric)){
													if(figdesc.type==POLYSCRIPT_FIGURE_TYPE_LINE||figdesc.type==POLYSCRIPT_FIGURE_TYPE_TRIANGLE||figdesc.type==POLYSCRIPT_FIGURE_TYPE_POLYGON){
														wprintf(L"[Error de semántica] Tienes un error en la línea %d: height no es compatible con Line, Triangle ni Polygon.\n",(i+1));
														founderrorinsidenested=true;
														break;
													}//height no es compatible con Line, Triangle y Polygon
													else{
														wprintf(std::wstring(param_and_value.at(1)+std::wstring(L" es un valor válido para height.\n")).c_str());
														figdesc.height=_wtof(param_and_value.at(1).c_str());
													}//height es compatible con los demás casos
												}//Si height es un valor numérico
												else if(variableBank.count(param_and_value.at(1))!=0){
													if(figdesc.type==POLYSCRIPT_FIGURE_TYPE_LINE||figdesc.type==POLYSCRIPT_FIGURE_TYPE_TRIANGLE||figdesc.type==POLYSCRIPT_FIGURE_TYPE_POLYGON){
														wprintf(L"[Error de semántica] Tienes un error en la línea %d: height no es compatible con Line, Triangle ni Polygon.\n",(i+1));
														founderrorinsidenested=true;
														break;
													}//height no es compatible con Line, Triangle y Polygon
													else{
														wprintf(std::wstring(std::wstring(L"Se encontró una variable existente en la línea ")+std::to_wstring(i+1)+std::wstring(L": ")+param_and_value.at(1)+std::wstring(L". Se asignará su valor al parámetro height.\n")).c_str());
														figdesc.height=variableBank[param_and_value.at(1)].getDataFloat();
													}//height es compatible con los demás casos
												}//Si height es una variable ya existente
												else{
													wprintf(std::wstring(std::wstring(L"[Error de compilación] Tienes un error en la línea ")+std::to_wstring(i+1)+std::wstring(L": ")+param_and_value.at(1)+std::wstring(L" no es un valor válido para height.\n")).c_str());
													founderrorinsidenested=true;
													break;
												}//Si height no es un valor numérico
											}//El parámetro es de tipo height
											else if(param_and_value.at(0)==L"brush"){
												if(regex_match(param_and_value.at(1),matcher,brushregex)){
													int pos1=param_and_value.at(1).find(L"(");
													if(pos1!=std::wstring::npos){
														pos1++;
														int pos2=param_and_value.at(1).find(L")");
														std::wstring value=param_and_value.at(1).substr(pos1,pos2-pos1);
														if(boost::contains(value,L"\"")){
															value.replace(value.find(L"\""),std::wstring(L"\"").size(),L"");
														}
														if(regex_match(value,matcher,colorregex)){
															wprintf(std::wstring(std::wstring(L"Se usará un color con valor ")+value+std::wstring(L" para el nuevo PolyscriptBrush ")+param_and_value.at(0)+std::wstring(L".\n")).c_str());
															std::vector<std::wstring>colors;
															boost::algorithm::split_regex(colors,value,boost::wregex(L"[,]"));
															PolyscriptColor color(_wtof(colors.at(0).c_str()),_wtof(colors.at(1).c_str()),_wtof(colors.at(2).c_str()),_wtof(colors.at(3).c_str()));
															PolyscriptBrush brush(color);
															variableBank.insert(std::pair<std::wstring,PolyscriptBrush>(param_and_value.at(0),brush));
														}//Si el parámetro que contiene brush es un string de tipo color
														else if(variableBank.count(value)!=0){
															wprintf(std::wstring(std::wstring(L"Se encontró una variable existente en la línea ")+std::to_wstring(i+1)+std::wstring(L": ")+value+std::wstring(L". Se asignará su valor al nuevo PolyscriptBrush ")+param_and_value.at(0)+std::wstring(L".\n")).c_str());
															PolyscriptColor color=static_cast<PolyscriptColor&>(variableBank[value]);
															PolyscriptBrush brush(color);
															variableBank.insert(std::pair<std::wstring,PolyscriptBrush>(param_and_value.at(0),brush));
														}//Si el parámetro que contiene brush es una variable ya existente
														else{
															wprintf(std::wstring(std::wstring(L"[Error de compilación] Tienes un error en la línea ")+std::to_wstring(i+1)+std::wstring(L": El valor ")+value+std::wstring (L" no es válido para PolyscriptBrush.\n")).c_str());
															break;
														}//Si el parámetro que contiene brush no es válido
													}
												}//Si brush contiene un valor brush(...)
												else if(variableBank.count(param_and_value.at(1))!=0){
													wprintf(std::wstring(std::wstring(L"Se encontró una variable existente en la línea ")+std::to_wstring(i+1)+std::wstring(L": ")+param_and_value.at(1)+std::wstring(L". Se asignará su valor al parámetro brush.\n")).c_str());
												}//Si brush es una variable ya existente
												else{
													wprintf(std::wstring(std::wstring(L"[Error de compilación] Tienes un error en la línea ")+std::to_wstring(i+1)+std::wstring(L": ")+param_and_value.at(1)+std::wstring(L" no es un valor válido para brush.\n")).c_str());
													founderrorinsidenested=true;
													break;
												}//Si brush no contiene un valor brush(...)
											}//El parámetro es de tipo brush
											else if(param_and_value.at(0)==L"bitmap"){
												int pos1=param_and_value.at(1).find(L"(");
												if(pos1!=std::wstring::npos){
													pos1++;
													int pos2=param_and_value.at(1).find(L")");
													std::wstring value=param_and_value.at(1).substr(pos1,pos2-pos1);
													//Supported image formats by STB_Image
													wsregex isfilepath=wsregex::compile(L"([a-zA-Z]:[\\\\\/])?(?:[a-zA-Z0-9]+[\\\\\/])*([a-zA-Z0-9]+[\\.](JPG|jpg|JPEG|jpeg|PNG|png|BMP|bmp|TGA|tga|PSD|psd))");
													if(regex_match(value,matcher,isfilepath)){
														wprintf(std::wstring(std::wstring(L"Se usará el archivo ubicado en ")+value+std::wstring(L" para bitmap.\n")).c_str());
														figdesc.bitmap=PolyscriptBitmap(value);
													}//Si el parámetro que contiene bitmap es una ruta a una imagen
													else{
														wprintf(std::wstring(std::wstring(L"[Error de compilación] Tienes un error en la línea ")+std::to_wstring(i+1)+std::wstring(L": El valor ")+value+std::wstring (L" no es válido para PolyscriptBitmap.\n")).c_str());
														founderrorinsidenested=true;
														break;
													}//Si el parámetro que contiene bitmap no es válido
												}
											}//El parámetro es de tipo bitmap
											else if(param_and_value.at(0)==L"gradient"){
												if(regex_match(param_and_value.at(1),matcher,gradientregex)){
													int pos1=param_and_value.at(1).find(L"(");
													if(pos1!=std::wstring::npos){
														pos1++;
														int pos2=param_and_value.at(1).find(L")");
														std::wstring value=param_and_value.at(1).substr(pos1,pos2-pos1);
														wsregex isgradientstop=wsregex::compile(L"([{].*[}]([,]?))+");
														std::wstring::const_iterator startPos=value.begin();
														if(regex_match(value,matcher,isgradientstop)){
															wprintf(std::wstring(std::wstring(L"Se usarán los gradientes ")+value+std::wstring(L" para gradient.\n")).c_str());
															std::vector<std::wstring>gradientStrings;
															boost::algorithm::split_regex(gradientStrings,value,boost::wregex(L"[}][\\,]"));
															std::vector<PolyscriptGradientStop> stops;
															for(std::wstring ws:gradientStrings){
																PolyscriptGradientStop gradientstop;
																while(ws.find(L"{")!=std::wstring::npos){
																	ws.replace(ws.find(L"{"),std::wstring(L"{").size(),L"");
																}
																std::vector<std::wstring>gradientparams;
																boost::algorithm::split_regex(gradientparams,ws,boost::wregex(L";"));
																std::vector<std::wstring>colors;
																boost::algorithm::split_regex(colors,gradientparams.at(0),boost::wregex(L"[,]"));
																gradientstop.color=PolyscriptColor(_wtof(colors.at(0).c_str()),_wtof(colors.at(1).c_str()),_wtof(colors.at(2).c_str()),_wtof(colors.at(3).c_str()));
																gradientstop.positionX=_wtof(gradientparams.at(1).c_str());
																gradientstop.positionY=_wtof(gradientparams.at(2).c_str());
																stops.push_back(gradientstop);
															}
															figdesc.gradient=PolyscriptGradient(stops);
														}//Si el parámetro que contiene gradient es un grupo de 1 o más gradientes
														else{
															wprintf(std::wstring(std::wstring(L"[Error de compilación] Tienes un error en la línea ")+std::to_wstring(i+1)+std::wstring(L": El valor ")+value+std::wstring (L" no es válido para PolyscriptGradient.\n")).c_str());
															founderrorinsidenested=true;
															break;
														}//Si el parámetro que contiene gradient no es válido
													}											
												}//Si gradient contiene un valor gradient(...)
											}//El parámetro es de tipo gradient
											else if(param_and_value.at(0)==L"opacity"){
												if(regex_match(param_and_value.at(1),matcher,isnumeric)){
													wprintf(std::wstring(param_and_value.at(1)+std::wstring(L" es un valor válido para opacity.\n")).c_str());
													figdesc.opacity=_wtof(param_and_value.at(1).c_str());
												}//Si opacity es un valor numérico
												else if(variableBank.count(param_and_value.at(1))!=0){
													wprintf(std::wstring(std::wstring(L"Se encontró una variable existente en la línea ")+std::to_wstring(i+1)+std::wstring(L": ")+param_and_value.at(1)+std::wstring(L". Se asignará su valor al parámetro opacity.\n")).c_str());
													figdesc.opacity=variableBank[param_and_value.at(1)].getDataFloat();
												}//Si opacity es una variable ya existente
												else{
													wprintf(std::wstring(std::wstring(L"[Error de compilación] Tienes un error en la línea ")+std::to_wstring(i+1)+std::wstring(L": ")+param_and_value.at(1)+std::wstring(L" no es un valor válido para opacity.\n")).c_str());
													founderrorinsidenested=true;
													break;
												}//Si opacity no es un valor numérico
											}//El parámetro es de tipo opacity
										}//Parametro que se encuentra en la lista de parametros_lexico
										else{
											wprintf(std::wstring(std::wstring(L"[Error de compilación] Tienes un error en la linea ")+std::to_wstring(i+1)+std::wstring(L": ")+param_and_value.at(0)+L" no se encuentra en la lista de parámetros aceptados.\n").c_str());
											founderrorinsidenested=true;
											break;
										}//Parámetro que no se encuentra en la lista de parametros_lexico
									}//Iterar por los parámetros de la línea de BeginFigure
									if(founderrorinsidenested)break;//Si encuentra errores adentro de la iteración de parámetros, rompe la compilación
								}//Sintaxis correcta en la etiqueta BeginFigure
								else{
									wprintf(std::wstring(std::wstring(L"[Error de compilación] Tienes un error en la línea ")+std::to_wstring(i+1)+std::wstring(L": Sentencia BeginFigure mal formada.\n")).c_str());
									break;
								}//Sintaxis incorrecta en la etiqueta BeginFigure
							}//Es la primera vez que ves un BeginFigure consecutivo
						}//Encontró una etiqueta BeginFigure
						else if(regex_match(fileLine,matcher,beginpoint)){
							if(foundbeginfigure){
								if(foundbeginpoint){
									wprintf(std::wstring(std::wstring(L"[Error de compilación] Tienes un error en la línea ")+std::to_wstring(i+1)+std::wstring(L": BeginPoint no se puede ejecutar varias veces seguidas.\n")).c_str());
									break;
								}//Repetición de BeginPoint varias veces
								else{
									foundbeginpoint=true;
									foundendpoint=false;
									linefoundbeginpoint=i+1;
									wprintf(std::wstring(std::wstring(L"[Inicio de Punto] Se ha iniciado un punto en la línea ")+std::to_wstring(i+1)+std::wstring(L".\n")).c_str());
								}//Es la primera vez que ves un BeginPoint consecutivo
							}//BeginPoint siempre debe estar anidado dentro de un BeginFigure
							else{
								wprintf(std::wstring(std::wstring(L"[Error de compilación] Tienes un error en la línea "+std::wstring(L": BeginPoint debe estar contenido dentro de un BeginFigure.\n"))).c_str());
								break;
							}//BeginPoint no está contenido dentro de un BeginFigure
						}//Encontró una etiqueta BeginPoint
						else if(regex_match(fileLine,matcher,endpoint)){
							if(foundbeginfigure){
								if(foundendpoint){
									wprintf(std::wstring(std::wstring(L"[Error de compilación] Tienes un error en la línea ")+std::to_wstring(i+1)+std::wstring(L": EndPoint no se puede ejecutar varias veces seguidas.\n")).c_str());
									break;
								}//Encontraste un EndPoint consecutivo
								else{
									foundendpoint=true;
									foundpoints=false;
									foundbeginpoint=false;
									linefoundendpoint=i+1;
									wprintf(std::wstring(std::wstring(L"[Fin de Punto] Se ha formado el punto que abarca de las líneas ")+std::to_wstring(linefoundbeginpoint)+std::wstring(L" hasta la línea ")+std::to_wstring(linefoundendpoint)+std::wstring(L".\n")).c_str());
								}//Es la primera vez que ves un EndPoint consecutivo
							}//EndPoint siempre debe estar anidado dentro de un BeginFigure
							else{
								wprintf(std::wstring(std::wstring(L"[Error de compilación] Tienes un error en la línea ")+std::to_wstring(i+1)+std::wstring(L": EndPoint debe estar contenido dentro de un BeginFigure.\n")).c_str());
								break;
							}//EndPoint no está contenido dentro de un BeginFigure
						}//Encontró una etiqueta EndPoint
						else if(regex_match(fileLine,matcher,endfigure)){
							if(foundbeginfigure){
								if(foundendfigure){
									wprintf(std::wstring(std::wstring(L"[Error de compilación] Tienes un error en la línea ")+std::to_wstring(i+1)+std::wstring(L": EndFigure no se puede ejecutar varias veces seguidas.\n")).c_str());
									break;
								}//Encontraste un EndFigure consecutivo
								else{
									foundendfigure=true;
									foundbeginfigure=false;
									linefoundendfigure=i+1;
									wprintf(std::wstring(std::wstring(L"[Fin de figura] Se ha formado la figura que abarca de las líneas ")+std::to_wstring(linefoundbeginfigure)+std::wstring(L" hasta la línea ")+std::to_wstring(linefoundendfigure)+std::wstring(L".\n")).c_str());
									PolyscriptFigure nuevaFigura(figdesc);
									figureBank.push_back(nuevaFigura);
								}//Es la primera vez que ves un EndFigure consecutivo
							}//Existe un BeginFigure que sirva de pareja para el EndFigure
							else{
								wprintf(std::wstring(std::wstring(L"[Error de compilación]: Tienes un error en la línea ")+std::to_wstring(i+1)+std::wstring(L": EndFigure necesita ser emparejado con un BeginFigure. Vea el manual de usuario para más información.\n")).c_str());
								break;
							}//No existe un BeginFigure que sirva de pareja para el EndFigure
						}//Encontró una etiqueta EndFigure
						else if(regex_match(fileLine,matcher,referencedeclaration)){
							wprintf(std::wstring(std::wstring(L"Se encontró una referencia: ")+fileLine+std::wstring(L".\n")).c_str());
							std::vector<std::wstring>name_and_value;
							boost::algorithm::split_regex(name_and_value,fileLine,boost::wregex(L"[\\=]"));
							name_and_value.at(0).replace(name_and_value.at(0).find(L"&"),std::wstring(L"&").size(),L"");
							name_and_value.at(1).replace(name_and_value.at(1).find(L"*"),std::wstring(L"*").size(),L"");
							if(variableBank.count(name_and_value.at(1))!=0){
								PolyscriptRef ref=PolyscriptRef(std::ref(variableBank[name_and_value.at(1)]));
								referenceBank.insert(std::pair<std::wstring,PolyscriptRef>(name_and_value.at(0),ref));
							}//Si la referencia se encuentra disponible en la lista de variables
							else{
								wprintf(std::wstring(std::wstring(L"[Error de compilación] Tienes un error en la línea ")+std::to_wstring(i+1)+std::wstring(L": La referencia ")+name_and_value.at(0)+std::wstring (L" no contiene una referencia de memoria válida.\n")).c_str());
								break;
							}//Si la referencia no se encuentra disponible en la lista de variables
						}//Encontró una posible etiqueta de referencia
						else if(regex_match(fileLine,matcher,variabledeclaration)){
							wprintf(std::wstring(std::wstring(L"Se encontró una variable: ")+fileLine+std::wstring(L".\n")).c_str());
							std::vector<std::wstring>name_and_value;
							boost::algorithm::split_regex(name_and_value,fileLine,boost::wregex(L"[\\=]"));
							if(boost::contains(name_and_value.at(1),L"\"")){
								while(name_and_value.at(1).find(L"\"")!=std::wstring::npos){
									name_and_value.at(1).replace(name_and_value.at(1).find(L"\""),std::wstring(L"\"").size(),L"");
								}
							}
							wprintf(name_and_value.at(1).c_str());
							wprintf(L"\n");
							if(regex_match(name_and_value.at(1),matcher,colorregex)){
								wprintf(L"Regex de color. \n");
								std::vector<std::wstring>colors;
								boost::algorithm::split_regex(colors,name_and_value.at(1),boost::wregex(L"[,]"));
								PolyscriptColor color(_wtof(colors.at(0).c_str()),_wtof(colors.at(1).c_str()),_wtof(colors.at(2).c_str()),_wtof(colors.at(3).c_str()));
								variableBank.insert(std::pair<std::wstring,PolyscriptColor>(name_and_value.at(0),color));
							}//Encontró una variable de tipo PolyscriptColor
							else if(regex_match(name_and_value.at(1),matcher,brushregex)){
								wprintf(L"Regex de brush. \n");
								int pos1=name_and_value.at(1).find(L"(");
								if(pos1!=std::wstring::npos){
									pos1++;
									int pos2=name_and_value.at(1).find(L")");
									std::wstring value=name_and_value.at(1).substr(pos1,pos2-pos1);
									if(regex_match(value,matcher,colorregex)){
										wprintf(std::wstring(std::wstring(L"Se usará un color con valor ")+value+std::wstring(L" para el nuevo PolyscriptBrush ")+name_and_value.at(0)+std::wstring(L".\n")).c_str());
										std::vector<std::wstring>colors;
										boost::algorithm::split_regex(colors,value,boost::wregex(L"[,]"));
										PolyscriptColor color(_wtof(colors.at(0).c_str()),_wtof(colors.at(1).c_str()),_wtof(colors.at(2).c_str()),_wtof(colors.at(3).c_str()));
										PolyscriptBrush brush(color);
										variableBank.insert(std::pair<std::wstring,PolyscriptBrush>(name_and_value.at(0),brush));
									}//Si el parámetro que contiene brush es un string de tipo color
									else if(variableBank.count(value)!=0){
										wprintf(std::wstring(std::wstring(L"Se encontró una variable existente en la línea ")+std::to_wstring(i+1)+std::wstring(L": ")+value+std::wstring(L". Se asignará su valor al nuevo PolyscriptBrush ")+name_and_value.at(0)+std::wstring(L".\n")).c_str());
										PolyscriptColor color=static_cast<PolyscriptColor&>(variableBank[value]);
										PolyscriptBrush brush(color);
										variableBank.insert(std::pair<std::wstring,PolyscriptBrush>(name_and_value.at(0),brush));
									}//Si el parámetro que contiene brush es una variable ya existente
									else{
										wprintf(std::wstring(std::wstring(L"[Error de compilación] Tienes un error en la línea ")+std::to_wstring(i+1)+std::wstring(L": El valor ")+value+std::wstring (L" no es válido para PolyscriptBrush.\n")).c_str());
										break;
									}//Si el parámetro que contiene brush no es válido
								}
							}//Encontró una variable de tipo PolyscriptBrush
							else if(regex_match(name_and_value.at(1),matcher,bitmapregex)){
								wprintf(L"Regex de bitmap. \n");
								int pos1=name_and_value.at(1).find(L"(");
								if(pos1!=std::wstring::npos){
									pos1++;
									int pos2=name_and_value.at(1).find(L")");
									std::wstring value=name_and_value.at(1).substr(pos1,pos2-pos1);
									//Supported image formats by STB_Image
									wsregex isfilepath=wsregex::compile(L"([a-zA-Z]:[\\\\\/])?(?:[a-zA-Z0-9_]+[\\\\\/])*([a-zA-Z0-9_]+[\\.](JPG|jpg|JPEG|jpeg|PNG|png|BMP|bmp|TGA|tga|PSD|psd))");
									if(regex_match(value,matcher,isfilepath)){
										wprintf(std::wstring(std::wstring(L"Se usará el archivo ubicado en ")+value+std::wstring(L" para el nuevo PolyscriptBitmap ")+name_and_value.at(0)+std::wstring(L".\n")).c_str());
										PolyscriptBitmap bmp(value);
										variableBank.insert(std::pair<std::wstring,PolyscriptBitmap>(name_and_value.at(0),bmp));
									}//Si el parámetro que contiene bitmap es una ruta a una imagen
									else{
										wprintf(std::wstring(std::wstring(L"[Error de compilación] Tienes un error en la línea ")+std::to_wstring(i+1)+std::wstring(L": El valor ")+value+std::wstring (L" no es válido para PolyscriptBitmap.\n")).c_str());
										break;
									}//Si el parámetro que contiene bitmap no es válido
								}
							}//Encontró una variable de tipo PolyscriptBitmap
							else if(regex_match(name_and_value.at(1),matcher,gradientregex)){
								wprintf(L"Regex de gradient. \n");
								int pos1=name_and_value.at(1).find(L"(");
								if(pos1!=std::wstring::npos){
									pos1++;
									int pos2=name_and_value.at(1).find(L")");
									std::wstring value=name_and_value.at(1).substr(pos1,pos2-pos1);
									wsregex isgradientstop=wsregex::compile(L"([{].*[}]([,]?))+");
									std::wstring::const_iterator startPos=value.begin();
									if(regex_match(value,matcher,isgradientstop)){
										wprintf(std::wstring(std::wstring(L"Se usarán los gradientes ")+value+std::wstring(L" para el nuevo PolyscriptGradient ")+name_and_value.at(0)+std::wstring(L".\n")).c_str());
										std::vector<std::wstring>gradientStrings;
										boost::algorithm::split_regex(gradientStrings,value,boost::wregex(L"[}][\\,]"));
										std::vector<PolyscriptGradientStop> stops;
										for(std::wstring ws:gradientStrings){
											PolyscriptGradientStop gradientstop;
											while(ws.find(L"{")!=std::wstring::npos){
												ws.replace(ws.find(L"{"),std::wstring(L"{").size(),L"");
											}
											std::vector<std::wstring>gradientparams;
											boost::algorithm::split_regex(gradientparams,ws,boost::wregex(L";"));
											std::vector<std::wstring>colors;
											boost::algorithm::split_regex(colors,gradientparams.at(0),boost::wregex(L"[,]"));
											gradientstop.color=PolyscriptColor(_wtof(colors.at(0).c_str()),_wtof(colors.at(1).c_str()),_wtof(colors.at(2).c_str()),_wtof(colors.at(3).c_str()));
											gradientstop.positionX=_wtof(gradientparams.at(1).c_str());
											gradientstop.positionY=_wtof(gradientparams.at(2).c_str());
											stops.push_back(gradientstop);
										}
										PolyscriptGradient gradient(stops);
										variableBank.insert(std::pair<std::wstring,PolyscriptGradient>(name_and_value.at(0),gradient));
									}//Si el parámetro que contiene gradient es un grupo de 1 o más gradientes
									else{
										wprintf(std::wstring(std::wstring(L"[Error de compilación] Tienes un error en la línea ")+std::to_wstring(i+1)+std::wstring(L": El valor ")+value+std::wstring (L" no es válido para PolyscriptGradient.\n")).c_str());
										break;
									}//Si el parámetro que contiene gradient no es válido
								}
							}//Encontró una variable de tipo PolyscriptGradient
							else if(regex_match(name_and_value.at(1),matcher,isint)){
								wprintf(L"Regex de int. \n");
								int value=_wtoi(name_and_value.at(1).c_str());
								PolyscriptInt integer(value);
								variableBank.insert(std::pair<std::wstring,PolyscriptInt>(name_and_value.at(0),integer));
								int c=variableBank[name_and_value.at(0)].getDataInt();
							}//Encontró una variable de tipo PolyscriptInt
							else if(regex_match(name_and_value.at(1),matcher,isfloat)){
								wprintf(L"Regex de float. \n");
								float value=_wtof(name_and_value.at(1).c_str());
								PolyscriptFloat floating(value);
								variableBank.insert(std::pair<std::wstring,PolyscriptFloat>(name_and_value.at(0),floating));
								float c=variableBank[name_and_value.at(0)].getDataFloat();
							}//Encontró una variable de tipo PolyscriptFloat
							else if(variableBank.count(name_and_value.at(1))!=0){
								wprintf(std::wstring(std::wstring(L"Se encontró una variable existente en la línea ")+std::to_wstring(i+1)+std::wstring(L": ")+name_and_value.at(1)+std::wstring(L". Se asignará su valor a la variable ")+name_and_value.at(0)+std::wstring(L".\n")).c_str());
								PolyscriptVariable var=variableBank[name_and_value.at(1)];
								variableBank.insert(std::pair<std::wstring,PolyscriptVariable>(name_and_value.at(0),var));
							}//Si se asignó con una variable ya existente
							else{
								wprintf(std::wstring(std::wstring(L"[Error de compilación] Tienes un error en la línea ")+std::to_wstring(i+1)+std::wstring(L": La variable ")+name_and_value.at(0)+std::wstring (L" no es válida.\n")).c_str());
								break;
							}//Si no es una variable válida
						}//Encontró una posible etiqueta de variable
						else{
							wprintf(std::wstring(std::wstring(L"[Error de compilación] Tienes un error en la línea ")+std::to_wstring(i+1)+std::wstring(L": Instrucción desconocida.\n")).c_str());
							break;
						}//El compilador no reconoce la instrucción que se le asignó
					}//Si no se ha encontrado la etiqueta BeginPoint o ya encontró sus puntos (x,y)
				}//Iterar las líneas del archivo
			}//Asegurarse de que el archivo no se encontraba vacío
		}//El número de BeginPoint y EndPoint coinciden
		else{
			wprintf(L"[Error de compilación] Error: El número de BeginPoint y EndPoint no coinciden.\n");
		}//El número de BeginPoint y EndPoint no coinciden
	}//El número de BeginFigure y EndFigure coinciden
	else{
		wprintf(L"[Error de compilación] Error: El número de BeginFigure y EndFigure no coinciden.\n");
	}//El número de BeginFigure y EndFigure no coinciden
}