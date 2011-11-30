//~ #include "cheetah.h"
//~ 
//~ entity * newEntity(entity * parent)
//~ {
	//~ entity *e = (entity *)malloc((size_t)sizeof(entity));
	//~ if(!e) error("out of memory");
	//~ e->x = e->y = 0.0f;
	//~ e->visible = 1;
	//~ e->next = e->child = 0;
	//~ if(parent->child)
	//~ {
		//~ e->prev = parent->child->prev;
		//~ parent->child->prev->next = e;
		//~ parent->child->prev = e;
	//~ }
	//~ else
	//~ {
		//~ parent->child = e;
		//~ e->prev = e;
	//~ }
	//~ return e;
//~ }
//~ 
//~ 
//~ //process all entity tree recursively
//~ void processEntities(entity * top)
//~ {
	//~ while(top)
	//~ {
		//~ glColor4ubv(top->col);
		//~ glBegin(GL_POINTS);
		//~ glVertex2f(top->x, top->y);
		//~ glEnd();
		//~ if(top->child) processEntities(top->child);
		//~ top = top->next;
	//~ }
//~ }