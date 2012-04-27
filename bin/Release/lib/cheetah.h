typedef struct SDL_Rect {
	short x, y;
	unsigned short w, h;
} SDL_Rect;
struct {
	unsigned int flags;				/**< Read-only */
	void *format;		/**< Read-only */
	int w, h;				/**< Read-only */
	unsigned short pitch;				/**< Read-only */
	void *pixels;				/**< Read-write */
	int offset;				/**< Private */

	/** Hardware-specific surface info */
	struct private_hwdata *hwdata;

	/** clipping information */
	SDL_Rect clip_rect;			/**< Read-only */
	unsigned int unused1;				/**< for binary compatibility */

	/** Allow recursive locks */
	unsigned int locked;				/**< Private */

	/** info for fast blit mapping to other surfaces */
	struct SDL_BlitMap *map;		/**< Private */

	/** format version, bumped at every change to invalidate blit maps */
	unsigned int format_version;		/**< Private */

	/** Reference count -- used when freeing surface */
	int refcount;				/**< Read-mostly */
} screen;
typedef struct __dirstream {
} DIR;
struct {
	char GLSL, BE, FBO, VBO, MT, PS;
} supported;
typedef struct Image {
	char *name;
	char *options;
	/* OpenGL texture id */
	unsigned int id;
	/* width and height of the original image */
	float w, h;
	int channels;
	int queued;
} Image;
typedef struct ImageData {
	int w, h, channels;
	char *data;
} ImageData;
typedef struct Framebuffer {
	unsigned int id;
	Image *image;
} Framebuffer;
typedef struct FontChar
{
	/* OpenGL list id */
	unsigned int vertex;
	/* Width of char */
	float w;
	float v[4], t[4];
} FontChar;
typedef struct Font {
	Image *image;
	float _scale, height, _interval, spacew;
	FontChar ***chars;
	int allocated;
	int mem;
	bool scalable, unicode;
} Font;
typedef struct Shader {
	unsigned int id;
} Shader;
typedef struct Point {
	float x, y;
} Point;
typedef struct Point3 {
	float x, y, z;
} Point3;
typedef struct Vbo {
	unsigned int id, count, tex;
	Point *data;
} Vbo;
enum {
	blend_alpha,
	blend_additive,
	blend_multiplicative,
	blend_screen,
	blend_detail,
	blend_substractive,
	blend_difference,
	blend_mask,
	blendAlpha = 0,
	blendAdditive,
	blendMultiplicative,
	blendScreen,
	blendDetail,
	blendSubstractive,
	blendDifference,
	blendMask,
};
enum {
	alignLeft = 1,
	alignCenter,
	alignRight,
	alignJustify,
	align_left = 1,
	align_center,
	align_right,
	align_justify
};
struct {
	double scaleX, scaleY, offsetX, offsetY;
	/*оригинальные ширина и высота, относительно которых считаются все координаты*/
	double origWidth, origHeight;
	double aspect;
	bool autoScale, autoScaleFont;
} screenScale;
typedef struct Resource {
	Image *image;
	unsigned char *data;
	char *name;
	char *options;
	int len;
} Resource;
unsigned int getEventType();
unsigned int getEventKey();
unsigned int getEventKeyUnicode();
int getEventMouseX();
int getEventMouseY();
unsigned int getEventMouseButton();
unsigned int getEventResizeW();
unsigned int getEventResizeH();
void setResizeDelay(int delay);
int getMouseX();
int getMouseY();
char *getKeyState();
void myError(const char *fmt, ...);
bool isPointer(void * ptr);
unsigned char * loadfile(const char * filename, unsigned int * length);
int fileatime(const char * filename);
int filemtime(const char * filename);
int filectime(const char * filename);
DIR *openDir(const char *name);
struct dirent *readDir (DIR *dirp);
int closeDir(DIR *dirp);
bool isDir(const char *name);
bool mkDir(const char * path);
float fontWidth(Font *f, register const char *str);
float fontHeight(Font *font);
void fontPrintf(Font *currentFont, register const unsigned char * str, float x, float y, float maxw, int align);
void fontScale(Font *font, float scale);
void fontInterval(Font *font, float interval);
float fontGetScale(Font *font);
float fontGetInterval(Font *font);
void fontSetGlyph(Font *ptr, const char *line);
void deleteFont(Font * ptr);
void newFramebufferOpt(Framebuffer *fboptr, unsigned int width, unsigned int height, const char * options);
bool framebufferCheck(Framebuffer * ptr);
void framebufferBind(Framebuffer * ptr);
void framebufferUnbind(Framebuffer * ptr);
void framebufferDraw(Framebuffer * ptr);
void framebufferDrawq(Framebuffer * ptr, float qx, float qy, float qw, float qh);
void deleteFramebuffer(Framebuffer * ptr);
void generateImageData(ImageData *ptr, int w, int h, const char *imageType);
void generateImage(Image *ptr, int w, int h, const char *imageType);
void colorMask(bool r, bool g, bool b, bool a);
void enableDepthTest();
void disableDepthTest();
void enableStencilTest();
void disableStencilTest();
void move(double translateX, double translateY);
void scale(double scaleX, double scaleY);
void rotate(double angle);
void translateObject(double x, double y, double angle, double width, double height, double origin_x, double origin_y);
void autoScale(bool autoScale);
void prepare();
void end();
void blend(bool blend);
void enableBlend();
void disableBlend();
void push();
void pop();
void reset();
void line(double x1, double y1, double x2, double y2);
void rectangle();
void rectanglexy(float x, float y, float w, float h);
void circle(double rad, double segments, bool filled);
void circlexy(float x, float y, double rad, double segments, bool filled);
void point();
void pointxy(float x, float y);
void pointSize(float size);
double getPointSize();
void lineWidth(float width);
double getLineWidth();
void smooth(bool smooth);
void color(unsigned char r, unsigned char g, unsigned char b, unsigned char a);
void colorf(float r, float g, float b, float a);
void clearColor(float r, float g, float b, float a);
void clearScreen(bool enabled);
void blendMode(int mode);
void blendEquation(int mode);
void blendFunc(int sourcefactor, int destinationfactor);
void clear();
void clearColorDepth();
void clearColorStencil();
void clearDepth();
void clearStencil();
void stencilFunc(int func, int ref, unsigned int mask);
void stencilOp(int fail, int zfail, int zpass);
void drawToStencil();
void drawUsingStencil();
void newImageOpt(Image* ptr, const char *name, const char *options);
inline void imageBind(Image * image);
void enableTexture();
void disableTexture();
void imageDraw(Image * image);
void imageDrawxy(Image * image, float x, float y, float w, float h);
void imageDrawt(Image * image, float x, float y, float w, float h, float a, float ox, float oy);
void imageDrawq(Image * image, float qx, float qy, float qw, float qh);
void imageDrawqxy(Image * image, float x, float y, float w, float h, float qx, float qy, float qw, float qh);
void activeTexture(int i);
void deleteImage(Image * ptr);
void imageFiltering(Image * img, bool enabled);
void _newImageFromData(Image * ptr, ImageData * imgdata, const char *options);
void newFragmentVertexShader(Shader * ptr, const char * pix, const char * ver);
void newFragmentShader(Shader * ptr, const char * frag);
bool shaderCheck(Shader * ptr);
void deleteShader(Shader * ptr);
void shaderBind(Shader * ptr);
void shaderUnbind(Shader * ptr);
unsigned int GetUniformLocation(unsigned int program, const char * name);
void UseProgramObject(unsigned int program);
void Uniform1i(unsigned int location, int var);
void Uniform1f(unsigned int location, float var);
void Uniform2f(unsigned int location, float var, float var1);
void Uniform3f(unsigned int location, float var, float var1, float var2);
void Uniform4f(unsigned int location, float var, float var1, float var2, float var3);
unsigned int getTicks();
double getTime();
void delay(unsigned int ms);
void sleep(unsigned int sec);
Vbo * newVbo(Point * data, Point * tex, unsigned int count);
void vboDraw(Vbo * ptr);
Vbo * newVboPoints(Point * data, unsigned int count);
void vboDrawSprites(Vbo * ptr, Image * img, float size);
Vbo * newVboPoints3(Point3 * data, unsigned int count);
void vboDrawSprites3(Vbo * ptr, Image * img, float size);
void deleteVbo(Vbo * ptr);
bool init(const char * appName, unsigned int width, unsigned int height, int bpp, const char * attr);
bool isInit();
int getWindowWidth();
int getWindowHeight();
void caption(const char * text);
SDL_Rect ** getModes();
void (*swapBuffers)(void);
int (*showCursor)(int mode);
int (*grabCursor)(int mode);
