typedef struct SDL_Rect {
	short		x, y;
	unsigned short	w, h;
} SDL_Rect;
struct {
	unsigned int	flags;
	void		*format;
	int		w, h;
	unsigned short	pitch;
	void		*pixels;
	int		offset;
	struct private_hwdata *hwdata;
	SDL_Rect	clip_rect;
	unsigned int	unused1;
	unsigned int	locked;
	struct SDL_BlitMap *map;
	unsigned int	ormat_version;
	int		refcount;
} screen;
typedef struct __dirstream {
} DIR;
struct {
	bool		GLSL;
	bool		BE;
	bool		FBO;
	bool		VBO;
	bool		MT;
	bool		PS;
} supported;
typedef struct Color {
	unsigned char	r, g, b, a;
} Color;
typedef struct Point {
	float x, y;
} Point;
typedef struct Size {
	short w, h;
} Size;
typedef struct Point3 {
	float x, y, z;
} Point3;
typedef struct Image {
	char		*name;
	char		*options;
	/* OpenGL texture id */
	unsigned	id;
	/* width and height */
	float		w, h;
	int		channels;
	int		queued;
} Image;
typedef struct ImageData {
	int		w, h, channels;
	char		*data;
} ImageData;
typedef struct BorderImage {
	Image		*image;
	/* image borders */
	float		top;
	float		right;
	float		bottom;
	float		left;
	/* draw border only, without central part */
	bool		borderOnly;
} BorderImage;
typedef struct Multitexture {
	float		w, h;
	int		size;
	Image 		**images;
} Multitexture;
typedef struct ParticleForce {
	Point		position;
	unsigned	maxParticles;
} ParticleForce;
typedef struct Particle {
	Point		position;
	Point		speed;
	float		age;
	/* Cheetah engine uses one random seed to generate all other parameters procedurally */
	unsigned	seed;
} Particle;
typedef struct ParticleSystem {
	ParticleForce	*forces;
	Particle	*particles;
	Image		*image;
	Point		emitterPosition;
	unsigned	maxParticles;
	float		emissionRate;
	float		direction;
	float		directionVariation;
	float		startSpeed;
	float		startSpeedVariation;
	float		scale;
	float		scaleVariation;
	float		gravity;
	float		gravityVariation;
	float		particleLife;
	float		particleLifeVariation;
	double		lifeTime;
	Color		color;
	Color		colorVariation;
	double		_lasttime;
	double		_startTime;
	unsigned	_aliveParticles;
} ParticleSystem;
typedef struct Atlas {
	Image		*image;
	/* original width and height */
	float		w, h;
	/* atlas width and height */
	float		aw, ah;
	/* offset on atlas*/
	float		x, y;
	/*texture coordinates*/
	float		tex[8];
} Atlas;
typedef struct Framebuffer {
	unsigned	id;
	Image		*image;
} Framebuffer;
typedef struct FontChar
{
	/* Width of char */
	float		w;
	float		v[4];
	float		t[4];
	bool kerning;
} FontChar;
typedef struct Font {
	Image		*image;
	void		*hash;
	void		*kerningHash;
	float		height;
	float		_spacewidth;
	float		_interval;
	float		_scale;
	int		mem;
	bool		scalable;
} Font;
typedef struct Shader {
	unsigned	id;
} Shader;
typedef struct _Tilemap {
	int		w, h;            // size in tiles
	int		tw, th;          // single tile size
	float		**index;       // texture coords index
	unsigned char	**map; // tile indexes map
	int		scalable;        // should tilemap be scaled to screen size or drawed per-pixel
	Image		*img;
} Tilemap;
typedef struct Vbo {
	unsigned	id, count, tex;
	Point		*data;
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
	float		scaleX;
	float		scaleY;
	float		offsetX;
	float		offsetY;
	/* original (first-time defined) width and height, if auto-scale enabled*/
	float		origWidth;
	float		origHeight;
	float		aspect;
	bool		autoScale;
	bool		autoScaleFont;
} screenScale;
struct {
	/* Used to check window resizing */
	unsigned	rescaleTime;
	/* System timers: usigned (milliseconds) and double (seconds) */
	unsigned	time;
	double		timed;
	/* Game time: may be stopped/run faster/run slower than system time */
	double		gameTimed;
	double		timeOffsetd;
	double		gameSpeed;
	unsigned	resizeDelay;
} globalTimers;
void atlasDrawt(Atlas *p, float x, float y, float w, float h, float a, float ox, float oy);
void atlasDrawxy(Atlas *p, float x, float y, float w, float h);
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
unsigned char *getKeyState();
void gameSpeed(double speed);
void myError(const char *fmt, ...);
bool isPointer(void * ptr);
unsigned char *loadfile(const char *filename, unsigned int *length);
int fileatime(const char * filename);
int filemtime(const char * filename);
int filectime(const char * filename);
DIR *openDir(const char *name);
struct dirent *readDir(DIR *dirp);
int closeDir(DIR *dirp);
bool isDir(const char *name);
bool mkDir(const char * path);
char *getDirentName(struct dirent * de);
float fontWidth(Font *f, const char *str);
float fontHeight(Font *font);
void fontPrintf(Font *currentFont, const unsigned char *str, float x, float y, float maxw, int align);
void fontScale(Font *font, float scale);
void fontInterval(Font *font, float interval);
float fontGetScale(Font *font);
float fontGetInterval(Font *font);
void fontSetGlyph(Font *ptr, const char *line);
void fontSetKerning(Font *ptr, const char *line);
void deleteFont(Font * ptr);
void newFramebufferOpt(Framebuffer *fboptr, unsigned int width, unsigned int height, const char * options);
bool framebufferCheck(Framebuffer * ptr);
void framebufferBind(Framebuffer * ptr);
void framebufferUnbind(Framebuffer * ptr);
void framebufferSaveBMP(Framebuffer * ptr, const char* name);
void deleteFramebuffer(Framebuffer * ptr);
static void generateImageData(ImageData *ptr, int w, int h, const char *imageType, bool alpha);
void generateImage(Image *ptr, int w, int h, const char *imageType, const char *options);
void colorMask(bool r, bool g, bool b, bool a);
void enableDepthTest();
void disableDepthTest();
void enableStencilTest();
void disableStencilTest();
void enableScissorTest();
void disableScissorTest();
void enableAlphaTest();
void disableAlphaTest();
void setScissor(int x, int y, int w, int h);
void flush();
void move(double translateX, double translateY);
void scale(double scaleX, double scaleY);
void rotate(double angle);
void translateObject(double x, double y, double angle, double width, double height, double origin_x, double origin_y);
void blend(bool blend);
void enableBlend();
void disableBlend();
void push();
void pop();
void reset();
void rectanglexy(float x, float y, float w, float h);
void circlexy(float x, float y, float rad, float segments, bool filled);
void color(unsigned char r, unsigned char g, unsigned char b, unsigned char a);
void colorf(float r, float g, float b, float a);
void clearColor(float r, float g, float b, float a);
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
void newImageOpt(Image *ptr, const char *name, const char *options);
void newImageRaw(Image *ptr, int width, int height, const char *data, const char *options);
void imageBind(Image * image);
void enableTexture();
void disableTexture();
void imageDrawxy(Image * image, float x, float y, float w, float h);
void imageDrawt(Image * image, float x, float y, float w, float h, float a, float ox, float oy);
void imageDrawqxy(Image * image, float x, float y, float w, float h, float qx, float qy, float qw, float qh);
void imageDrawqt(Image * image, float x, float y, float w, float h, float qx, float qy, float qw, float qh, float a, float ox, float oy);
void borderImageDrawt(BorderImage * borderImage, float x, float y, float w, float h, float a, float ox, float oy);
void borderImageDrawxy(BorderImage * borderImage, float x, float y, float w, float h);
void initMultitexture(Multitexture * multitexture);
void deleteMultitexture(Multitexture * multitexture);
void multitextureDrawxy(Multitexture * multitexture, float x, float y, float w, float h);
void multitextureDrawt(Multitexture * multitexture, float x, float y, float w, float h, float a, float ox, float oy);
void multitextureDrawqxy(Multitexture * multitexture, float x, float y, float w, float h, float qx, float qy, float qw, float qh);
void multitextureDrawqt(Multitexture * multitexture, float x, float y, float w, float h, float qx, float qy, float qw, float qh, float a, float ox, float oy);
void deleteImage(Image * ptr);
void newParticleSystem(ParticleSystem *ptr, int maxParticles, const char *options);
void particleSystemUpdate(ParticleSystem *ptr);
void particleSystemDraw(ParticleSystem *ptr, float x, float y);
void deleteParticleSystem(ParticleSystem *ptr);
void newFragmentVertexShader(Shader * ptr, const char * pix, const char * ver);
void newFragmentShader(Shader * ptr, const char * frag);
bool shaderCheck(Shader * ptr);
void deleteShader(Shader * ptr);
void shaderBind(Shader * ptr);
void shaderUnbind(Shader * ptr);
unsigned int GetUniformLocation(unsigned int program, const char * name);
void Uniform1i(unsigned int location, int var);
void Uniform1f(unsigned int location, float var);
void Uniform2f(unsigned int location, float var, float var1);
void Uniform3f(unsigned int location, float var, float var1, float var2);
void Uniform4f(unsigned int location, float var, float var1, float var2, float var3);
unsigned int getTicks();
double getGameTime();
double getTime();
void delay(unsigned int ms);
void sleep(unsigned int sec);
void newTilmapInternal(Tilemap *t, const char *name);
void tilemapDraw(Tilemap *t, double x, double y, double r, double z, double ox, double oy);
void deleteTilemap(Tilemap *t);
Vbo * newVbo(Point * data, Point * tex, unsigned int count);
void vboDraw(Vbo * ptr);
Vbo * newVboPoints(Point * data, unsigned int count);
void vboDrawSprites(Vbo * ptr, Image * img, float size);
Vbo * newVboPoints3(Point3 * data, unsigned int count);
void vboDrawSprites3(Vbo * ptr, Image * img, float size);
void deleteVbo(Vbo * ptr);
void setWindowSize(unsigned w, unsigned h);
bool cheetahInit(const char * appName, const char * options);
bool isInit();
int getWindowWidth();
int getWindowHeight();
void swapBuffers();
void setTitle(const char * text);
SDL_Rect **getModesSDL();
void clearScreen(bool enabled);
void autoScale(bool autoScale);
void prepare();
int (*showCursor)(int mode);
int (*grabCursor)(int mode);
