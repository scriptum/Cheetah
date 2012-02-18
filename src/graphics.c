#include <math.h>
#include "cheetah.h"
//~ #include "SOIL/SOIL.h"
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

//~ #if 0
void colorMask(bool r, bool g, bool b, bool a) {
	glColorMask(r,g,b,a);
}
//~ #endif

GLuint quadlist;

bool init(const char * appName, unsigned int width, unsigned int height, int bpp, const char * attr) {
	bool fullscreen = 0;
	bool resizable = 0;
	bool vsync = 0;
	bool firstrun = 0;
	bool depth = 0;
	bool stencil = 0;
	char ch;
	Uint32 flags = SDL_OPENGL;

	while(*attr)
	{
		ch = *attr;
		if(ch == 'f') fullscreen = 1;
		if(ch == 'r') resizable = 1;
		if(ch == 'v') vsync = 1;
		if(ch == 'd') depth = 1;
		if(ch == 's') stencil = 1;
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
		if(depth) SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);
		if(stencil) SDL_GL_SetAttribute(SDL_GL_STENCIL_SIZE, 8);
		firstrun = 1;
	}
	if(appName) SDL_WM_SetCaption (appName, appName);
	SDL_GL_SetAttribute(SDL_GL_SWAP_CONTROL, vsync);
	screen = SDL_SetVideoMode(width, height, bpp, flags);
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
	glDepthRange(-10000,10000);
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

void disableDepthTest() {
	glDisable(GL_DEPTH_TEST);
}

void enableStencilTest() {
	glEnable(GL_STENCIL_TEST);
}

void disableStencilTest() {
	glDisable(GL_STENCIL_TEST);
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

void blend(bool bl) {
	if(bl) glEnable(GL_BLEND);
	else glDisable(GL_BLEND);
}

void enableBlend() {
	glEnable(GL_BLEND);
}

void disableBlend() {
	glDisable(GL_BLEND);
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

void setClearColor(float r, float g, float b, float a) {
	glClearColor(r,g,b,a);
}

void setBlendMode(int mode) {
	if(mode == blend_substractive) {
		glBlendEquation_(GL_FUNC_REVERSE_SUBTRACT);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	}
	else
	{
		glBlendEquation_(GL_FUNC_ADD);
		switch(mode) {
			case blend_alpha:
				glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
				break;
			case blend_multiplicative:
				glBlendFunc(GL_DST_COLOR, GL_ONE_MINUS_SRC_ALPHA);
				break;
			case blend_additive:
				glBlendFunc(GL_SRC_ALPHA, GL_ONE);
				break;
			case blend_screen:
				glBlendFunc(GL_ONE, GL_ONE_MINUS_SRC_COLOR);
				break;
			case blend_detail:
				glBlendFunc(GL_DST_COLOR, GL_SRC_COLOR);
				break;
		}
	}
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

void clearColorStencil() {
	glClear(GL_COLOR_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
}

void clearDepth() {
	glClear(GL_DEPTH_BUFFER_BIT);
}

void clearStencil() {
	glClear(GL_STENCIL_BUFFER_BIT);
}

void setStencilFunc(int func, int ref, unsigned int mask) {
	glStencilFunc(func, ref, mask);
}

void setStencilOp(int fail, int zfail, int zpass) {
	glStencilOp(fail, zfail, zpass);
}

void drawToStencil() {
	glStencilFunc (GL_NEVER, 0x0, 0x1);
	glStencilOp(GL_REPLACE,GL_KEEP,GL_REPLACE);
}

void drawUsingStencil() {
	glStencilFunc (GL_EQUAL, 0x1, 0x1);
	glStencilOp(GL_KEEP,GL_KEEP,GL_KEEP);
}

//~ /**
 //~ * @descr Load image from disc with specific optons
 //~ * @group image
 //~ * @param File name
 //~ * @param String of options. Supported options:
 //~ * `n` - use nearest interpolation
 //~ * `m` - generate mip-maps (automatically sets mip-map interpolation)
 //~ * @return Image object
 //~ * */
//~ Image *newImageOpt(const char *name, const char *options) {
	//~ Image *ptr;
	//~ char ch;
	//~ if(!screen)
	//~ {
		//~ myError("Call init function before!");
		//~ return NULL;
	//~ }
	//~ int width, height, channels, repeat = 1;
	//~ unsigned int tex_id;
	//~ unsigned char* img;
	//~ unsigned int file_size;
	//~ unsigned char * myBuf = loadfile(name, &file_size);
	//~ img = SOIL_load_image_from_memory(
				//~ myBuf, sizeof(unsigned char) * file_size,
				//~ &width, &height, &channels,
				//~ 0 );
	//~ if( NULL == img ) {
		//~ myError("can't load image %s", name);
		//~ return 0;
	//~ }
	//~ tex_id = SOIL_internal_create_OGL_texture(
			//~ img, width, height, channels,
			//~ 0, SOIL_FLAG_TEXTURE_REPEATS,
			//~ GL_TEXTURE_2D, GL_TEXTURE_2D,
			//~ GL_MAX_TEXTURE_SIZE );
	//~ 
	//~ while(*options)
	//~ {
		//~ ch = *options;
		//~ if(ch == 'n') {
			//~ glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
			//~ glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		//~ }
		//~ options++;
	//~ }
	//~ SOIL_free_image_data( img );
	//~ ptr = (Image *)malloc(sizeof(Image));
	//~ ptr->id = tex_id;
	//~ ptr->w = width;
	//~ ptr->h = height;
	//~ return ptr;
//~ }
//~ 
//~ /**
 //~ * @descr Load image from disc
 //~ * @group image
 //~ * @param File name
 //~ * @return Image object
 //~ * */
//~ Image *newImage(const char *name) {
	//~ return newImageOpt(name, "");
//~ }

/**
 * @descr Bind Image object. Equivalent to glBindTexture.
 * @group image
 * @param Image object
 * */
void imageBind(Image * image) {
	glBindTexture(GL_TEXTURE_2D, image->id);
}

/**
 * @descr Enable texturing. Equivalent to glEnable(GL_TEXTURE_2D).
 * @group image
 * */
void enableTexture2D() {
	glEnable(GL_TEXTURE_2D);
}

/**
 * @descr Disable texturing. Equivalent to glDisable(GL_TEXTURE_2D).
 * @group image
 * */
void disableTexture2D() {
	glDisable(GL_TEXTURE_2D);
}

void imageDraw(Image * image) {
	glBindTexture(GL_TEXTURE_2D, image->id);
	glEnable(GL_TEXTURE_2D);
	glCallList(quadlist);
	glDisable(GL_TEXTURE_2D);
}

/*TODO: Заюзать шейдры для передачи координат, а выводить квад листом. В шрифтах аналогично*/
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

void activeTexture(int i) {
	glActiveTexture_(GL_TEXTURE0 + i);
}

void deleteImage(Image * ptr) {
	if(ptr)
		glDeleteTextures(1, &ptr->id);
	else myError("Trying to free a null-image. Maybe, you did it manually?");
}

void disableFiltering(Image * img) {
	glBindTexture(GL_TEXTURE_2D, img->id);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
}

void enableFiltering(Image * img) {
	glBindTexture(GL_TEXTURE_2D, img->id);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
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

Framebuffer * newFramebuffer(unsigned int width, unsigned int height, const char * options) {
	//unsigned int percision, bool alpha, bool interpolation, bool repeat) {
	unsigned int percision = 8;
	bool alpha = 0;
	bool interpolation = 1;
	bool repeat = 0;
	Image *ptr;
	Framebuffer *fboptr;
	GLint current_fbo;
	GLenum internal, format;
	bool status;
	char ch;
	
	if(!screen)
	{
		myError("Call init function before!");
		return 0;
	}
	
	if(!supported.FBO) {
		myError("Framebuffers are not supported on this machine. You'd better to check it in script (cheetah.supported)");
		return 0;
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
		options++;
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
	
	ptr = (Image*)malloc(sizeof(Image));
	fboptr = (Framebuffer*)malloc(sizeof(Framebuffer));
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

Vbo * newVbo(Point * data, Point * tex, unsigned int count) {
	Vbo * ptr = (Vbo*)malloc(sizeof(Vbo));
	ptr->count = count;
	glGenBuffers_(1, &ptr->id);
	glBindBuffer_(GL_ARRAY_BUFFER_ARB, ptr->id);
	glBufferData_(GL_ARRAY_BUFFER_ARB, sizeof(Point)*4*count, (void*)data, GL_STATIC_DRAW_ARB);
	glGenBuffers_(1, &ptr->tex);
	glBindBuffer_(GL_ARRAY_BUFFER_ARB, ptr->tex);
	glBufferData_(GL_ARRAY_BUFFER_ARB, sizeof(Point)*4*count, (void*)tex, GL_STATIC_DRAW_ARB);
	return ptr;
}

void vboDraw(Vbo * ptr) {
	glEnable(GL_TEXTURE_2D);
	glEnableClientState(GL_VERTEX_ARRAY); 
	glEnableClientState(GL_TEXTURE_COORD_ARRAY); 
	glBindBuffer_(GL_ARRAY_BUFFER_ARB, ptr->id);
	glVertexPointer(2, GL_FLOAT, 0, (char *) NULL);
	glBindBuffer_(GL_ARRAY_BUFFER_ARB, ptr->tex);
	glTexCoordPointer(2, GL_FLOAT, 0, (char *) NULL);
	glDrawArrays(GL_QUADS, 0, ptr->count*4); 
	glDisableClientState(GL_VERTEX_ARRAY);
	glDisableClientState(GL_TEXTURE_COORD_ARRAY);
	glDisable(GL_TEXTURE_2D);
}

Vbo * newVboPoints(Point * data, unsigned int count) {
	Vbo * ptr = (Vbo*)malloc(sizeof(Vbo));
	ptr->count = count;
	int i;
	if(supported.PS)
	{
		if(supported.VBO)
		{
			glGenBuffers_(1, &ptr->id);
			glBindBuffer_(GL_ARRAY_BUFFER_ARB, ptr->id);
			glBufferData_(GL_ARRAY_BUFFER_ARB, sizeof(Point)*count, (void*)data, GL_STATIC_DRAW_ARB);
		}
		else
		{
			printf("No VBO support: rendering VBO's using lists.\n");
			ptr->id = glGenLists(1);
			glNewList(ptr->id, GL_COMPILE);
			glBegin(GL_POINTS);
			for (i = 0; i < count; i++)
				glVertex2f(data[i].x, data[i].y);
			glEnd();
			glEndList();
		}
	}
	else
	{
		ptr->data = data;
		printf("No Point Sprite support: falling down to quads.\n");
	}
	
	return ptr;
}
void vboDrawSprites(Vbo * ptr, Image * img, float size) {
	int i;
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, img->id);
	if(supported.PS)
	{
		glPointSize(size);
		glEnable(GL_POINT_SPRITE);
		glTexEnvi(GL_POINT_SPRITE, GL_COORD_REPLACE, GL_TRUE);
		
		if(supported.VBO)
		{
			glEnableClientState(GL_VERTEX_ARRAY);
			glBindBuffer_(GL_ARRAY_BUFFER_ARB, ptr->id);
			glVertexPointer(2, GL_FLOAT, 0, (char *) NULL);
			glDrawArrays(GL_POINTS, 0, ptr->count); 
			glDisableClientState(GL_VERTEX_ARRAY);
		}
		else
			glCallList(ptr->id);
	}
	else
	{
		for (i = 0; i < ptr->count; i++)
		{
			glPushMatrix();
			glTranslatef(ptr->data[i].x - size/2, ptr->data[i].y - size/2, 0.0);
			glScalef(size, size, 1.0);
			glCallList(quadlist);
			glPopMatrix();
		}
	}
	
	glDisable(GL_TEXTURE_2D);
}

Vbo * newVboPoints3(Point3 * data, unsigned int count) {
	Vbo * ptr = (Vbo*)malloc(sizeof(Vbo));
	ptr->count = count;
	int i;
	glGenBuffers_(1, &ptr->id);
	glBindBuffer_(GL_ARRAY_BUFFER_ARB, ptr->id);
	glBufferData_(GL_ARRAY_BUFFER_ARB, sizeof(Point3)*count, (void*)data, GL_STATIC_DRAW_ARB);
	return ptr;
}
void vboDrawSprites3(Vbo * ptr, Image * img, float size) {
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, img->id);
	glPointSize(size);
	glEnable(GL_POINT_SPRITE);
	glTexEnvi(GL_POINT_SPRITE, GL_COORD_REPLACE, GL_TRUE);
	glEnableClientState(GL_VERTEX_ARRAY);
	glBindBuffer_(GL_ARRAY_BUFFER_ARB, ptr->id);
	glVertexPointer(3, GL_FLOAT, 0, (char *) NULL);
	glDrawArrays(GL_POINTS, 0, ptr->count); 
	glDisableClientState(GL_VERTEX_ARRAY);
	
	glDisable(GL_TEXTURE_2D);
}

void deleteVbo(Vbo * ptr) {
	if(ptr) {
		glDeleteBuffers_(1, &ptr->id);
	}
	else myError("Trying to free a null-VBO. Maybe, you did it manually?");
}