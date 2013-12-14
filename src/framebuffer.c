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

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "cheetah.h"
#include "cmacros.h"
#include "render.h"
#include "cvertex.h"
#include "image_write.h"
#include "test.h"

void resetView(unsigned w, unsigned h);
void resetViewDefault();

static inline void bindFramebuffer(unsigned id)
{
	glBindFramebuffer_(GL_FRAMEBUFFER_EXT, id);
}

static bool checkFramebufferStatus(void)
{
	GLenum status;
	status = (GLenum)glCheckFramebufferStatus_(GL_FRAMEBUFFER_EXT);
	switch(status) {
		case GL_FRAMEBUFFER_COMPLETE_EXT:
			return TRUE;
		case GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT_EXT:
			dbg("FBO: incomplete attachment");
			break;
		case GL_FRAMEBUFFER_UNSUPPORTED_EXT:
			dbg("Unsupported FBO format");
			break;
		case GL_FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT_EXT:
			dbg("FBO: missing attachment");
			break;
		case GL_FRAMEBUFFER_INCOMPLETE_DIMENSIONS_EXT:
			dbg("FBO: attached images must have same dimensions");
			break;
		case GL_FRAMEBUFFER_INCOMPLETE_FORMATS_EXT:
			dbg("FBO: attached images must have same format");
			break;
		case GL_FRAMEBUFFER_INCOMPLETE_DRAW_BUFFER_EXT:
			dbg("FBO: missing draw buffer");
			break;
		case GL_FRAMEBUFFER_INCOMPLETE_READ_BUFFER_EXT:
			dbg("FBO: missing read buffer");
			break;
		default:
			dbg("FBO: unknown error");
	}
	return FALSE;
}

/* Create framebuffer object. Note, that not all video drivers support this. It's recommend to check returning value using cheetah.inPointer and check framebuffer support using cheetah.supported.FBO. */
void newFramebufferOpt(Framebuffer *fboptr, unsigned int width, unsigned int height, const char * options) {
	Image *ptr = NULL;
	GLint current_fbo;
	GLint internal;
	GLenum format;
	bool status;

	fboptr->id = 0;

	NEEDED_INIT_VOID;

	if(!supported.FBO) {
		dbg("Framebuffers are not supported on this machine. You'd better to check it in script (try \"if cheetah.supported.FBO\")");
		return;
	}

	CHECK_OPTION(options, alpha);       /* Use alpha channel */
	CHECK_OPTION(options, nearest);     /* Use nearest interpolation */
	CHECK_OPTION(options, clamp);       /* Use clamp policy */
	CHECK_OPTION(options, percision16); /* Set percision to half float */
	CHECK_OPTION(options, percision32); /* Set percision  */

	if(TRUE == percision32)
	{
		internal = alpha ? GL_RGBA32F_ARB : GL_RGB32F_ARB;
		format = GL_FLOAT;
	}
	else if(TRUE == percision16)
	{
		internal = alpha ? GL_RGBA16F_ARB : GL_RGB16F_ARB;
		format = GL_HALF_FLOAT_ARB;
	}
	else
	{
		internal = alpha ? GL_RGBA : GL_RGB;
		format = GL_UNSIGNED_BYTE;
	}

	/* save current fbo */
	glGetIntegerv(GL_DRAW_FRAMEBUFFER_BINDING_EXT, &current_fbo);

	new(ptr, Image, 1);
	ptr->w = (float)width;
	ptr->h = (float)height;

	/* generate texture save target */
	glGenTextures(1, &ptr->id);
	TEXTURE_BIND(ptr->id);

	if(TRUE == nearest)
		TEX_NEAREST;
	else
		TEX_LINEAR;

	if(TRUE == clamp)
		TEX_CLAMP;
	else
		TEX_REPEAT;

	glTexImage2D(GL_TEXTURE_2D, 0, internal, (GLsizei)width, (GLsizei)height, 0, GL_RGBA, format, 0);
	//~ glBindTexture(GL_TEXTURE_2D, 0);

	/* create framebuffer */
	glGenFramebuffers_(1, &fboptr->id);
	bindFramebuffer(fboptr->id);
	glFramebufferTexture2D_(GL_FRAMEBUFFER_EXT, GL_COLOR_ATTACHMENT0_EXT,
			GL_TEXTURE_2D, ptr->id, 0);
	status = checkFramebufferStatus();

	/* unbind framebuffer */
	bindFramebuffer((GLuint)current_fbo);

	if(TRUE == status)
	{
		fboptr->image = ptr;
		return;
	}
	else
	{
		glDeleteTextures(1, &ptr->id);
		glDeleteFramebuffers_(1, &fboptr->id);
		delete(ptr);
		fboptr->id = 0;
		return;
	}
}

/* Check, if framebuffer created without errors. */
bool framebufferCheck(Framebuffer *ptr) {
	return (bool)ptr->id;
}

/* Bind framebuffer object. Means, that now all graphics will be rendered to
 * this framebuffer. */
void framebufferBind(Framebuffer *ptr) {
	if(ptr->id)
	{
		FLUSH_BUFFER();
		bindFramebuffer(ptr->id);
		glViewport(0, 0, (GLsizei)ptr->image->w, (GLsizei)ptr->image->h);
		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();
		glOrtho(0, (int)ptr->image->w, 0, (int)ptr->image->h, -1, 1);
		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();
	}
}

/* Unbind framebuffer object. Means, that now all graphics will be rendered to
 * default screen. This function unbinds the current framebuffer object. */
void framebufferUnbind(Framebuffer *ptr) {
	FLUSH_BUFFER();
	bindFramebuffer(0);
	resetViewDefault();
}

/* Save image from framebuffer to BMP format */
void framebufferSaveBMP(Framebuffer *ptr, const char *name) {
	unsigned char *img = NULL;
	GLsizei w = (GLsizei)ptr->image->w; 
	GLsizei h = (GLsizei)ptr->image->h;
	new(img, unsigned char, (size_t)(3 * w * h));
	bindFramebuffer(ptr->id);
	TEXTURE_BIND(ptr->image->id);
	glReadBuffer(GL_COLOR_ATTACHMENT0_EXT);
	glReadPixels(0, 0, w, h, GL_RGB, GL_UNSIGNED_BYTE, img);
	stbi_write_bmp(name, w, h, 3, img);
	delete(img);
}

/* Delete framebuffer and free memory. */
void deleteFramebuffer(Framebuffer *ptr) {
	if(ptr)
	{
		glDeleteTextures(1, &ptr->image->id);
		glDeleteFramebuffers_(1, &ptr->id);
		delete(ptr->image);
	}
	else
	{
		dbg("Trying to free a null-framebuffer. Maybe, you did it manually?");
	}
}
