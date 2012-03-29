typedef struct SDL_Rect {
	short x, y;
	unsigned short w, h;
} SDL_Rect;
typedef struct __dirstream {
} DIR;
//~ typedef struct dirent {
	//~ unsigned int ino;
	//~ unsigned int off;
	//~ unsigned short int reclen;
	//~ unsigned char type;
	//~ char name[256];
//~ } dirent;
//~ typedef struct lstat {
	//~ unsigned long int     st_dev;     /* ID of device containing file */
	//~ unsigned long int     st_ino;     /* inode number */
	//~ int    st_mode;    /* protection */
	   //~ st_nlink;   /* number of hard links */
	 //~ uid_t     st_uid;     /* user ID of owner */
	 //~ gid_t     st_gid;     /* group ID of owner */
	 //~ dev_t     st_rdev;    /* device ID (if special file) */
	 //~ off_t     st_size;    /* total size, in bytes */
	 //~ blksize_t st_blksize; /* blocksize for file system I/O */
	 //~ blkcnt_t  st_blocks;  /* number of 512B blocks allocated */
	 //~ time_t    st_atime;   /* time of last access */
	 //~ time_t    st_mtime;   /* time of last modification */
	 //~ time_t    st_ctime;   /* time of last status change */
//~ } lstat;
struct {
	char GLSL, BE, FBO, VBO, MT, PS;
} supported;
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
typedef struct Point {
	float x, y;
} Point;
typedef struct Point3 {
	float x, y, z;
} Point3;
typedef struct Vbo {
	unsigned int id, count, tex;
	Point * data;
} Vbo;
enum {
	blend_alpha,
	blend_additive,
	blend_multiplicative,
	blend_screen,
	blend_detail,
	blend_substractive,
	blendAlpha = 0,
	blendAdditive,
	blendMultiplicative,
	blendScreen,
	blendDetail,
	blendSubstractive,
};
struct {
	double scaleX, scaleY, offsetX, offsetY;
	/*оригинальные ширина и высота, относительно которых считаются все координаты*/
	double origWidth, origHeight;
	double aspect;
	bool autoScale, autoScaleFont;
} screenScale;
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
Font * newFont(Image * img);
float fontWidth(Font *f, register const char *str);
float fontHeight(Font *font);
void fontPrint(register const char * str, int maxw, int align);
void fontPrintf(register const char * str, float maxw, int align);
void fontSelect(Font *font);
void fontScale(Font *font, float scale);
void fontSetGlyph(Font *ptr, unsigned int ch, float x1, float y1, float x2, float y2, float cx1, float cy1, float w, float h);
void deleteFont(Font * ptr);
void colorMask(bool r, bool g, bool b, bool a);
bool init(const char * appName, unsigned int width, unsigned int height, int bpp, const char * attr);
bool isInit();
int getWindowWidth();
int getWindowHeight();
void swapBuffers();
SDL_Rect ** getModes();
void showCursor();
void hideCursor();
void enableDepthTest();
void disableDepthTest();
void enableStencilTest();
void disableStencilTest();
unsigned int getTicks();
double getTime();
void delay(unsigned int ms);
void sleep(unsigned int sec);
void move(double translateX, double translateY);
void scale(double scaleX, double scaleY);
void rotate(double angle);
void translateObject(double x, double y, double angle, double width, double height, double origin_x, double origin_y);
void setAutoScale(bool autoScale);
void doAutoScale();
void blend(bool blend);
void enableBlend();
void disableBlend();
void push();
void pop();
void reset();
void line(double x1, double y1, double x2, double y2);
void rectangle(bool filled);
void circle(double rad, double segments, bool filled);
void point();
double getPointSize();
void setPointSize(float size);
void setLineWidth(float width);
double getLineWidth();
void setSmooth(bool smooth);
void color(unsigned char r, unsigned char g, unsigned char b, unsigned char a);
void setClearColor(float r, float g, float b, float a);
void setBlendMode(int mode);
void clear();
void clearColorDepth();
void clearColorStencil();
void clearDepth();
void clearStencil();
void setStencilFunc(int func, int ref, unsigned int mask);
void setStencilOp(int fail, int zfail, int zpass);
void drawToStencil();
void drawUsingStencil();
Image *newImage(const char *name);
Image *newImageOpt(const char *name, const char *options);
void imageBind(Image * image);
void enableTexture2D();
void disableTexture2D();
void imageDraw(Image * image);
void imageDrawq(Image * image, float qx, float qy, float qw, float qh);
void imageDrawqxy(Image * image, float x, float y, float w, float h, float qx, float qy, float qw, float qh);
void activeTexture(int i);
void deleteImage(Image * ptr);
void imageFiltering(Image * img, bool enabled);
Framebuffer * newFramebuffer(unsigned int width, unsigned int height, const char * options);
void framebufferBind(Framebuffer * ptr);
void framebufferUnbind();
void framebufferDraw(Framebuffer * ptr);
void framebufferDrawq(Framebuffer * ptr, float qx, float qy, float qw, float qh);
void deleteFramebuffer(Framebuffer * ptr);
Vbo * newVbo(Point * data, Point * tex, unsigned int count);
void vboDraw(Vbo * ptr);
Vbo * newVboPoints(Point * data, unsigned int count);
void vboDrawSprites(Vbo * ptr, Image * img, float size);
Vbo * newVboPoints3(Point3 * data, unsigned int count);
void vboDrawSprites3(Vbo * ptr, Image * img, float size);
void deleteVbo(Vbo * ptr);
Shader * newVertexFragmentShader(const char * ver, const char * pix);
Shader * newFragmentShader(const char * str);
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
