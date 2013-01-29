/*******************************************************************************

Copyright (c) 2012-2013 Pavel Roschin (aka RPG) <rpg89@post.ru>

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

#include <string.h>
#include <stdio.h>
#include <SDL.h>

#include "cheetah.h"
#include "render.h"

//Shaders
// GL_ARB_shading_language_100, GL_ARB_shader_objects, GL_ARB_fragment_shader, GL_ARB_vertex_shader
PFNGLCREATEPROGRAMOBJECTARBPROC       glCreateProgramObject_      = NULL;
PFNGLDELETEOBJECTARBPROC              glDeleteObject_             = NULL;
PFNGLUSEPROGRAMOBJECTARBPROC          glUseProgramObject_         = NULL; 
PFNGLCREATESHADEROBJECTARBPROC        glCreateShaderObject_       = NULL;
PFNGLSHADERSOURCEARBPROC              glShaderSource_             = NULL;
PFNGLCOMPILESHADERARBPROC             glCompileShader_            = NULL;
PFNGLGETOBJECTPARAMETERIVARBPROC      glGetObjectParameteriv_     = NULL;
PFNGLATTACHOBJECTARBPROC              glAttachObject_             = NULL;
PFNGLGETINFOLOGARBPROC                glGetInfoLog_               = NULL;
PFNGLLINKPROGRAMARBPROC               glLinkProgram_              = NULL;
PFNGLGETUNIFORMLOCATIONARBPROC        glGetUniformLocation_       = NULL;
PFNGLUNIFORM1FARBPROC                 glUniform1f_                = NULL;
PFNGLUNIFORM2FARBPROC                 glUniform2f_                = NULL;
PFNGLUNIFORM3FARBPROC                 glUniform3f_                = NULL;
PFNGLUNIFORM4FARBPROC                 glUniform4f_                = NULL;
PFNGLUNIFORM1FVARBPROC                glUniform1fv_               = NULL;
PFNGLUNIFORM2FVARBPROC                glUniform2fv_               = NULL;
PFNGLUNIFORM3FVARBPROC                glUniform3fv_               = NULL;
PFNGLUNIFORM4FVARBPROC                glUniform4fv_               = NULL;
PFNGLUNIFORM1IARBPROC                 glUniform1i_                = NULL;
PFNGLBINDATTRIBLOCATIONARBPROC        glBindAttribLocation_       = NULL;
PFNGLGETACTIVEUNIFORMARBPROC          glGetActiveUniform_         = NULL;
PFNGLGETSHADERIVPROC                  glGetShaderiv_              = NULL;
PFNGLGETPROGRAMIVPROC                 glGetProgramiv_             = NULL;

// GL_EXT_blend_minmax
PFNGLBLENDEQUATIONEXTPROC glBlendEquation_ = NULL;

// GL_EXT_framebuffer_object
PFNGLBINDRENDERBUFFEREXTPROC        glBindRenderbuffer_        = NULL;
PFNGLDELETERENDERBUFFERSEXTPROC     glDeleteRenderbuffers_     = NULL;
PFNGLGENFRAMEBUFFERSEXTPROC         glGenRenderbuffers_        = NULL;
PFNGLRENDERBUFFERSTORAGEEXTPROC     glRenderbufferStorage_     = NULL;
PFNGLCHECKFRAMEBUFFERSTATUSEXTPROC  glCheckFramebufferStatus_  = NULL;
PFNGLBINDFRAMEBUFFEREXTPROC         glBindFramebuffer_         = NULL;
PFNGLDELETEFRAMEBUFFERSEXTPROC      glDeleteFramebuffers_      = NULL;
PFNGLGENFRAMEBUFFERSEXTPROC         glGenFramebuffers_         = NULL;
PFNGLFRAMEBUFFERTEXTURE2DEXTPROC    glFramebufferTexture2D_    = NULL;
PFNGLFRAMEBUFFERRENDERBUFFEREXTPROC glFramebufferRenderbuffer_ = NULL;
PFNGLGENERATEMIPMAPEXTPROC          glGenerateMipmap_          = NULL;

// GL_ARB_multitexture
PFNGLACTIVETEXTUREARBPROC       glActiveTexture_       = NULL;
PFNGLCLIENTACTIVETEXTUREARBPROC glClientActiveTexture_ = NULL;
PFNGLMULTITEXCOORD2FARBPROC     glMultiTexCoord2f_     = NULL;
PFNGLMULTITEXCOORD3FARBPROC     glMultiTexCoord3f_     = NULL;
PFNGLMULTITEXCOORD4FARBPROC     glMultiTexCoord4f_     = NULL;

// GL_ARB_vertex_buffer_object, GL_ARB_pixel_buffer_object
PFNGLGENBUFFERSARBPROC       glGenBuffers_       = NULL;
PFNGLBINDBUFFERARBPROC       glBindBuffer_       = NULL;
PFNGLMAPBUFFERARBPROC        glMapBuffer_        = NULL;
PFNGLUNMAPBUFFERARBPROC      glUnmapBuffer_      = NULL;
PFNGLBUFFERDATAARBPROC       glBufferData_       = NULL;
PFNGLBUFFERSUBDATAARBPROC    glBufferSubData_    = NULL;
PFNGLDELETEBUFFERSARBPROC    glDeleteBuffers_    = NULL;
PFNGLGETBUFFERSUBDATAARBPROC glGetBufferSubData_ = NULL;

// GL_EXT_framebuffer_blit
PFNGLBLITFRAMEBUFFEREXTPROC         glBlitFramebuffer_         = NULL;

#define getProcAddr SDL_GL_GetProcAddress

void initRenderer()
{
	const char *vendor = (const char *)glGetString(GL_VENDOR);
	const char *exts = (const char *)glGetString(GL_EXTENSIONS);
	const char *renderer = (const char *)glGetString(GL_RENDERER);
	const char *version = (const char *)glGetString(GL_VERSION);
	printf("Renderer: %s (%s)\n", renderer, vendor);
	printf("Driver: %s\n", version);
	
	printf("Checking extensions...\n");
	printf("Shaders:\t");
	supported.GLSL = 0;
	//проверка поддержки шейдров
	if(strstr(exts, "GL_ARB_shading_language_100") && strstr(exts, "GL_ARB_shader_objects") && strstr(exts, "GL_ARB_vertex_shader") && strstr(exts, "GL_ARB_fragment_shader"))
	{
		glCreateProgramObject_ =        (PFNGLCREATEPROGRAMOBJECTARBPROC)     getProcAddr("glCreateProgramObjectARB");
		glDeleteObject_ =               (PFNGLDELETEOBJECTARBPROC)            getProcAddr("glDeleteObjectARB");
		glUseProgramObject_ =           (PFNGLUSEPROGRAMOBJECTARBPROC)        getProcAddr("glUseProgramObjectARB");
		glCreateShaderObject_ =         (PFNGLCREATESHADEROBJECTARBPROC)      getProcAddr("glCreateShaderObjectARB");
		glShaderSource_ =               (PFNGLSHADERSOURCEARBPROC)            getProcAddr("glShaderSourceARB");
		glCompileShader_ =              (PFNGLCOMPILESHADERARBPROC)           getProcAddr("glCompileShaderARB");
		glGetObjectParameteriv_ =       (PFNGLGETOBJECTPARAMETERIVARBPROC)    getProcAddr("glGetObjectParameterivARB");
		glAttachObject_ =               (PFNGLATTACHOBJECTARBPROC)            getProcAddr("glAttachObjectARB");
		glGetInfoLog_ =                 (PFNGLGETINFOLOGARBPROC)              getProcAddr("glGetInfoLogARB");
		glLinkProgram_ =                (PFNGLLINKPROGRAMARBPROC)             getProcAddr("glLinkProgramARB");
		glGetUniformLocation_ =         (PFNGLGETUNIFORMLOCATIONARBPROC)      getProcAddr("glGetUniformLocationARB");
		glUniform1f_ =                  (PFNGLUNIFORM1FARBPROC)               getProcAddr("glUniform1fARB");
		glUniform2f_ =                  (PFNGLUNIFORM2FARBPROC)               getProcAddr("glUniform2fARB");
		glUniform3f_ =                  (PFNGLUNIFORM3FARBPROC)               getProcAddr("glUniform3fARB");
		glUniform4f_ =                  (PFNGLUNIFORM4FARBPROC)               getProcAddr("glUniform4fARB");
		glUniform1fv_ =                 (PFNGLUNIFORM1FVARBPROC)              getProcAddr("glUniform1fvARB");
		glUniform2fv_ =                 (PFNGLUNIFORM2FVARBPROC)              getProcAddr("glUniform2fvARB");
		glUniform3fv_ =                 (PFNGLUNIFORM3FVARBPROC)              getProcAddr("glUniform3fvARB");
		glUniform4fv_ =                 (PFNGLUNIFORM4FVARBPROC)              getProcAddr("glUniform4fvARB");
		glUniform1i_ =                  (PFNGLUNIFORM1IARBPROC)               getProcAddr("glUniform1iARB");
		glBindAttribLocation_ =         (PFNGLBINDATTRIBLOCATIONARBPROC)      getProcAddr("glBindAttribLocationARB");
		glGetActiveUniform_ =           (PFNGLGETACTIVEUNIFORMARBPROC)        getProcAddr("glGetActiveUniformARB");
		glGetShaderiv_ =                (PFNGLGETSHADERIVPROC)                getProcAddr("glGetShaderiv");
		glGetProgramiv_ =               (PFNGLGETPROGRAMIVPROC)               getProcAddr("glGetProgramiv");
		supported.GLSL = 1;
		printf("yes\n");
	}
	else printf("no\n");
	
	printf("Minmax blend:\t");
	supported.BE = 0;
	if(strstr(exts, "GL_EXT_blend_minmax"))
	{
		glBlendEquation_ = (PFNGLBLENDEQUATIONEXTPROC) getProcAddr("glBlendEquationEXT");
		supported.BE = 1;
		//~ if(strstr(vendor, "ATI")) ati_minmax_bug = 1;
		//~ if(dbgexts) conoutf(CON_INIT, "Using GL_EXT_blend_minmax extension.");
		printf("yes\n");
	}
	else printf("no\n");
	
	printf("Framebuffer:\t");
	supported.FBO = 0;
	if(strstr(exts, "GL_EXT_framebuffer_object"))
	{
		glBindRenderbuffer_        = (PFNGLBINDRENDERBUFFEREXTPROC)       getProcAddr("glBindRenderbufferEXT");
		glDeleteRenderbuffers_     = (PFNGLDELETERENDERBUFFERSEXTPROC)    getProcAddr("glDeleteRenderbuffersEXT");
		glGenRenderbuffers_        = (PFNGLGENFRAMEBUFFERSEXTPROC)        getProcAddr("glGenRenderbuffersEXT");
		glRenderbufferStorage_     = (PFNGLRENDERBUFFERSTORAGEEXTPROC)    getProcAddr("glRenderbufferStorageEXT");
		glCheckFramebufferStatus_  = (PFNGLCHECKFRAMEBUFFERSTATUSEXTPROC) getProcAddr("glCheckFramebufferStatusEXT");
		glBindFramebuffer_         = (PFNGLBINDFRAMEBUFFEREXTPROC)        getProcAddr("glBindFramebufferEXT");
		glDeleteFramebuffers_      = (PFNGLDELETEFRAMEBUFFERSEXTPROC)     getProcAddr("glDeleteFramebuffersEXT");
		glGenFramebuffers_         = (PFNGLGENFRAMEBUFFERSEXTPROC)        getProcAddr("glGenFramebuffersEXT");
		glFramebufferTexture2D_    = (PFNGLFRAMEBUFFERTEXTURE2DEXTPROC)   getProcAddr("glFramebufferTexture2DEXT");
		glFramebufferRenderbuffer_ = (PFNGLFRAMEBUFFERRENDERBUFFEREXTPROC)getProcAddr("glFramebufferRenderbufferEXT");
		glGenerateMipmap_          = (PFNGLGENERATEMIPMAPEXTPROC)         getProcAddr("glGenerateMipmapEXT");
		supported.FBO = 1;
		printf("yes\n");
	}
	else printf("no\n");
	
	printf("Multitexture:\t");
	supported.MT = 0;
	if(strstr(exts, "GL_ARB_multitexture"))
	{
		glActiveTexture_       = (PFNGLACTIVETEXTUREARBPROC)      getProcAddr("glActiveTextureARB");
		glClientActiveTexture_ = (PFNGLCLIENTACTIVETEXTUREARBPROC)getProcAddr("glClientActiveTextureARB");
		glMultiTexCoord2f_     = (PFNGLMULTITEXCOORD2FARBPROC)    getProcAddr("glMultiTexCoord2fARB");
		glMultiTexCoord3f_     = (PFNGLMULTITEXCOORD3FARBPROC)    getProcAddr("glMultiTexCoord3fARB");
		glMultiTexCoord4f_     = (PFNGLMULTITEXCOORD4FARBPROC)    getProcAddr("glMultiTexCoord4fARB");
		supported.MT = 1;
		printf("yes\n");
	}
	else printf("no\n");
	
	printf("Vertex Buffer:\t");
	supported.VBO = 0;
	if(strstr(exts, "GL_ARB_vertex_buffer_object"))
	{
		glGenBuffers_       = (PFNGLGENBUFFERSARBPROC)      getProcAddr("glGenBuffersARB");
		glBindBuffer_       = (PFNGLBINDBUFFERARBPROC)      getProcAddr("glBindBufferARB");
		glMapBuffer_        = (PFNGLMAPBUFFERARBPROC)       getProcAddr("glMapBufferARB");
		glUnmapBuffer_      = (PFNGLUNMAPBUFFERARBPROC)     getProcAddr("glUnmapBufferARB");
		glBufferData_       = (PFNGLBUFFERDATAARBPROC)      getProcAddr("glBufferDataARB");
		glBufferSubData_    = (PFNGLBUFFERSUBDATAARBPROC)   getProcAddr("glBufferSubDataARB");
		glDeleteBuffers_    = (PFNGLDELETEBUFFERSARBPROC)   getProcAddr("glDeleteBuffersARB");
		glGetBufferSubData_ = (PFNGLGETBUFFERSUBDATAARBPROC)getProcAddr("glGetBufferSubDataARB");
		supported.VBO = 1;
		printf("yes\n");
	}
	else printf("no\n");
	
	printf("Point Sprite:\t");
	supported.PS = 0;
	if(strstr(exts, "GL_ARB_point_sprite"))
	{
		supported.PS = 1;
		printf("yes\n");
	}
	else printf("no\n");
}