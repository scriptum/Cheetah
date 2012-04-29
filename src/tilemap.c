#include "tilemap.h"

/*
typedef struct _Tilemap {
	int w, h;            // size in tiles
        int tw, th;          // single tile size
	float index[256][4]; // texture coords index
	unsigned char **map; // tile indexes map
	int scalable;        // should tilemap be scaled to screen size or drawed per-pixel
} Tilemap;
*/

void calcuateIndexes(Tilemap *t, int tw, int th, int imgw, imgh);

Tilemap *loadTileMap(char *name) {
	static unsigned char tmpMap[MAX_MAP_SIZE][MAX_MAP_SIZE];
	static char tmpStr[MAX_MAP_SIZE * 4];
	char timg[1024];
	int tw, th;     // tile size (we get it from file header)
	int iw, ih;     // just indexes for loops
	int w, h;       // map size in tiles (we get it after index map reading)
	int imgw, imgh; // image size (we DONT GET IT!!!)
	int id;         // storage for incoming index from file (while reading)
	
	if (name == NULL)
		myError("Can't load tilemap without name");
	
	FILE *f = fopen(name, "r");
	if (f == NULL)
		myError("Can't open tilemap %s", name);
	
	if (fscanf(f, "%s %d %d", &timg, &tw, &th) != 3)
		myError("Can't read tilemap's image and size from %s", name);
	
	// allocate mem
	
	Tilemap *t = NULL;
	new(t, Tilemap, 1);
	
	// prepare
	
	for (iw = 0; iw < MAX_MAP_SIZE; iw ++)
		for (ih = 0; ih < MAX_MAP_SIZE; ih++)
			tmpMap[iw][ih] = 0;
	
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
				myError("Worng tilemap index from %s", name);
			
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

