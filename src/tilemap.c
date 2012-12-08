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
#define INDEX_COUNT ((int)(t->img->w / t->tw) * (int)(t->img->h / t->th))
void newTilmapInternal(Tilemap *t, const char *name) {
	//~ int iw, ih, i = 0;  // just indexes for loops
	//~ 
	//~ if(!t->img->id)
	//~ {
		//~ MYERROR("Cannot get image for tile map");
		//~ return;
	//~ }
	//~ 
	//~ if(name == NULL)
	//~ {
		//~ MYERROR("Can't load tilemap without name");
		//~ return;
	//~ }
	//~ 
	//~ if(t == NULL)
	//~ {
		//~ MYERROR("Map %s not initialised", name);
		//~ return;
	//~ }
	//~ 
	//~ FILE *f = fopen(name, "r");
	//~ if(f == NULL)
	//~ {
		//~ MYERROR("Can't open tilemap %s", name);
		//~ return;
	//~ }
	//~ 
	//~ //easy way to skip line
	//~ do {} while(fgetc(f) != '\n');
	//~ 
	//~ // get mem for index
	//~ t->index = NULL;
	//~ new(t->index, float *, INDEX_COUNT);
	//~ for (i = 0; i < INDEX_COUNT; i++) {
		//~ t->index[i] = NULL;
		//~ new(t->index[i], float, 8);
	//~ }
	//~ 
	//~ // calculate indexes' texture coords
	//~ i = 0;
	//~ for (iw  = 0; iw < t->img->w / t->tw; iw++) {
		//~ for (ih = 0; ih < t->img->h / t->th; ih++) {
			//~ t->index[i][0] = (iw * t->tw) / t->img->w;
			//~ t->index[i][1] = (ih * t->th) / t->img->h;
			//~ t->index[i][2] = t->index[i][0];
			//~ t->index[i][3] = t->index[i][1] + t->th / t->img->h;
			//~ t->index[i][4] = t->index[i][0] + t->tw / t->img->w;
			//~ t->index[i][5] = t->index[i][3];
			//~ t->index[i][6] = t->index[i][4];
			//~ t->index[i][7] = t->index[i][1];
			//~ 
			//~ i++;
		//~ }
	//~ }
	//~ 
	//~ // get memory
	//~ t->map = NULL;
	//~ new(t->map, unsigned char *, t->w);
	//~ for (i = 0; i < t->w; i++) {
		//~ t->map[i] = NULL;
		//~ new(t->map[i], unsigned char, t->h);
	//~ }
	//~ 
	//~ // read index map
	//~ for (iw = 0; iw < t->w; iw++) {
		//~ for (ih = 0; ih < t->h; ih++) {
			//~ if (fscanf(f, "%d", &i) != 1)
			//~ {
				//~ MYERROR("Can't read tilemap from %s", name);
				//~ return;
			//~ }
			//~ t->map[iw][ih] = (unsigned char)i;
		//~ }
	//~ }
	//~ 
	//~ fclose(f);
}

//void tilemapDraw(Tilemap *t, float x, float y, float r, float z) {
void tilemapDraw(Tilemap *t, double x, double y, double r, double z, double ox, double oy) {
	//~ int i, j, k = 0;
	//~ int x1, y1, x2, y2; // coords of visible part of tilemap (in tiles)
	//~ 
	//~ x1 = y1 = 0; x2 = t->w; y2 = t->h;
	//~ 
	//~ int camx_i = (int)x / t->tw; // we look at this tile
	//~ int camy_i = (int)y / t->th; // we look at this tile
	//~ 
	//~ i = screen->w / t->tw / 2; // half screen width in tiles
	//~ j = screen->h / t->th / 2; // half screen height in tiles
	//~ i = j = (i > j ? i : j); // i = j = max(i, j)
	//~ 
	//~ i = j = (float)i / z + 2; // apply zoom
	//~ 
	//~ x1 = camx_i - i; // get left drawing edge in tiles
	//~ x1 = x1 > 0 ? x1 : 0; // check if we draw from negative index
	//~ x1 = x1 < t->w - 1 ? x1 : t->w - 1; // check if we draw from owerflow index
	//~ 
	//~ x2 = camx_i + i;
	//~ x2 = x2 > 0 ? x2 : 0;
	//~ x2 = x2 < t->w - 1 ? x2 : t->w - 1;
	//~ 
	//~ y1 = camy_i - j;
	//~ y1 = y1 > 0 ? y1 : 0;
	//~ y1 = y1 < t->h - 1 ? y1 : t->h - 1;
	//~ 
	//~ y2 = camy_i + j;
	//~ y2 = y2 > 0 ? y2 : 0;
	//~ y2 = y2 < t->h - 1 ? y2 : t->h - 1;
	//~ 
	//~ ACCUM_START((x2 - x1) * (y2 - y1) * 4);
	//~ 
	//~ glPushMatrix();
	//~ if (x || y) glTranslated(x, y, 0);
	//~ if (r) glRotated(r, 0, 0, 1);
	//~ if (z) glScalef(z, z, 1);
	//~ if (ox || oy) glTranslated(-ox / z, -oy / z, 0);
	//~ 
	//~ // draw bottom tiles
	//~ for (i = x1; i < x2; i++) {
		//~ for (j = y1; j < y2; j++) {
			//~ ACCUM_VERTEX(i * t->tw, j * t->th, t->tw, t->th);
			//~ ACCUM_TEXTURE_ARRAY(t->index[t->map[i][j]]);
			//~ ACCUM_ADD();
			//~ k += 4;
		//~ }
	//~ }
	//~ imageBind(t->img);
	//~ ACCUM_DRAW();
	//~ glPopMatrix();
}

void deleteTilemap(Tilemap *t) {
	//~ int i;
	//~ for (i = 0; i < INDEX_COUNT; i++) delete(t->index[i]);
	//~ delete(t->index);
	//~ 
	//~ for (i = 0; i < t->w; i++) delete(t->map[i]);
	//~ delete(t->map);
}
