typedef struct SDL_Rect {
	short x, y;
	unsigned short w, h;
} SDL_Rect;
typedef struct Image {
	/* OpenGL texture id */
	unsigned int id;
	/* width and height of the original image */
	unsigned int w, h;
} Image;
typedef struct Framebuffer {
	unsigned int id;
	Image * image;
} Framebuffer;
typedef struct FontChar
{
	/* OpenGL list id */
	unsigned int vertex;
	/* Width of char */
	float w;
} FontChar;
typedef struct Font {
	Image * image;
	float scale, height;
	FontChar chars[256];
} Font;
typedef struct Shader {
	unsigned int id;
} Shader;
unsigned int getEventType();
unsigned int getEventKey();
unsigned int getEventKeyUnicode();
int getEventMouseX();
int getEventMouseY();
unsigned int getEventMouseButton();
unsigned int getEventResizeW();
unsigned int getEventResizeH();
int getMouseX();
int getMouseY();
Font * newFont(Image * img);
float fontWidth(Font *f, register const char *str);
float fontHeight(Font *font);
void fontPrint(register const char * str);
void fontPrintf(register const char * str, float maxw, int align);
void fontSelect(Font *font);
void fontScale(Font *font, float scale);
void fontSetGlyph(Font *ptr, unsigned int ch, float x1, float y1, float x2, float y2, float cx1, float cy1, float w, float h);
void deleteFont(Font * ptr);
bool init(const char * appName, int width, int height, int bpp, const char * attr);
int getWindowWidth();
int getWindowHeight();
void swapBuffers();
SDL_Rect ** getModes();
void showCursor();
void hideCursor();
void enableDepthTest();
unsigned int getTicks();
double getTime();
unsigned int GetTicks();
void translate(double translateX, double translateY);
void move(double translateX, double translateY);
void scale(double scaleX, double scaleY);
void rotate(double rotate);
void push();
void pop();
void translateObject(double x, double y, double a, double w, double h, double ox, double oy);
void reset();
void line(double x1, double y1, double x2, double y2);
void rectangle(bool filled);
void circle(double rad, double segments, bool filled);
void point(double x1, double y1);
void setPointSize(float size);
void setLineWidth(float width);
void setSmooth(bool smooth);
void Color(unsigned char r, unsigned char g, unsigned char b, unsigned char a);
void setBlendMode(const char * str);
void setBlendAlpha();
void clear();
void clearColorDepth();
void clearDepth();
Image *newImage(const char *name);
void imageDraw(Image * image);
void imageDrawq(Image * image, float qx, float qy, float qw, float qh);
void deleteImage(Image * ptr);
Framebuffer * newFramebuffer(unsigned int width, unsigned int height, unsigned int percision, bool alpha, bool interpolation, bool repeat);
void framebufferBind(Framebuffer * ptr);
void framebufferUnbind();
void framebufferDraw(Framebuffer * ptr);
void framebufferDrawq(Framebuffer * ptr, float qx, float qy, float qw, float qh);
void deleteFramebuffer(Framebuffer * ptr);
Shader * newPixelShader(const char * str);
void deleteShader(Shader * ptr);
void useShader(Shader * ptr);
void disableShader();
unsigned int GetUniformLocation(unsigned int program, const char * name);
void UseProgramObject(unsigned int program);
void Uniform1i(unsigned int location, int var);
void Uniform1f(unsigned int location, float var);
void Uniform2f(unsigned int location, float var, float var1);
void Uniform3f(unsigned int location, float var, float var1, float var2);
void Uniform4f(unsigned int location, float var, float var1, float var2, float var3);
