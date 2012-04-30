#include "tilemap.h"

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

void calcuateIndexes(Tilemap *t, int tw, int th, int imgw, imgh);

void newTilmapInternal(Tilemap *t, char *name, int imgw, int imgh) {
	static unsigned char tmpMap[MAX_MAP_SIZE][MAX_MAP_SIZE];
	static char tmpStr[MAX_MAP_SIZE * 4];
	int tw, th;     // tile size (we get it from file header)
	int iw, ih;     // just indexes for loops
	int w, h;       // map size in tiles (we get it after index map reading)
	int id;         // storage for incoming index from file (while reading)
	
	if (name == NULL)
		MYERROR("Can't load tilemap without name");
	
	if (t == NULL)
		MYERROR("Map %s not initialised", name)
	
	FILE *f = fopen(name, "r");
	if (f == NULL)
		MYERROR("Can't open tilemap %s", name);
	
	if (fscanf(f, "%s %d %d", &tmpStr, &tw, &th) != 3)
		MYERROR("Can't read tilemap's image and size from %s", name);
	
	// prepare
	
	// here maybe crash
	for (iw = 0; iw < MAX_MAP_SIZE; iw++)
		memset(tmpMap[iw], 0, MAX_MAP_SIZE *sizeof(char))
	
	calculateIndexes(t, tw, th, imgw, imgh);
	iw = ih = 0;
	
	// read index map to temp map (we don't know sizes)
	
	// read string
	while (fgets(tmpStr, MAX_MAP_SIZE * 4, f) != NULL) {
		char *pos = tmpStr;
		ih = 0;
		
		// read all numbers in string
		while (sscanf(pos, "%d\t", &id) == 1) {
			if (id < 0 || id > 255)
				MYERROR("Worng tilemap index from %s", name);
			
			tmpMap[iw][ih] = id;
			
			pos = strstr(pos, " ");
			ih++;
		}
		
		iw++;
		w = iw;
		if (h < ih) h = ih;
	}
	
	// copy temp map to our tilemap
	
	t->map = NULL;
	new(t->map, unsigned char *, w);
	for (iw = 0; iw < w; iw++) {
		t->map[iw] = NULL;
		new(t->map[iw], unsigned char, h);
	}
	
	for (iw = 0; iw < w; iw ++)
		for (ih = 0; ih < h; ih++)
			t->map[iw][ih] = tmpMap[iw][ih];
}

void calcuateIndexes(Tilemap *t, int tw, int th, int imgw, int imgh)
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

void drawTilemap(Tilemap *t, float x, float y) {
	int i, j, k;
	int x1, y1, x2, y2; // coords of visible part of tilemap (in tiles)
	
	glEnable(GL_TEXTURE_2D);
	
	int camx_i = (int)x / t->tw; // we look at this tile
	int camy_i = (int)y / t->th; // we look at this tile
	
	i = screen->w / t->tw / 2; // half screen width in tiles
	j = screen->h / t->th / 2; // half screen height in tiles
	/*
	i = (float)i / tilemap.camzoom + 2; // apply zoom
	j = (float)j / tilemap.camzoom + 2; // apply zoom
	*/
	x1 = camx_i - i; // get left drawing edge in tiles
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
	y2 = y2 < t->h - 1 ? y2 : t->h - 1;
	
	// draw bottom tiles
	for (i = x1; i < x2; i++) {
		for (j = y1; j < y2; j++) {
			VERTEX_COORD(x,y,w,h);
			TEXTURE_COORD();
		}
	}
	
	imageBind(image);
	DRAWQT;
}

