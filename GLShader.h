#ifndef GLSHADER_INCLUDED
#define GLSHADER_INCLUDED
#include "stdafx.h"
class GLShader{
public:
	unsigned int ID;
	GLShader(){}
	GLShader(const char* vertexPath,const char* fragPath){
		std::string vertexCode;
		std::string fragmentCode;
		std::ifstream vShaderFile;
		std::ifstream fShaderFile;
		vShaderFile.exceptions(std::ifstream::failbit|std::ifstream::badbit);
		fShaderFile.exceptions(std::ifstream::failbit|std::ifstream::badbit);
		try{
			vShaderFile.open(vertexPath);
			fShaderFile.open(fragPath);
			std::stringstream vShaderStream,fShaderStream;
			vShaderStream<<vShaderFile.rdbuf();
			fShaderStream<<fShaderFile.rdbuf();
			vShaderFile.close();
			fShaderFile.close();
			vertexCode=vShaderStream.str();
			fragmentCode=fShaderStream.str();
		}catch(std::ifstream::failure ex){
			MessageBoxA(NULL,ex.what(),"Error ifstream",MB_OK);
		}
		const char* vShaderCode=vertexCode.c_str();
		const char* fShaderCode=fragmentCode.c_str();
		unsigned int vertex,fragment;
		int success;
		char infoLog[512];
		vertex=glCreateShader(GL_VERTEX_SHADER);
		glShaderSource(vertex,1,&vShaderCode,NULL);
		glCompileShader(vertex);
		glGetShaderiv(vertex,GL_COMPILE_STATUS,&success);
		if(!success){
			glGetShaderInfoLog(vertex,512,NULL,infoLog);
			std::string err(std::string("ERROR::SHADER::VERTEX::COMPILATION_FAILED\n")+std::string(infoLog));
			MessageBoxA(NULL,err.c_str(),"Error Vertex Shader",MB_OK);
		}
		fragment=glCreateShader(GL_FRAGMENT_SHADER);
		glShaderSource(fragment,1,&fShaderCode,NULL);
		glCompileShader(fragment);
		glGetShaderiv(fragment,GL_COMPILE_STATUS,&success);
		if(!success){
			glGetShaderInfoLog(fragment,512,NULL,infoLog);
			std::string err(std::string("ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n")+std::string(infoLog));
			MessageBoxA(NULL,err.c_str(),"Error Fragment Shader",MB_OK);
		}
		ID=glCreateProgram();
		glAttachShader(ID,vertex);
		glAttachShader(ID,fragment);
		glLinkProgram(ID);
		glGetProgramiv(ID,GL_LINK_STATUS,&success);
		if(!success){
			glGetProgramInfoLog(ID,512,NULL,infoLog);
			std::string err(std::string("ERROR::SHADER::PROGRAM::LINKING_FAILED\n")+std::string(infoLog));
			MessageBoxA(NULL,err.c_str(),"Error Shader Program",MB_OK);
		}
		glDeleteShader(vertex);
		glDeleteShader(fragment);
	}
	void use(){
		glUseProgram(ID);
	}
	void setBool(const std::string& name,bool value)const{
		glUniform1i(glGetUniformLocation(ID,name.c_str()),(int)value);
	}
	void setInt(const std::string&name,int value)const{
		glUniform1i(glGetUniformLocation(ID,name.c_str()),value);
	}
	void setFloat(const std::string&name,float value)const{
		glUniform1f(glGetUniformLocation(ID,name.c_str()),value);
	}
	void setMatrix4f(const std::string&name,float* value){
		glUniform4fv(glGetUniformLocation(ID,name.c_str()),1,value);
	}
};
#endif