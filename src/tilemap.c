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

void newTilmapInternal(Tilemap *t, const char *name) {
	int iw, ih, i = 0;  // just indexes for loops
	char tmpStr[256];
	
	if(!t->img->id)
	{
		MYERROR("Cannot get image for tile map");
		return;
	}
	
	if (name == NULL)
		MYERROR("Can't load tilemap without name");
	
	if (t == NULL)
		MYERROR("Map %s not initialised", name);
	
	FILE *f = fopen(name, "r");
	if (f == NULL)
		MYERROR("Can't open tilemap %s", name);
	
	if (fscanf(f, "%s %d %d %d %d\n", tmpStr, &t->w, &t->h, &t->tw, &t->th) != 5)
		MYERROR("Can't read tilemap's image and size from %s", name);
	
	// calculate indexes' texture coords
	i = 0;
	for (iw  = 0; iw < t->w; iw++) {
		for (ih = 0; ih < t->h; ih++) {
			t->index[i][0] = iw * t->tw;
			t->index[i][1] = ih * t->th;
			t->index[i][2] = (iw + 1) * t->tw;
			t->index[i][3] = (ih + 1) * t->th;
			
			i++;
		}
	}
	
	// get memory
	t->map = NULL;
	new(t->map, unsigned char *, t->w);
	for (i = 0; i < t->w; i++) {
		t->map[i] = NULL;
		new(t->map[i], unsigned char, t->h);
	}
	
	// read index map
	for (iw = 0; iw < t->w; iw++) {
		for (ih = 0; ih < t->h; ih++) {
			if (fscanf(f, "%d", &i) != 1)
				MYERROR("Can't read tilemap from %s", name);
			t->map[iw][ih] = (unsigned char)i;
		}
	}
	
	fclose(f);
}

//void tilemapDraw(Tilemap *t, float x, float y, float r, float z) {
void tilemapDraw(Tilemap *t, double x, double y, double r, double z) {
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
	
	ACCUM_START((x2 - x1) * (y2 - y1) * 4);
	
	glPushMatrix();
	glTranslated(x, y, 0);
	glRotated(r, 0, 0, 1);
	glScaled(z, z, 1);
	
	// draw bottom tiles
	for (i = x1; i < x2; i++) {
		for (j = y1; j < y2; j++) {
			ACCUM_VERTEX(i * t->tw, j * t->th, (i + 1) * t->tw, (j + 1) * t->th);
			ACCUM_TEXTURE_ARRAY(t->index[t->map[i][j]]);
			ACCUM_ADD();
			k += 4;
		}
	}
	
	glEnable(GL_TEXTURE_2D);
	imageBind(t->img);
	ACCUM_DRAW();
	glPopMatrix();
}

void deleteTilemap(Tilemap *t) {
	
}
