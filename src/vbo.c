/*******************************************************************************

Copyright (c) 2012 Pavel Roschin (aka RPG) <rpg89@post.ru>

Permission is hereby granted, free of charge, to any person obtaining a copy 
of this software and associated documentation files (the "Software"), to 
deal in the Software without restriction, including without limitation the 
rights to use, copy, modify, merge, publish, distribute, sublicense, and/or 
sell copies of the Software, and to permit persons to whom the Software is 
furnished to do so, subject to the following conditions:  The above 
copyright notice and this permission notice shall be included in all copies 
or substantial portions of the Software.
 
THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR 
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, 
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE 
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER 
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING 
FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS 
IN THE SOFTWARE.

*******************************************************************************/

#include "cheetah.h"
#include "render.h"

Vbo * newVbo(Point * data, Point * tex, unsigned int count) {
	Vbo *ptr = NULL;
	new(ptr, Vbo, 1);
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
	//~ glEnable(GL_TEXTURE_2D);
	glEnableClientState(GL_VERTEX_ARRAY); 
	glEnableClientState(GL_TEXTURE_COORD_ARRAY); 
	glBindBuffer_(GL_ARRAY_BUFFER_ARB, ptr->id);
	glVertexPointer(2, GL_FLOAT, 0, (char *) NULL);
	glBindBuffer_(GL_ARRAY_BUFFER_ARB, ptr->tex);
	glTexCoordPointer(2, GL_FLOAT, 0, (char *) NULL);
	glDrawArrays(GL_QUADS, 0, ptr->count*4); 
	glDisableClientState(GL_VERTEX_ARRAY);
	glDisableClientState(GL_TEXTURE_COORD_ARRAY);
	//~ glDisable(GL_TEXTURE_2D);
}

Vbo * newVboPoints(Point * data, unsigned int count) {
	Vbo *ptr = NULL;
	new(ptr, Vbo, 1);
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
	//~ glEnable(GL_TEXTURE_2D);
	imageBind(img);
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
			glScalef(size, size, 1);
			glCallList(quadlist);
			glPopMatrix();
		}
	}
	
	//~ glDisable(GL_TEXTURE_2D);
}

Vbo * newVboPoints3(Point3 * data, unsigned int count) {
	Vbo *ptr = NULL;
	new(ptr, Vbo, 1);
	ptr->count = count;
	glGenBuffers_(1, &ptr->id);
	glBindBuffer_(GL_ARRAY_BUFFER_ARB, ptr->id);
	glBufferData_(GL_ARRAY_BUFFER_ARB, sizeof(Point3)*count, (void*)data, GL_STATIC_DRAW_ARB);
	return ptr;
}
void vboDrawSprites3(Vbo * ptr, Image * img, float size) {
	//~ glEnable(GL_TEXTURE_2D);
	imageBind(img);
	glPointSize(size);
	glEnable(GL_POINT_SPRITE);
	glTexEnvi(GL_POINT_SPRITE, GL_COORD_REPLACE, GL_TRUE);
	glEnableClientState(GL_VERTEX_ARRAY);
	glBindBuffer_(GL_ARRAY_BUFFER_ARB, ptr->id);
	glVertexPointer(3, GL_FLOAT, 0, (char *) NULL);
	glDrawArrays(GL_POINTS, 0, ptr->count); 
	glDisableClientState(GL_VERTEX_ARRAY);
	
	//~ glDisable(GL_TEXTURE_2D);
}

void deleteVbo(Vbo * ptr) {
	if(ptr) {
		glDeleteBuffers_(1, &ptr->id);
	}
	else myError("Trying to free a null-VBO. Maybe, you did it manually?");
}
