#include <string.h>
#include <stdio.h>
#include "cheetah.h"
#include "render.h"

//шейдры
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


void initRenderer()
{
	const char *vendor = (const char *)glGetString(GL_VENDOR);
	const char *exts = (const char *)glGetString(GL_EXTENSIONS);
	const char *renderer = (const char *)glGetString(GL_RENDERER);
	const char *version = (const char *)glGetString(GL_VERSION);
	printf("Renderer: %s (%s)\n", renderer, vendor);
	printf("Driver: %s\n", version);
	supported.GLSL = 0;
	//проверка поддержки шейдров
	if(strstr(exts, "GL_ARB_shading_language_100") && strstr(exts, "GL_ARB_shader_objects") && strstr(exts, "GL_ARB_vertex_shader") && strstr(exts, "GL_ARB_fragment_shader"))
	{
		glCreateProgramObject_ =        (PFNGLCREATEPROGRAMOBJECTARBPROC)     SDL_GL_GetProcAddress("glCreateProgramObjectARB");
		glDeleteObject_ =               (PFNGLDELETEOBJECTARBPROC)            SDL_GL_GetProcAddress("glDeleteObjectARB");
		glUseProgramObject_ =           (PFNGLUSEPROGRAMOBJECTARBPROC)        SDL_GL_GetProcAddress("glUseProgramObjectARB");
		glCreateShaderObject_ =         (PFNGLCREATESHADEROBJECTARBPROC)      SDL_GL_GetProcAddress("glCreateShaderObjectARB");
		glShaderSource_ =               (PFNGLSHADERSOURCEARBPROC)            SDL_GL_GetProcAddress("glShaderSourceARB");
		glCompileShader_ =              (PFNGLCOMPILESHADERARBPROC)           SDL_GL_GetProcAddress("glCompileShaderARB");
		glGetObjectParameteriv_ =       (PFNGLGETOBJECTPARAMETERIVARBPROC)    SDL_GL_GetProcAddress("glGetObjectParameterivARB");
		glAttachObject_ =               (PFNGLATTACHOBJECTARBPROC)            SDL_GL_GetProcAddress("glAttachObjectARB");
		glGetInfoLog_ =                 (PFNGLGETINFOLOGARBPROC)              SDL_GL_GetProcAddress("glGetInfoLogARB");
		glLinkProgram_ =                (PFNGLLINKPROGRAMARBPROC)             SDL_GL_GetProcAddress("glLinkProgramARB");
		glGetUniformLocation_ =         (PFNGLGETUNIFORMLOCATIONARBPROC)      SDL_GL_GetProcAddress("glGetUniformLocationARB");
		glUniform1f_ =                  (PFNGLUNIFORM1FARBPROC)               SDL_GL_GetProcAddress("glUniform1fARB");
		glUniform2f_ =                  (PFNGLUNIFORM2FARBPROC)               SDL_GL_GetProcAddress("glUniform2fARB");
		glUniform3f_ =                  (PFNGLUNIFORM3FARBPROC)               SDL_GL_GetProcAddress("glUniform3fARB");
		glUniform4f_ =                  (PFNGLUNIFORM4FARBPROC)               SDL_GL_GetProcAddress("glUniform4fARB");
		glUniform1fv_ =                 (PFNGLUNIFORM1FVARBPROC)              SDL_GL_GetProcAddress("glUniform1fvARB");
		glUniform2fv_ =                 (PFNGLUNIFORM2FVARBPROC)              SDL_GL_GetProcAddress("glUniform2fvARB");
		glUniform3fv_ =                 (PFNGLUNIFORM3FVARBPROC)              SDL_GL_GetProcAddress("glUniform3fvARB");
		glUniform4fv_ =                 (PFNGLUNIFORM4FVARBPROC)              SDL_GL_GetProcAddress("glUniform4fvARB");
		glUniform1i_ =                  (PFNGLUNIFORM1IARBPROC)               SDL_GL_GetProcAddress("glUniform1iARB");
		glBindAttribLocation_ =         (PFNGLBINDATTRIBLOCATIONARBPROC)      SDL_GL_GetProcAddress("glBindAttribLocationARB");
		glGetActiveUniform_ =           (PFNGLGETACTIVEUNIFORMARBPROC)        SDL_GL_GetProcAddress("glGetActiveUniformARB");
		glGetShaderiv_ =                (PFNGLGETSHADERIVPROC)                SDL_GL_GetProcAddress("glGetShaderiv");
		glGetProgramiv_ =               (PFNGLGETPROGRAMIVPROC)               SDL_GL_GetProcAddress("glGetProgramiv");
		supported.GLSL = 1;
	}
	
	supported.BE = 0;
	if(strstr(exts, "GL_EXT_blend_minmax"))
	{
		glBlendEquation_ = (PFNGLBLENDEQUATIONEXTPROC) SDL_GL_GetProcAddress("glBlendEquationEXT");
		supported.BE = 1;
		//~ if(strstr(vendor, "ATI")) ati_minmax_bug = 1;
		//~ if(dbgexts) conoutf(CON_INIT, "Using GL_EXT_blend_minmax extension.");
	}
	
	supported.FBO = 0;
	if(strstr(exts, "GL_EXT_framebuffer_object"))
	{
		glBindRenderbuffer_        = (PFNGLBINDRENDERBUFFEREXTPROC)       SDL_GL_GetProcAddress("glBindRenderbufferEXT");
		glDeleteRenderbuffers_     = (PFNGLDELETERENDERBUFFERSEXTPROC)    SDL_GL_GetProcAddress("glDeleteRenderbuffersEXT");
		glGenRenderbuffers_        = (PFNGLGENFRAMEBUFFERSEXTPROC)        SDL_GL_GetProcAddress("glGenRenderbuffersEXT");
		glRenderbufferStorage_     = (PFNGLRENDERBUFFERSTORAGEEXTPROC)    SDL_GL_GetProcAddress("glRenderbufferStorageEXT");
		glCheckFramebufferStatus_  = (PFNGLCHECKFRAMEBUFFERSTATUSEXTPROC) SDL_GL_GetProcAddress("glCheckFramebufferStatusEXT");
		glBindFramebuffer_         = (PFNGLBINDFRAMEBUFFEREXTPROC)        SDL_GL_GetProcAddress("glBindFramebufferEXT");
		glDeleteFramebuffers_      = (PFNGLDELETEFRAMEBUFFERSEXTPROC)     SDL_GL_GetProcAddress("glDeleteFramebuffersEXT");
		glGenFramebuffers_         = (PFNGLGENFRAMEBUFFERSEXTPROC)        SDL_GL_GetProcAddress("glGenFramebuffersEXT");
		glFramebufferTexture2D_    = (PFNGLFRAMEBUFFERTEXTURE2DEXTPROC)   SDL_GL_GetProcAddress("glFramebufferTexture2DEXT");
		glFramebufferRenderbuffer_ = (PFNGLFRAMEBUFFERRENDERBUFFEREXTPROC)SDL_GL_GetProcAddress("glFramebufferRenderbufferEXT");
		glGenerateMipmap_          = (PFNGLGENERATEMIPMAPEXTPROC)         SDL_GL_GetProcAddress("glGenerateMipmapEXT");
		supported.FBO = 1;
		//~ if(dbgexts) conoutf(CON_INIT, "Using GL_EXT_framebuffer_object extension.");
		//~ if(strstr(exts, "GL_EXT_framebuffer_blit"))
		//~ {
				//~ glBlitFramebuffer_     = (PFNGLBLITFRAMEBUFFEREXTPROC)        SDL_GL_GetProcAddress("glBlitFramebufferEXT");
				//~ hasFBB = true;
				//~ if(dbgexts) conoutf(CON_INIT, "Using GL_EXT_framebuffer_blit extension.");
		//~ }
	}
	supported.MT = 0;
	if(strstr(exts, "GL_ARB_multitexture"))
	{
			glActiveTexture_       = (PFNGLACTIVETEXTUREARBPROC)      SDL_GL_GetProcAddress("glActiveTextureARB");
			glClientActiveTexture_ = (PFNGLCLIENTACTIVETEXTUREARBPROC)SDL_GL_GetProcAddress("glClientActiveTextureARB");
			glMultiTexCoord2f_     = (PFNGLMULTITEXCOORD2FARBPROC)    SDL_GL_GetProcAddress("glMultiTexCoord2fARB");
			glMultiTexCoord3f_     = (PFNGLMULTITEXCOORD3FARBPROC)    SDL_GL_GetProcAddress("glMultiTexCoord3fARB");
			glMultiTexCoord4f_     = (PFNGLMULTITEXCOORD4FARBPROC)    SDL_GL_GetProcAddress("glMultiTexCoord4fARB");
			supported.MT = 1;
	}
	supported.VBO = 0;
	if(strstr(exts, "GL_ARB_vertex_buffer_object"))
	{
		glGenBuffers_       = (PFNGLGENBUFFERSARBPROC)      SDL_GL_GetProcAddress("glGenBuffersARB");
		glBindBuffer_       = (PFNGLBINDBUFFERARBPROC)      SDL_GL_GetProcAddress("glBindBufferARB");
		glMapBuffer_        = (PFNGLMAPBUFFERARBPROC)       SDL_GL_GetProcAddress("glMapBufferARB");
		glUnmapBuffer_      = (PFNGLUNMAPBUFFERARBPROC)     SDL_GL_GetProcAddress("glUnmapBufferARB");
		glBufferData_       = (PFNGLBUFFERDATAARBPROC)      SDL_GL_GetProcAddress("glBufferDataARB");
		glBufferSubData_    = (PFNGLBUFFERSUBDATAARBPROC)   SDL_GL_GetProcAddress("glBufferSubDataARB");
		glDeleteBuffers_    = (PFNGLDELETEBUFFERSARBPROC)   SDL_GL_GetProcAddress("glDeleteBuffersARB");
		glGetBufferSubData_ = (PFNGLGETBUFFERSUBDATAARBPROC)SDL_GL_GetProcAddress("glGetBufferSubDataARB");
		supported.VBO = 1;
	}
}