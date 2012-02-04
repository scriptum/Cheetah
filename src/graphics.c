#include <math.h>
#include "cheetah.h"
#include "SOIL/SOIL.h"
#include "render.h"
//~ void stackdumper(int param)
//~ {
	//~ void *array[10];
	//~ size_t size;
	//~ char **strings;
	//~ size_t i;
//~ 
	//~ size = backtrace(array, 10);
	//~ strings = backtrace_symbols (array, size);
//~ 
	//~ printf ("Obtained %zd stack frames.\n", size);
//~ 
	//~ for (i = 0; i < size; i++)
		//~ printf ("%s\n", strings[i]);
//~ 
	//~ free (strings);
//~ }

GLuint quadlist;

bool init(const char * appName, unsigned int width, unsigned int height, int bpp, const char * attr) {
	bool fullscreen = 0;
	bool resizable = 0;
	bool vsync = 0;
	bool firstrun = 0;
	char ch;
	Uint32 flags = SDL_OPENGL;

	while(*attr)
	{
		ch = *attr;
		if(ch == 'f') fullscreen = 1;
		if(ch == 'r') resizable = 1;
		if(ch == 'v') vsync = 1;
		//if(ch == 'm') flags |= SDL_WINDOW_MAXIMIZED;
		attr++;
	}

	if (fullscreen) {
		flags |= SDL_FULLSCREEN;
	} else if (resizable) {
		flags |= SDL_RESIZABLE;
	}
	
	if(width < 1) width = 1;
	if(height < 1) height = 1;
	
	if (screen == NULL) {
		if ( SDL_Init ( SDL_INIT_VIDEO | SDL_INIT_AUDIO ) != 0 )
			return 0;
		atexit(SDL_Quit);
		SDL_EnableUNICODE(1);
		SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
		firstrun = 1;
	}
	if(appName) SDL_WM_SetCaption (appName, appName);
	SDL_GL_SetAttribute(SDL_GL_SWAP_CONTROL, vsync);
	screen = SDL_SetVideoMode(width, height, 32, flags);
	if (screen == NULL)
		myError("couldn't set %dx%dx%d video mode: %s",
								width, height, bpp, SDL_GetError());
	initRenderer();
	if (firstrun) {
		/* set background color */
		glClearColor( 0, 0, 0, 1);
		/* set line antialiasing */
		glEnable(GL_POINT_SMOOTH);
		glEnable(GL_LINE_SMOOTH);
		/* enable blending */
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		glEnable(GL_BLEND);
		/* enable backface culling */
		glDisable(GL_CULL_FACE);
		glCullFace(GL_BACK);
		glDisable(GL_DEPTH_TEST);
	}

	glViewport( 0, 0, width, height );
	glMatrixMode( GL_PROJECTION );
	glLoadIdentity();
	glOrtho( 0, width, height, 0, -1, 1 );
	glMatrixMode( GL_MODELVIEW );
	glLoadIdentity();
	
	if(firstrun) {
		quadlist = glGenLists(1);
		glNewList(quadlist, GL_COMPILE);
		glBegin(GL_QUADS);
		glTexCoord2f(0, 0); glVertex2f(0, 0);
		glTexCoord2f(0, 1); glVertex2f(0, 1);
		glTexCoord2f(1, 1); glVertex2f(1, 1);
		glTexCoord2f(1, 0); glVertex2f(1, 0);
		glEnd();
		glEndList();
	}
	return 0;
}

int getWindowWidth() {
	return screen->w;
}

int getWindowHeight() {
	return screen->h;
}

void swapBuffers() {
	SDL_GL_SwapBuffers();
}

void setWindowCaption(const char * text){
	SDL_WM_SetCaption(text, text);
}

SDL_Rect ** getModes() {
	SDL_Rect ** modes = SDL_ListModes(0, SDL_OPENGL | SDL_FULLSCREEN);
	if(modes == (SDL_Rect **)0 || modes == (SDL_Rect **)-1)
		return 0;
	return modes;
}

void showCursor() {
	SDL_ShowCursor(SDL_ENABLE);
}

void hideCursor() {
	SDL_ShowCursor(SDL_DISABLE);
}

void enableDepthTest() {
	glEnable(GL_DEPTH_TEST);
}

void disableDepthTest(){
	glDisable(GL_DEPTH_TEST);
}

unsigned int getTicks() {
	return SDL_GetTicks();
}

double getTime() {
	return (double)SDL_GetTicks()/1000;
}

unsigned int GetTicks() {
	return SDL_GetTicks();
}

void delay(unsigned int ms) {
	return SDL_Delay(ms);
}

void translate(double translateX, double translateY) {
	glTranslated(translateX, translateY, 0);
}

void move(double translateX, double translateY) {
	glTranslated(translateX, translateY, 0);
}

void scale(double scaleX, double scaleY) {
	glScaled(scaleX, scaleY, 0);
}

void rotate(double rotate) {
	glRotated(rotate, 0, 0, 1);
}

void blend(bool blend) {
	if(blend) glEnable(GL_BLEND);
	else glDisable(GL_BLEND);
}

void push() {
	glPushMatrix();
	if (glGetError() == GL_STACK_OVERFLOW)
		myError("No more free slots to save the view.");
}

void pop() {
	glPopMatrix();
	if (glGetError() == GL_STACK_UNDERFLOW)
		myError("No saved view was found.");
}

void translateObject(double x, double y, double angle, double width, double height, double origin_x, double origin_y) {
	glTranslated(x, y, 0);
	glTranslated(origin_x, origin_y, 0);
	glRotated(angle, 0, 0, 1);
	glScalef(width, height, 0);
	glTranslated(-origin_x/width, -origin_y/height, 0);
	//glTranslated(ox, oy, 0);
}

void reset() {
	glLoadIdentity();
}

void line(double x1, double y1, double x2, double y2) {
	glBegin(GL_LINES);
	glVertex2d(x1, y1);
	glVertex2d(x2, y2);
	glEnd();
}

void rectangle(bool filled) {
	if(filled)
		glCallList(quadlist);
	else
	{
		glBegin(GL_LINE_LOOP);
		glVertex2d(0, 0);
		glVertex2d(1, 0);
		glVertex2d(1, 1);
		glVertex2d(0, 1);
		glEnd();
	}
}

void circle(double rad, double segments, bool filled) {
	int i;
	const double DBLPI = 3.1415926 * 2;
	GLdouble angle;
	glBegin(filled ? GL_TRIANGLE_FAN : GL_LINE_LOOP);
	int max = segments;
	for (i = 0; i <= max; i++)
	{
		angle = DBLPI / segments * (double)i;
		glVertex2d(sin(angle) * rad, cos(angle) * rad);
	}
	glEnd();
}

void point(double x1, double y1) {
	glBegin(GL_POINTS);
	glVertex2d(x1, y1);
	glEnd();
}

void setPointSize(float size) {
	glPointSize(size);
}

void setLineWidth(float width) {
	glLineWidth(width);
}

void setSmooth(bool smooth) {
	if(smooth) {
		glEnable(GL_POINT_SMOOTH);
		glEnable(GL_LINE_SMOOTH);
	} else {
		glDisable(GL_POINT_SMOOTH);
		glDisable(GL_LINE_SMOOTH);
	}
}

//~ void setColor(unsigned char r, unsigned char g, unsigned char b, unsigned char a) {
	//~ glColor4ub(r,g,b,a);
//~ }
//~ 
//~ void setColorf(float r, float g, float b, float a) {
	//~ glColor4f(r,g,b,a);
//~ }

//export OGL color functions

void Color(unsigned char r, unsigned char g, unsigned char b, unsigned char a) {
	glColor4ub(r,g,b,a);
}

void setBlendMode(const char * str) {
	if(strcmp(str, "subtractive") == 0) {
		glBlendEquation_(GL_FUNC_REVERSE_SUBTRACT);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	}
	else
		glBlendEquation_(GL_FUNC_ADD);
	if (strcmp(str, "alpha") == 0)
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	else if (strcmp(str, "multiplicative") == 0)
		glBlendFunc(GL_DST_COLOR, GL_ONE_MINUS_SRC_ALPHA);
	else if (strcmp(str, "additive") == 0)
		glBlendFunc(GL_SRC_ALPHA, GL_ONE);
	else if (strcmp(str, "screen") == 0)
		glBlendFunc(GL_ONE, GL_ONE_MINUS_SRC_COLOR);
	else if (strcmp(str, "detail") == 0)
    glBlendFunc(GL_DST_COLOR, GL_SRC_COLOR);
  //~ else if (strcmp(str, "mask") == 0)
    //~ glBlendFuncSeparate_(GL_ZERO, GL_ONE, GL_SRC_COLOR, GL_ZERO);
  //~ else if (strcmp(str, "foreground") == 0)
    //~ glBlendFunc(GL_DST_ALPHA, GL_ONE_MINUS_DST_ALPHA);
}

void setBlendAlpha() {
	glBlendEquation_(GL_FUNC_ADD);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}

void clear() {
	glClear(GL_COLOR_BUFFER_BIT);
}

void clearColorDepth() {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void clearDepth() {
	glClear(GL_DEPTH_BUFFER_BIT);
}

Image *newImage(const char *name) {
	Image *ptr;
	if(!screen)
	{
		myError("Call init function before!");
		return 0;
	}
	int width, height, channels, repeat = 1;
	unsigned int tex_id;
	unsigned char* img;
	unsigned int file_size;
	unsigned char * myBuf = loadfile(name, &file_size);
	img = SOIL_load_image_from_memory(
				myBuf, sizeof(unsigned char) * file_size,
				&width, &height, &channels,
				0 );
	if( NULL == img ) {
		myError("can't load image %s", name);
		return 0;
	}
	tex_id = SOIL_internal_create_OGL_texture(
			img, width, height, channels,
			0, SOIL_FLAG_TEXTURE_REPEATS,
			GL_TEXTURE_2D, GL_TEXTURE_2D,
			GL_MAX_TEXTURE_SIZE );

	SOIL_free_image_data( img );
	ptr = (Image *)malloc(sizeof(Image));
	ptr->id = tex_id;
	ptr->w = width;
	ptr->h = height;
	return ptr;
}

void imageDraw(Image * image) {
	glBindTexture(GL_TEXTURE_2D, image->id);
	glEnable(GL_TEXTURE_2D);
	glCallList(quadlist);
	glDisable(GL_TEXTURE_2D);
}

/*todo: Заюзать шейдры для передачи координат, а выводить квад листом. В шрифтах аналогично*/
void imageDrawq(Image * image, float qx, float qy, float qw, float qh) {
	glBindTexture(GL_TEXTURE_2D, image->id);
	glEnable(GL_TEXTURE_2D);
	qx = qx/(float)image->w;
	qy = qy/(float)image->h;
	qw = qw ? qx + qw/(float)image->w : 1;
	qh = qh ? qy + qh/(float)image->h : 1;
	glBindTexture(GL_TEXTURE_2D, image->id);
	glEnable(GL_TEXTURE_2D);
	glBegin(GL_QUADS);
	glTexCoord2f(qx, qy);   glVertex2i(0, 0);
	glTexCoord2f(qx, qh);   glVertex2i(0, 1);
	glTexCoord2f(qw, qh);   glVertex2i(1, 1);
	glTexCoord2f(qw, qy);   glVertex2i(1, 0);
	glEnd();
	glDisable(GL_TEXTURE_2D);
}

void imageDrawMultitexture2(Image * image0, Image * image1) {
	glEnable(GL_TEXTURE_2D);
	glActiveTexture_(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, image0->id);
	glActiveTexture_(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, image1->id);
	glActiveTexture_(GL_TEXTURE0);
	glCallList(quadlist);
	glDisable(GL_TEXTURE_2D);
}

void imageDrawMultitexture3(Image * image0, Image * image1, Image * image2) {
	glEnable(GL_TEXTURE_2D);
	glActiveTexture_(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, image0->id);
	glActiveTexture_(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, image1->id);
	glActiveTexture_(GL_TEXTURE2);
	glBindTexture(GL_TEXTURE_2D, image2->id);
	glActiveTexture_(GL_TEXTURE0);
	glCallList(quadlist);
	glDisable(GL_TEXTURE_2D);
}

void imageDrawMultitexture4(Image * image0, Image * image1, Image * image2, Image * image3) {
	glEnable(GL_TEXTURE_2D);
	glActiveTexture_(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, image0->id);
	glActiveTexture_(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, image1->id);
	glActiveTexture_(GL_TEXTURE2);
	glBindTexture(GL_TEXTURE_2D, image2->id);
	glActiveTexture_(GL_TEXTURE3);
	glBindTexture(GL_TEXTURE_2D, image3->id);
	glActiveTexture_(GL_TEXTURE0);
	glCallList(quadlist);
	glDisable(GL_TEXTURE_2D);
}

void deleteImage(Image * ptr) {
	if(ptr)
		glDeleteTextures(1, &ptr->id);
	else myError("Trying to free a null-image. Maybe, you did it manually?");
}

int checkFramebufferStatus()
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

Framebuffer * newFramebuffer(unsigned int width, unsigned int height, unsigned int percision, bool alpha, bool interpolation, bool repeat) {
	Image *ptr;
	Framebuffer *fboptr;
	GLint current_fbo;
	GLenum internal, format;
	bool status;
	
	if(!screen)
	{
		myError("Call init function before!");
		return 0;
	}
	
	if(!supported.FBO) {
		myError("Framebuffers are not supported on this machine. You'd better to check it in script (isSupported)");
		return 0;
	}
	
	if(percision == 32) {
		internal = alpha ? GL_RGBA32F_ARB : GL_RGB32F_ARB;
		format = GL_FLOAT;
	}
	else if(percision == 16) {
		internal = alpha ? GL_RGBA16F_ARB : GL_RGB16F_ARB;
		format = GL_FLOAT;
	}
	else {
		if(percision != 8) myError("Invalid parameter in framebuffer's percision (8 expected, got %d). Using 8bit framebuffer.", percision);
		internal = alpha ? GL_RGBA : GL_RGB;
		format = GL_UNSIGNED_BYTE;
	}
	
	//save current fbo
	glGetIntegerv(GL_DRAW_FRAMEBUFFER_BINDING_EXT, &current_fbo);
	
	ptr = (Image*)malloc(sizeof(Image));
	fboptr = (Framebuffer*)malloc(sizeof(Framebuffer));
	ptr->w = width;
	ptr->h = height;
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
	status = checkFramebufferStatus();

	// unbind framebuffer
	glBindFramebuffer_(GL_FRAMEBUFFER_EXT, (GLuint)current_fbo);
		
	if (status) {
		fboptr->image = ptr;
		return fboptr;
	}
	else {
		glDeleteTextures(1, &ptr->id);
		glDeleteFramebuffers_(1, &fboptr->id);
		free(ptr);
		free(fboptr);
		return 0;
	}
}

void framebufferBind(Framebuffer * ptr) {
	glBindFramebuffer_(GL_FRAMEBUFFER_EXT, ptr->id);
	glViewport( 0, 0, ptr->image->w, ptr->image->h );
	glMatrixMode( GL_PROJECTION );
	glLoadIdentity();
	glOrtho( 0, ptr->image->w, 0, ptr->image->h, -1, 1 );
	glMatrixMode( GL_MODELVIEW );
	glLoadIdentity();
}

void framebufferUnbind() {
	glBindFramebuffer_(GL_FRAMEBUFFER_EXT, 0);
	glViewport( 0, 0, screen->w, screen->h );
	glMatrixMode( GL_PROJECTION );
	glLoadIdentity();
	glOrtho( 0, screen->w, screen->h, 0, -1, 1 );
	glMatrixMode( GL_MODELVIEW );
	glLoadIdentity();
}

void framebufferDraw(Framebuffer * ptr) {
	imageDraw(ptr->image);
}

void framebufferDrawq(Framebuffer * ptr, float qx, float qy, float qw, float qh) {
	imageDrawq(ptr->image, qx, qy, qw, qh);
}

void deleteFramebuffer(Framebuffer * ptr) {
	if(ptr) {
		glDeleteTextures(1, &ptr->image->id);
		glDeleteFramebuffers_(1, &ptr->id);
		free(ptr->image);
	}
	else myError("Trying to free a null-framebuffer. Maybe, you did it manually?");
}