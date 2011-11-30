#include "cheetah.h"
#include "render.h"

const char * std_vertex_shader = "void main()\
{\
	gl_Position    = gl_ModelViewProjectionMatrix * gl_Vertex;\
	gl_TexCoord[0] = gl_MultiTexCoord0;\
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

int compile(GLuint shader, const char* name)
{
	GLint compiled;
	GLint blen = 0;
	GLsizei slen = 0;
	GLchar* compiler_log;
	
	glCompileShader_(shader);

	glGetShaderiv_(shader, GL_COMPILE_STATUS, &compiled);
	if (!compiled)
	{
		glGetShaderiv_(shader, GL_INFO_LOG_LENGTH , &blen);       
		if (blen > 1)
		{
			compiler_log = (GLchar*)malloc(blen);
			glGetInfoLog_(shader, blen, &slen, compiler_log);
			myError("Error while compiling shader %s: %s\n", name, compiler_log);
			free (compiler_log);
		}
		return 0;
	}
	return 1;
}
Shader * newPixelShader(const char * str) {
	GLuint v, f, p, linked;
	if(!screen)
	{
		myError("Call init function before!");
		return 0;
	}
	if(!supported.GLSL)
	{
		myError("Trying to create shader, but system doesn't support shaders.");
		return 0;
	}
	v = glCreateShaderObject_(GL_VERTEX_SHADER);
	f = glCreateShaderObject_(GL_FRAGMENT_SHADER);
	glShaderSource_(v, 1, &std_vertex_shader, NULL);
	glShaderSource_(f, 1, &str, NULL);
	if(!compile(v, "string_shader"))
		return 0;
	if(!compile(f, "string_shader"))
	{
		glDeleteObject_(v);
		return 0;
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
		return 0;
	}
	glDeleteObject_(v);
	glDeleteObject_(f);
	Shader *ptr = new(Shader);
	ptr->id = p;
	return ptr;
}

void deleteShader(Shader * ptr) {
	GLuint p;
	if(ptr) {
		if(supported.GLSL && ptr->id)
			glDeleteObject_(ptr->id);
	}
	else myError("Trying to free a null-shader. Maybe, you did it manually?");
}

void useShader(Shader * ptr) {
	if(supported.GLSL)
		glUseProgramObject_(ptr->id);
}

void disableShader() {
	if(supported.GLSL)
		glUseProgramObject_(0);
}

unsigned int GetUniformLocation(unsigned int program, const char * name) {
	return glGetUniformLocation_(program, name);
}
void UseProgramObject(unsigned int program) {
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

