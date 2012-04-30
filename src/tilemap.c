#include "cheetah.h"
#include "render.h"
#include "macros.h"

/*
typedef struct _Tilemap {
	int w, h;            // size in tiles
        int tw, th;          // single tile size
	float index[256][4]; // texture coords index
	unsigned char **map; // tile indexes map
	int scalable;        // should tilemap be scaled to screen size or drawed per-pixel
	Image *img;
} Tilemap;
*/
#define MAX_MAP_SIZE 256

static inline void calculateIndexes(Tilemap *t, int tw, int th, int imgw, int imgh)
{
	int w = imgw / tw; // image width in tiles
	int h = imgh / th; // image height in tiles
	int iw, ih, i = 0; // just counters for loops
	
	for (iw  = 0; iw < w; iw++) {
		for (ih = 0; ih < h; ih++) {
			t->index[i][0] = iw * tw;
			t->index[i][1] = ih * th;
			t->index[i][2] = (iw + 1) * tw;
			t->index[i][3] = (ih + 1) * th;
			
			i++;
		}
	}
}

void newTilmapInternal(Tilemap *t, const char *name) {
	printf("== %s: %d\n", __FILE__, __LINE__);
	static unsigned char tmpMap[MAX_MAP_SIZE][MAX_MAP_SIZE];
	static char tmpStr[MAX_MAP_SIZE * 4];
	int tw, th;     // tile size (we get it from file header)
	int iw, ih;     // just indexes for loops
	int w, h;       // map size in tiles (we get it after index map reading)
	int id;         // storage for incoming index from file (while reading)
	if(!t->img->id)
	{
		MYERROR("Cannot get image for tile map");
		return;
	}
	int imgw = t->img->w;
	int imgh = t->img->h;
	printf("== %s: %d\n", __FILE__, __LINE__);
	if (name == NULL)
		MYERROR("Can't load tilemap without name");
	
	if (t == NULL)
		MYERROR("Map %s not initialised", name);
	printf("== %s: %d\n", __FILE__, __LINE__);
	FILE *f = fopen(name, "r");
	if (f == NULL)
		MYERROR("Can't open tilemap %s", name);
	printf("== %s: %d\n", __FILE__, __LINE__);
	if (fscanf(f, "%s %d %d\n", tmpStr, &tw, &th) != 3)
		MYERROR("Can't read tilemap's image and size from %s", name);
	printf("== %s: %d\n", __FILE__, __LINE__);
	// prepare
	printf("== %s: %d\n", __FILE__, __LINE__);
	// here maybe crash
	for (iw = 0; iw < MAX_MAP_SIZE; iw++)
		memset(tmpMap[iw], 0, MAX_MAP_SIZE * sizeof(char));
	printf("== %s: %d\n", __FILE__, __LINE__);
	calculateIndexes(t, tw, th, imgw, imgh);
	iw = ih = 0;
	printf("== %s: %d\n", __FILE__, __LINE__);
	// read index map to temp map (we don't know sizes)
	printf("== %s: %d\n", __FILE__, __LINE__);
	// read string
	while (fgets(tmpStr, MAX_MAP_SIZE * 4, f) != NULL) {
		char *pos = tmpStr;
		ih = 0;
		printf("== %s: %d\n", __FILE__, __LINE__);
		// read all numbers in string
		while (sscanf(pos, "%d\t", &id) == 1) {
			if (id < 0 || id > 255)
				MYERROR("Worng tilemap index from %s", name);
			//~ printf("== %s: %d\n", __FILE__, __LINE__);
			tmpMap[iw][ih] = id;
			//~ printf("== %s: %d\n", __FILE__, __LINE__);
			pos = strstr(pos, "\t");
			if (pos == NULL) break;
			ih++;
		}
		printf("== %s: %d\n", __FILE__, __LINE__);
		iw++;
		w = iw;
		if (h < ih) h = ih;
	}
	printf("== %s: %d\n", __FILE__, __LINE__);
	// copy temp map to our tilemap
	
	t->map = NULL;
	new(t->map, unsigned char *, w);
	for (iw = 0; iw < w; iw++) {
		t->map[iw] = NULL;
		new(t->map[iw], unsigned char, h);
	}
	printf("== %s: %d\n", __FILE__, __LINE__);
	for (iw = 0; iw < w; iw ++)
		for (ih = 0; ih < h; ih++)
			t->map[iw][ih] = tmpMap[iw][ih];
	printf("== %s: %d\n", __FILE__, __LINE__);
}

void tilemapDraw(Tilemap *t, float x, float y, float r, float z) {
	int i, j, k = 0;
	int x1, y1, x2, y2; // coords of visible part of tilemap (in tiles)
	
	x1 = y1 = 0; x2 = t->w; y2 = t->h;
	
	int camx_i = (int)x / t->tw; // we look at this tile
	int camy_i = (int)y / t->th; // we look at this tile
	
	i = screen->w / t->tw / 2; // half screen width in tiles
	j = screen->h / t->th / 2; // half screen height in tiles
	/*
	i = (float)i / tilemap.camzoom + 2; // apply zoom
	j = (float)j / tilemap.camzoom + 2; // apply zoom
	*/
	/*x1 = camx_i - i; // get left drawing edge in tiles
	x1 = x1 > 0 ? x1 : 0; // check if we draw from negative index
	x1 = x1 < t->w - 1 ? x1 : t->w - 1; // check if we draw from owerflow index
	
	x2 = camx_i + i;
	x2 = x2 > 0 ? x2 : 0;
	x2 = x2 < t->w - 1 ? x2 : t->w - 1;
	
	y1 = camy_i - j;
	y1 = y1 > 0 ? y1 : 0;
	y1 = y1 < t->h - 1 ? y1 : t->h - 1;
	
	y2 = camy_i + j;
	y2 = y2 > 0 ? y2 : 0;
	y2 = y2 < t->h - 1 ? y2 : t->h - 1;*/
	
	VERTEX_QUERY((x2 - x1) * (y2 - y1) * 4);
	move(x, y);
	rotate(r);
	scale(z);
	// draw bottom tiles
	for (i = x1; i < x2; i++) {
		for (j = y1; j < y2; j++) {
			VERTEX_COORD(i * t->tw, j * t->th, (i + 1) * t->tw, (j + 1) * t->th);
			memcpy(texCoord + k, t->index[t->map[i][j]], 8);
			k += 4;
		}
	}
	
	glEnable(GL_TEXTURE_2D);
	imageBind(t->img);
	glVertexPointer(2, GL_FLOAT, 0, vertexCoord);
	glTexCoordPointer(2, GL_FLOAT, 0, texCoordQuad);
	glDrawArrays(GL_QUADS, 0, k);
}

void deleteTilemap(Tilemap *t) {
	
}