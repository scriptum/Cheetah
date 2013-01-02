/*******************************************************************************

Copyright (c) 2012 Pavel Roschin (aka RPG) <rpg89@post.ru>

Permission is hereby granted, free of charge, to any person obtaining a copy 
of this software and associated documentation files (the "Software"), to 
deal in the Software without restriction, including without limitation the 
rights to use, copy, modify, merge, publish, distribute, sublicense, and/or 
sell copies of the Software, and to permit persons to whom the Software is 
furnished to do so, subject to the following conditions:  The above 
copyright notice and this permission notice shall be included in all copies 
or substantial portions of the Software.
 
THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR 
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, 
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE 
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER 
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING 
FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS 
IN THE SOFTWARE.

*******************************************************************************/

#include "cheetah.h"
#include "render.h"

const char * std_vertex_shader = "varying vec2 TexCoord;\
void main()\
{\
	gl_Position = gl_ModelViewProjectionMatrix * gl_Vertex;\
	TexCoord = gl_MultiTexCoord0.xy;\
}";

//~ char * loadfile(const char * name, unsigned int * length)
//~ {
	//~ char * data;
	//~ unsigned int len;
	//~ PHYSFS_file* myfile = PHYSFS_openRead(name);
	//~ if (!myfile)
		//~ return 0;
	//~ len = PHYSFS_fileLength(myfile);
	//~ *length = len;
	//~ data = (char *)malloc(sizeof(char) * len + 1);
	//~ PHYSFS_read (myfile, data, sizeof(char), len);
	//~ PHYSFS_close(myfile);
	//~ data[len] = 0;
	//~ return data;
//~ }

static int compile(GLuint shader, const char* name)
{
	GLint compiled;
	GLint blen = 0;
	GLsizei slen = 0;
	GLchar *compiler_log = NULL;
	
	glCompileShader_(shader);

	glGetShaderiv_(shader, GL_COMPILE_STATUS, &compiled);
	if (!compiled)
	{
		glGetShaderiv_(shader, GL_INFO_LOG_LENGTH , &blen);       
		if (blen > 1)
		{
			new(compiler_log, GLchar, blen);
			glGetInfoLog_(shader, blen, &slen, compiler_log);
			myError("Error while compiling shader %s: %s\n", name, compiler_log);
			delete(compiler_log);
		}
		return 0;
	}
	return 1;
}

void newFragmentVertexShader(Shader * ptr, const char * pix, const char * ver) {
	GLuint v, f, p;
	GLint linked;
	if(!screen)
	{
		myError("Call init function before!");
		return;
	}
	//~ new(ptr, Shader, 1);
	ptr->id = 0;
	if(!supported.GLSL)
	{
		myError("Trying to create shader, but system doesn't support shaders.");
		return;
	}
	v = glCreateShaderObject_(GL_VERTEX_SHADER);
	f = glCreateShaderObject_(GL_FRAGMENT_SHADER);
	glShaderSource_(v, 1, &ver, NULL);
	glShaderSource_(f, 1, &pix, NULL);
	if(!compile(v, "string_shader"))
		return;
	if(!compile(f, "string_shader"))
	{
		glDeleteObject_(v);
		return;
	}
	p = glCreateProgramObject_();
	glAttachObject_(p,v);
	glAttachObject_(p,f);
	glLinkProgram_(p);
  
	glGetProgramiv_(p, GL_LINK_STATUS, &linked);
	if (!linked)
	{
		myError("Error while linking shader\n");
		glDeleteObject_(v);
		glDeleteObject_(f);
		return;
	}
	glDeleteObject_(v);
	glDeleteObject_(f);
	ptr->id = p;
}

void newFragmentShader(Shader * ptr, const char * frag) {
	newFragmentVertexShader(ptr, frag, std_vertex_shader);
}

bool shaderCheck(Shader * ptr) {
	return ptr->id;
}

void deleteShader(Shader * ptr) {
	if(ptr) {
		if(supported.GLSL && ptr->id)
			glDeleteObject_(ptr->id);
	}
	else myError("Trying to free a null-shader. Maybe, you did it manually?");
}

void shaderBind(Shader * ptr) {
	if(supported.GLSL && ptr->id)
	{
		FLUSH_BUFFER();
		glUseProgramObject_(ptr->id);
	}
}

void shaderUnbind(Shader * ptr) {
	if(supported.GLSL)
	{
		FLUSH_BUFFER();
		glUseProgramObject_(0);
	}
}

unsigned int GetUniformLocation(unsigned int program, const char * name) {
	return glGetUniformLocation_(program, name);
}
void UseProgramObject(unsigned int program) {
	FLUSH_BUFFER();
	glUseProgramObject_(program);
}
void Uniform1i(unsigned int location, int var) {
	glUniform1i_(location, var);
}
void Uniform1f(unsigned int location, float var) {
	glUniform1f_(location, var);
}
void Uniform2f(unsigned int location, float var, float var1) {
	glUniform2f_(location, var, var1);
}
void Uniform3f(unsigned int location, float var, float var1, float var2) {
	glUniform3f_(location, var, var1, var2);
}
void Uniform4f(unsigned int location, float var, float var1, float var2, float var3) {
	glUniform4f_(location, var, var1, var2, var3);
}
