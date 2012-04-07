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


static int checkFramebufferStatus()
{
	GLenum status;
	status = (GLenum) glCheckFramebufferStatus_(GL_FRAMEBUFFER_EXT);
	switch(status) {
		case GL_FRAMEBUFFER_COMPLETE_EXT:
			return 1;
		case GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT_EXT:
			myError("Framebuffer incomplete, incomplete attachment\n");
			return 0;
		case GL_FRAMEBUFFER_UNSUPPORTED_EXT:
			myError("Unsupported framebuffer format\n");
			return 0;
		case GL_FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT_EXT:
			myError("Framebuffer incomplete, missing attachment\n");
			return 0;
		case GL_FRAMEBUFFER_INCOMPLETE_DIMENSIONS_EXT:
			myError("Framebuffer incomplete, attached images must have same dimensions\n");
			return 0;
		case GL_FRAMEBUFFER_INCOMPLETE_FORMATS_EXT:
			myError("Framebuffer incomplete, attached images must have same format\n");
			return 0;
		case GL_FRAMEBUFFER_INCOMPLETE_DRAW_BUFFER_EXT:
			myError("Framebuffer incomplete, missing draw buffer\n");
			return 0;
		case GL_FRAMEBUFFER_INCOMPLETE_READ_BUFFER_EXT:
			myError("Framebuffer incomplete, missing read buffer\n");
			return 0;
	}
	return 0;
}

/**
 * @descr Create framebuffer object. Note, that not all video drivers support this. It's recommend to check returning value using cheetah.inPointer and check framebuffer support using cheetah.supported.FBO.
 * @group graphics/framebuffer
 * @var width
 * @var height
 * @var string of options. Supported options:
 *  * _a_ - enable alpha channel
 *  * _n_ - enable smooth interpolation
 *  * _r_ - repeat as texture (not all faramebuffers need this)
 *  * _1_ - create 8 bits (1 byte) per channel framebuffer (default)
 *  * _2_ - create 16 bits (2 byte) per channel framebuffer (slow), not all systems support this
 *  * _4_ - create 32 bits (4 byte) per channel framebuffer (very SLOW), use only if you know, that you doing, not all systems support this
 * @return Framebuffer object
 * */
void newFramebufferOpt(Framebuffer *fboptr, unsigned int width, unsigned int height, const char * options) {
	//unsigned int percision, bool alpha, bool interpolation, bool repeat) {
	unsigned int percision = 8;
	bool alpha = 0;
	bool interpolation = 1;
	bool repeat = 0;
	Image *ptr = NULL;
	GLint current_fbo;
	GLenum internal, format;
	bool status;
	char ch;
	
	fboptr->id = 0;
	
	if(!screen)
	{
		myError("framebuffer: call init function before!");
		return;
	}
	
	if(!supported.FBO) {
		myError("Framebuffers are not supported on this machine. You'd better to check it in script (if cheetah.supported.FBO)");
		return;
	}
	
	while(*options)
	{
		ch = *options;
		options++;
		if(ch == 'a') {alpha = 1; continue;}
		if(ch == 'n') {interpolation = 0; continue;}
		if(ch == 'r') {repeat = 1; continue;}
		if(ch == '4') {percision = 32; continue;}
		if(ch == '2') {percision = 16; continue;}
		if(ch == '1') {percision = 8; continue;}
	}
	
	if(percision == 32) {
		internal = alpha ? GL_RGBA32F_ARB : GL_RGB32F_ARB;
		format = GL_FLOAT;
	}
	else if(percision == 16) {
		internal = alpha ? GL_RGBA16F_ARB : GL_RGB16F_ARB;
		format = GL_HALF_FLOAT_ARB;
	}
	else {
		//~ if(percision != 8) myError("Invalid parameter in framebuffer's percision (8 expected, got %d). Using 8bit framebuffer.", percision);
		internal = alpha ? GL_RGBA : GL_RGB;
		format = GL_UNSIGNED_BYTE;
	}
	
	//save current fbo
	glGetIntegerv(GL_DRAW_FRAMEBUFFER_BINDING_EXT, &current_fbo);
	
	new(ptr, Image, 1);
	//~ new(fboptr, Framebuffer, 1);
	ptr->w = width;
	ptr->h = height;
	//~ GLuint depthbuffer;
	//~ glGenRenderbuffers_(1, &depthbuffer);
			//~ glBindRenderbuffer_(GL_RENDERBUFFER_EXT, depthbuffer);
			//~ glRenderbufferStorage_(GL_RENDERBUFFER_EXT, GL_DEPTH_COMPONENT16, width, height);
			//~ glBindRenderbuffer_(GL_RENDERBUFFER_EXT, 0);
			
	// generate texture save target
	glGenTextures(1, &ptr->id);
	glBindTexture(GL_TEXTURE_2D, ptr->id);
	
	if(interpolation) {
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	}
	else {
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	}
	
	if(repeat) {
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	} else {
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
	}
	
	glTexImage2D(GL_TEXTURE_2D, 0, internal, width, height, 0, GL_RGBA, format, 0);
	glBindTexture(GL_TEXTURE_2D, 0);
	
	// create framebuffer
	glGenFramebuffers_(1, &fboptr->id);
	glBindFramebuffer_(GL_FRAMEBUFFER_EXT, fboptr->id);
	glFramebufferTexture2D_(GL_FRAMEBUFFER_EXT, GL_COLOR_ATTACHMENT0_EXT,
			GL_TEXTURE_2D, ptr->id, 0);
	//~ glFramebufferRenderbuffer_(GL_FRAMEBUFFER_EXT, GL_DEPTH_ATTACHMENT_EXT,
					//~ GL_RENDERBUFFER_EXT, depthbuffer);
	status = checkFramebufferStatus();

	// unbind framebuffer
	glBindFramebuffer_(GL_FRAMEBUFFER_EXT, (GLuint)current_fbo);
		
	if (status) {
		fboptr->image = ptr;
		return;
	}
	else {
		glDeleteTextures(1, &ptr->id);
		glDeleteFramebuffers_(1, &fboptr->id);
		delete(ptr);
		fboptr->id = 0;
		//~ myError("Framebuffer is not initialized.");
		//~ delete(fboptr);
		return;
	}
}

/**
 * @descr Check, if framebuffer created without errors.
 * @group graphics/framebuffer
 * @var Framebuffer object
 * */
bool framebufferCheck(Framebuffer * ptr) {
	return ptr->id;
}

/**
 * @descr Bind framebuffer object. Means, that now all graphics will be rendered to this framebuffer.
 * @group graphics/framebuffer
 * @var Framebuffer object
 * */
void framebufferBind(Framebuffer * ptr) {
	if(ptr->id) {
		printf("%d %d\n", (int)ptr->image->w, (int)ptr->image->h);
		glBindFramebuffer_(GL_FRAMEBUFFER_EXT, ptr->id);
		glViewport(0, 0, (GLsizei)ptr->image->w, (GLsizei)ptr->image->h);
		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();
		glOrtho(0, (int)ptr->image->w, 0, (int)ptr->image->h, -1, 1);
		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();
	}
	//~ else myError("Framebuffer is not initialized.");
}

/**
 * @descr Unbind framebuffer object. Means, that now all graphics will be rendered to default screen. This function unbinds the current framebuffer object.
 * @group graphics/framebuffer
 * */
void framebufferUnbind(Framebuffer * ptr) {
	printf("%d %d\n", screen->w, screen->h);
	glBindFramebuffer_(GL_FRAMEBUFFER_EXT, 0);
	glViewport(0, 0, screen->w, screen->h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(0, screen->w, screen->h, 0, -1, 1);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}

#if 0
/**
 * @descr Draw framebuffer. Same as Image:draw.
 * @group graphics/framebuffer
 * @var Framebuffer object
 * @see imageDraw
 * */
void framebufferDraw(Framebuffer * ptr) {
	imageDraw(ptr->image);
}

/**
 * @descr Draw part of framebuffer with texture coordinates. Same as Image:drawq.
 * @group graphics/framebuffer
 * @var Framebuffer object
 * @var x offset of framebuffer's texture
 * @var y offset of framebuffer's texture
 * @var width of framebuffer's texture
 * @var height of framebuffer's texture
 * @see imageDrawq
 * */
void framebufferDrawq(Framebuffer * ptr, float qx, float qy, float qw, float qh) {
	imageDrawq(ptr->image, qx, qy, qw, qh);
}
#endif
/**
 * @descr Delete framebuffer and free memory.
 * @group graphics/framebuffer
 * @var Framebuffer object
 * */
void deleteFramebuffer(Framebuffer * ptr) {
	if(ptr) {
		glDeleteTextures(1, &ptr->image->id);
		glDeleteFramebuffers_(1, &ptr->id);
		delete(ptr->image);
	}
	else myError("Trying to free a null-framebuffer. Maybe, you did it manually?");
}
