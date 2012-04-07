/*static inline\s*([a-zA-Z0-9*]+)\s+([a-zA-Z0-9_]+)\s*(\((:?[^)]|\n)*\))\s*\{([^}]|\n)*\};
?
\1(*_\2)\3;
*/
void Message(const char *condition, const char *file, int line, int isError, int isHardError, const char *message, ...) asm("cpMessage");

enum
  {
    FP_NAN,
    FP_INFINITE,
    FP_ZERO,
    FP_SUBNORMAL,
    FP_NORMAL
  };
typedef enum
{
  _IEEE_ = -1,
  _SVID_,
  _XOPEN_,
  _POSIX_,
  _ISOC_
} _LIB_VERSION_TYPE;
extern _LIB_VERSION_TYPE _LIB_VERSION;
struct exception
{
	int type;
	char *name;
	double arg1;
	double arg2;
	double retval;
};
extern int matherr (struct exception *__exc);

typedef double cpFloat;
cpFloat (*fmax)(cpFloat a, cpFloat b) asm("_cpfmax");
cpFloat (*fmin)(cpFloat a, cpFloat b) asm("_cpfmin");
cpFloat (*fabs)(cpFloat f) asm("_cpfabs");
cpFloat (*fclamp)(cpFloat f, cpFloat min, cpFloat max) asm("_cpfclamp");
cpFloat (*fclamp01)(cpFloat f) asm("_cpfclamp01");
cpFloat (*flerp)(cpFloat f1, cpFloat f2, cpFloat t) asm("_cpflerp");
cpFloat (*flerpconst)(cpFloat f1, cpFloat f2, cpFloat d) asm("_cpflerpconst");
typedef uintptr_t cpHashValue;
typedef int cpBool;
typedef void * cpDataPointer;
typedef uintptr_t cpCollisionType;
typedef uintptr_t cpGroup;
typedef unsigned int cpLayers;
typedef unsigned int cpTimestamp;
typedef struct cpVect{cpFloat x,y;} cpVect;
typedef struct cpArray cpArray;
typedef struct cpHashSet cpHashSet;
typedef struct cpBody cpBody;
typedef struct cpShape cpShape;
typedef struct cpConstraint cpConstraint;
typedef struct cpCollisionHandler cpCollisionHandler;
typedef struct cpArbiter cpArbiter;
typedef struct cpSpace cpSpace;
cpVect cpvzero;
cpVect (*v)(const cpFloat x, const cpFloat y) asm("_cpv");
cpFloat vlength(const cpVect v) asm("cpvlength");
cpVect vslerp(const cpVect v1, const cpVect v2, const cpFloat t) asm("cpvslerp");
cpVect vslerpconst(const cpVect v1, const cpVect v2, const cpFloat a) asm("cpvslerpconst");
cpVect vforangle(const cpFloat a) asm("cpvforangle");
cpFloat vtoangle(const cpVect v) asm("cpvtoangle");
char* vstr(const cpVect v) asm("cpvstr");
cpBool (*veql)(const cpVect v1, const cpVect v2) asm("_cpveql");
cpVect (*vadd)(const cpVect v1, const cpVect v2) asm("_cpvadd");
cpVect (*vsub)(const cpVect v1, const cpVect v2) asm("_cpvsub");
cpVect (*vneg)(const cpVect v) asm("_cpvneg");
cpVect (*vmult)(const cpVect v, const cpFloat s) asm("_cpvmult");
cpFloat (*vdot)(const cpVect v1, const cpVect v2) asm("_cpvdot");
cpFloat (*vcross)(const cpVect v1, const cpVect v2) asm("_cpvcross");
cpVect (*vperp)(const cpVect v) asm("_cpvperp");
cpVect (*vrperp)(const cpVect v) asm("_cpvrperp");
cpVect (*vproject)(const cpVect v1, const cpVect v2) asm("_cpvproject");
cpVect (*vrotate)(const cpVect v1, const cpVect v2) asm("_cpvrotate");
cpVect (*vunrotate)(const cpVect v1, const cpVect v2) asm("_cpvunrotate");
cpFloat (*vlengthsq)(const cpVect v) asm("_cpvlengthsq");
cpVect (*vlerp)(const cpVect v1, const cpVect v2, const cpFloat t) asm("_cpvlerp");
cpVect (*vnormalize)(const cpVect v) asm("_cpvnormalize");
cpVect (*vnormalize_safe)(const cpVect v) asm("_cpvnormalize_safe");
cpVect (*vclamp)(const cpVect v, const cpFloat len) asm("_cpvclamp");
cpVect (*vlerpconst)(cpVect v1, cpVect v2, cpFloat d) asm("_cpvlerpconst");
cpFloat (*vdist)(const cpVect v1, const cpVect v2) asm("_cpvdist");
cpFloat (*vdistsq)(const cpVect v1, const cpVect v2) asm("_cpvdistsq");
cpBool (*vnear)(const cpVect v1, const cpVect v2, const cpFloat dist) asm("_cpvnear");
typedef struct cpBB{
 cpFloat l, b, r ,t;
} cpBB;
cpBB (*BBNew)(const cpFloat l, const cpFloat b, const cpFloat r, const cpFloat t) asm("_cpBBNew");
cpBB (*BBNewForCircle)(const cpVect p, const cpFloat r) asm("_cpBBNewForCircle");
cpBool (*BBIntersects)(const cpBB a, const cpBB b) asm("_cpBBIntersects");
cpBool (*BBContainsBB)(const cpBB bb, const cpBB other) asm("_cpBBContainsBB");
cpBool (*BBContainsVect)(const cpBB bb, const cpVect v) asm("_cpBBContainsVect");
cpBB (*BBMerge)(const cpBB a, const cpBB b) asm("_cpBBMerge");
cpBB (*BBExpand)(const cpBB bb, const cpVect v) asm("_cpBBExpand");
cpFloat (*BBArea)(cpBB bb) asm("_cpBBArea");
cpFloat (*BBMergedArea)(cpBB a, cpBB b) asm("_cpBBMergedArea");
cpFloat (*BBSegmentQuery)(cpBB bb, cpVect a, cpVect b) asm("_cpBBSegmentQuery");
cpBool (*BBIntersectsSegment)(cpBB bb, cpVect a, cpVect b) asm("_cpBBIntersectsSegment");
cpVect BBClampVect(const cpBB bb, const cpVect v) asm("cpBBClampVect");
cpVect BBWrapVect(const cpBB bb, const cpVect v) asm("cpBBWrapVect");
typedef cpBB (*cpSpatialIndexBBFunc)(void *obj);
typedef void (*cpSpatialIndexIteratorFunc)(void *obj, void *data);
typedef void (*cpSpatialIndexQueryFunc)(void *obj1, void *obj2, void *data);
typedef cpFloat (*cpSpatialIndexSegmentQueryFunc)(void *obj1, void *obj2, void *data);
typedef struct cpSpatialIndexClass cpSpatialIndexClass;
typedef struct cpSpatialIndex cpSpatialIndex;
struct cpSpatialIndex {
 cpSpatialIndexClass *klass;
 cpSpatialIndexBBFunc bbfunc;
 cpSpatialIndex *staticIndex, *dynamicIndex;
};
typedef struct cpSpaceHash cpSpaceHash;
cpSpaceHash* SpaceHashAlloc(void) asm("cpSpaceHashAlloc");
cpSpatialIndex* SpaceHashInit(cpSpaceHash *hash, cpFloat celldim, int numcells, cpSpatialIndexBBFunc bbfunc, cpSpatialIndex *staticIndex) asm("cpSpaceHashInit");
cpSpatialIndex* SpaceHashNew(cpFloat celldim, int cells, cpSpatialIndexBBFunc bbfunc, cpSpatialIndex *staticIndex) asm("cpSpaceHashNew");
void SpaceHashResize(cpSpaceHash *hash, cpFloat celldim, int numcells) asm("cpSpaceHashResize");
typedef struct cpBBTree cpBBTree;
cpBBTree* BBTreeAlloc(void) asm("cpBBTreeAlloc");
cpSpatialIndex* BBTreeInit(cpBBTree *tree, cpSpatialIndexBBFunc bbfunc, cpSpatialIndex *staticIndex) asm("cpBBTreeInit");
cpSpatialIndex* BBTreeNew(cpSpatialIndexBBFunc bbfunc, cpSpatialIndex *staticIndex) asm("cpBBTreeNew");
void BBTreeOptimize(cpSpatialIndex *index) asm("cpBBTreeOptimize");
typedef cpVect (*cpBBTreeVelocityFunc)(void *obj);
void BBTreeSetVelocityFunc(cpSpatialIndex *index, cpBBTreeVelocityFunc func) asm("cpBBTreeSetVelocityFunc");
typedef struct cpSweep1D cpSweep1D;
cpSweep1D* Sweep1DAlloc(void) asm("cpSweep1DAlloc");
cpSpatialIndex* Sweep1DInit(cpSweep1D *sweep, cpSpatialIndexBBFunc bbfunc, cpSpatialIndex *staticIndex) asm("cpSweep1DInit");
cpSpatialIndex* Sweep1DNew(cpSpatialIndexBBFunc bbfunc, cpSpatialIndex *staticIndex) asm("cpSweep1DNew");
typedef void (*cpSpatialIndexDestroyImpl)(cpSpatialIndex *index);
typedef int (*cpSpatialIndexCountImpl)(cpSpatialIndex *index);
typedef void (*cpSpatialIndexEachImpl)(cpSpatialIndex *index, cpSpatialIndexIteratorFunc func, void *data);
typedef cpBool (*cpSpatialIndexContainsImpl)(cpSpatialIndex *index, void *obj, cpHashValue hashid);
typedef void (*cpSpatialIndexInsertImpl)(cpSpatialIndex *index, void *obj, cpHashValue hashid);
typedef void (*cpSpatialIndexRemoveImpl)(cpSpatialIndex *index, void *obj, cpHashValue hashid);
typedef void (*cpSpatialIndexReindexImpl)(cpSpatialIndex *index);
typedef void (*cpSpatialIndexReindexObjectImpl)(cpSpatialIndex *index, void *obj, cpHashValue hashid);
typedef void (*cpSpatialIndexReindexQueryImpl)(cpSpatialIndex *index, cpSpatialIndexQueryFunc func, void *data);
typedef void (*cpSpatialIndexPointQueryImpl)(cpSpatialIndex *index, cpVect point, cpSpatialIndexQueryFunc func, void *data);
typedef void (*cpSpatialIndexSegmentQueryImpl)(cpSpatialIndex *index, void *obj, cpVect a, cpVect b, cpFloat t_exit, cpSpatialIndexSegmentQueryFunc func, void *data);
typedef void (*cpSpatialIndexQueryImpl)(cpSpatialIndex *index, void *obj, cpBB bb, cpSpatialIndexQueryFunc func, void *data);
struct cpSpatialIndexClass {
 cpSpatialIndexDestroyImpl destroy;
 cpSpatialIndexCountImpl count;
 cpSpatialIndexEachImpl each;
 cpSpatialIndexContainsImpl contains;
 cpSpatialIndexInsertImpl insert;
 cpSpatialIndexRemoveImpl remove;
 cpSpatialIndexReindexImpl reindex;
 cpSpatialIndexReindexObjectImpl reindexObject;
 cpSpatialIndexReindexQueryImpl reindexQuery;
 cpSpatialIndexPointQueryImpl pointQuery;
 cpSpatialIndexSegmentQueryImpl segmentQuery;
 cpSpatialIndexQueryImpl query;
};
void SpatialIndexFree(cpSpatialIndex *index) asm("cpSpatialIndexFree");
void SpatialIndexCollideStatic(cpSpatialIndex *dynamicIndex, cpSpatialIndex *staticIndex, cpSpatialIndexQueryFunc func, void *data) asm("cpSpatialIndexCollideStatic");
void (*SpatialIndexDestroy)(cpSpatialIndex *index) asm("_cpSpatialIndexDestroy");
int (*SpatialIndexCount)(cpSpatialIndex *index) asm("_cpSpatialIndexCount");
void (*SpatialIndexEach)(cpSpatialIndex *index, cpSpatialIndexIteratorFunc func, void *data) asm("_cpSpatialIndexEach");
cpBool (*SpatialIndexContains)(cpSpatialIndex *index, void *obj, cpHashValue hashid) asm("_cpSpatialIndexContains");
void (*SpatialIndexInsert)(cpSpatialIndex *index, void *obj, cpHashValue hashid) asm("_cpSpatialIndexInsert");
void (*SpatialIndexRemove)(cpSpatialIndex *index, void *obj, cpHashValue hashid) asm("_cpSpatialIndexRemove");
void (*SpatialIndexReindex)(cpSpatialIndex *index) asm("_cpSpatialIndexReindex");
void (*SpatialIndexReindexObject)(cpSpatialIndex *index, void *obj, cpHashValue hashid) asm("_cpSpatialIndexReindexObject");
void (*SpatialIndexPointQuery)(cpSpatialIndex *index, cpVect point, cpSpatialIndexQueryFunc func, void *data) asm("_cpSpatialIndexPointQuery");
void (*SpatialIndexSegmentQuery)(cpSpatialIndex *index, void *obj, cpVect a, cpVect b, cpFloat t_exit, cpSpatialIndexSegmentQueryFunc func, void *data) asm("_cpSpatialIndexSegmentQuery");
void (*SpatialIndexQuery)(cpSpatialIndex *index, void *obj, cpBB bb, cpSpatialIndexQueryFunc func, void *data) asm("_cpSpatialIndexQuery");
void (*SpatialIndexReindexQuery)(cpSpatialIndex *index, cpSpatialIndexQueryFunc func, void *data) asm("_cpSpatialIndexReindexQuery");
typedef void (*cpBodyVelocityFunc)(cpBody *body, cpVect gravity, cpFloat damping, cpFloat dt);
typedef void (*cpBodyPositionFunc)(cpBody *body, cpFloat dt);
typedef struct cpComponentNode {
 cpBody *root;
 cpBody *next;
 cpFloat idleTime;
} cpComponentNode;
struct cpBody {
 cpBodyVelocityFunc velocity_func;
 cpBodyPositionFunc position_func;
 cpFloat m; //mass
 cpFloat m_inv;
 cpFloat i; //moment
 cpFloat i_inv;
 cpVect p; //pos
 cpVect v; //vel
 cpVect f; //force
 cpFloat a; //angle
 cpFloat w; //angvel
 cpFloat t; //torque
 cpVect rot; //rot???
 cpDataPointer data;
 cpFloat v_limit;
 cpFloat w_limit;
 cpVect v_bias_private;
 cpFloat w_bias_private;
 cpSpace *space_private;
 cpShape *shapeList_private;
 cpArbiter *arbiterList_private;
 cpConstraint *constraintList_private;
 cpComponentNode node_private;
};
cpBody* BodyAlloc(void) asm("cpBodyAlloc");
cpBody* BodyInit(cpBody *body, cpFloat m, cpFloat i) asm("cpBodyInit");
cpBody* BodyNew(cpFloat m, cpFloat i) asm("cpBodyNew");
cpBody* BodyInitStatic(cpBody *body) asm("cpBodyInitStatic");
cpBody* BodyNewStatic(void) asm("cpBodyNewStatic");
void BodyDestroy(cpBody *body) asm("cpBodyDestroy");
void BodyFree(cpBody *body) asm("cpBodyFree");
 void BodySanityCheck(cpBody *body) asm("cpBodySanityCheck");
void BodyActivate(cpBody *body) asm("cpBodyActivate");
void BodyActivateStatic(cpBody *body, cpShape *filter) asm("cpBodyActivateStatic");
void BodySleep(cpBody *body) asm("cpBodySleep");
void BodySleepWithGroup(cpBody *body, cpBody *group) asm("cpBodySleepWithGroup");
cpBool (*BodyIsSleeping)(const cpBody *body) asm("_cpBodyIsSleeping");
cpBool (*BodyIsStatic)(const cpBody *body) asm("_cpBodyIsStatic");
cpBool (*BodyIsRogue)(const cpBody *body) asm("_cpBodyIsRogue");
cpFloat (*BodyGetMass)(const cpBody *body) asm("_cpBodyGetMass");
void BodySetMass(cpBody *body, cpFloat m) asm("cpBodySetMass");
cpFloat (*BodyGetMoment)(const cpBody *body) asm("_cpBodyGetMoment");
void BodySetMoment(cpBody *body, cpFloat i) asm("cpBodySetMoment");
cpVect (*BodyGetPos)(const cpBody *body) asm("_cpBodyGetPos");
void BodySetPos(cpBody *body, cpVect pos) asm("cpBodySetPos");
cpVect (*BodyGetVel)(const cpBody *body) asm("_cpBodyGetVel");
 void (*BodySetVel)(cpBody *body, const cpVect value) asm("_cpBodySetVel");
cpVect (*BodyGetForce)(const cpBody *body) asm("_cpBodyGetForce");
 void (*BodySetForce)(cpBody *body, const cpVect value) asm("_cpBodySetForce");
cpFloat (*BodyGetAngle)(const cpBody *body) asm("_cpBodyGetAngle");
void BodySetAngle(cpBody *body, cpFloat a) asm("cpBodySetAngle");
cpFloat (*BodyGetAngVel)(const cpBody *body) asm("_cpBodyGetAngVel");
 void (*BodySetAngVel)(cpBody *body, const cpFloat value) asm("_cpBodySetAngVel");
cpFloat (*BodyGetTorque)(const cpBody *body) asm("_cpBodyGetTorque");
 void (*BodySetTorque)(cpBody *body, const cpFloat value) asm("_cpBodySetTorque");
cpVect (*BodyGetRot)(const cpBody *body) asm("_cpBodyGetRot");
cpFloat (*BodyGetVelLimit)(const cpBody *body) asm("_cpBodyGetVelLimit");
 void (*BodySetVelLimit)(cpBody *body, const cpFloat value) asm("_cpBodySetVelLimit");
cpFloat (*BodyGetAngVelLimit)(const cpBody *body) asm("_cpBodyGetAngVelLimit");
 void (*BodySetAngVelLimit)(cpBody *body, const cpFloat value) asm("_cpBodySetAngVelLimit");
cpDataPointer (*BodyGetUserData)(const cpBody *body) asm("_cpBodyGetUserData");
 void (*BodySetUserData)(cpBody *body, const cpDataPointer value) asm("_cpBodySetUserData");
void BodyUpdateVelocity(cpBody *body, cpVect gravity, cpFloat damping, cpFloat dt) asm("cpBodyUpdateVelocity");
void BodyUpdatePosition(cpBody *body, cpFloat dt) asm("cpBodyUpdatePosition");
cpVect (*BodyLocal2World)(const cpBody *body, const cpVect v) asm("_cpBodyLocal2World");
cpVect (*BodyWorld2Local)(const cpBody *body, const cpVect v) asm("_cpBodyWorld2Local");
void BodyResetForces(cpBody *body) asm("cpBodyResetForces");
void BodyApplyForce(cpBody *body, const cpVect f, const cpVect r) asm("cpBodyApplyForce");
void BodyApplyImpulse(cpBody *body, const cpVect j, const cpVect r) asm("cpBodyApplyImpulse");
cpVect BodyGetVelAtWorldPoint(cpBody *body, cpVect point) asm("cpBodyGetVelAtWorldPoint");
cpVect BodyGetVelAtLocalPoint(cpBody *body, cpVect point) asm("cpBodyGetVelAtLocalPoint");
cpFloat (*BodyKineticEnergy)(const cpBody *body) asm("_cpBodyKineticEnergy");
typedef void (*cpBodyShapeIteratorFunc)(cpBody *body, cpShape *shape, void *data);
void BodyEachShape(cpBody *body, cpBodyShapeIteratorFunc func, void *data) asm("cpBodyEachShape");
typedef void (*cpBodyConstraintIteratorFunc)(cpBody *body, cpConstraint *constraint, void *data);
void BodyEachConstraint(cpBody *body, cpBodyConstraintIteratorFunc func, void *data) asm("cpBodyEachConstraint");
typedef void (*cpBodyArbiterIteratorFunc)(cpBody *body, cpArbiter *arbiter, void *data);
void BodyEachArbiter(cpBody *body, cpBodyArbiterIteratorFunc func, void *data) asm("cpBodyEachArbiter");
typedef struct cpShapeClass cpShapeClass;
typedef struct cpSegmentQueryInfo {
 cpShape *shape;
 cpFloat t;
 cpVect n;
} cpSegmentQueryInfo;
typedef enum cpShapeType{
 CP_CIRCLE_SHAPE,
 CP_SEGMENT_SHAPE,
 CP_POLY_SHAPE,
 CP_NUM_SHAPES
} cpShapeType;
typedef cpBB (*cpShapeCacheDataImpl)(cpShape *shape, cpVect p, cpVect rot);
typedef void (*cpShapeDestroyImpl)(cpShape *shape);
typedef cpBool (*cpShapePointQueryImpl)(cpShape *shape, cpVect p);
typedef void (*cpShapeSegmentQueryImpl)(cpShape *shape, cpVect a, cpVect b, cpSegmentQueryInfo *info);
struct cpShapeClass {
 cpShapeType type;
 cpShapeCacheDataImpl cacheData;
 cpShapeDestroyImpl destroy;
 cpShapePointQueryImpl pointQuery;
 cpShapeSegmentQueryImpl segmentQuery;
};
struct cpShape {
 const cpShapeClass *klass_private;
 cpBody *body;
 cpBB bb;
 cpBool sensor;
 cpFloat e;
 cpFloat u;
 cpVect surface_v;
 cpDataPointer data;
 cpCollisionType collision_type;
 cpGroup group;
 cpLayers layers;
 cpSpace *space_private;
 cpShape *next_private;
 cpShape *prev_private;
 cpHashValue hashid_private;
};
void ShapeDestroy(cpShape *shape) asm("cpShapeDestroy");
void ShapeFree(cpShape *shape) asm("cpShapeFree");
cpBB ShapeCacheBB(cpShape *shape) asm("cpShapeCacheBB");
cpBB ShapeUpdate(cpShape *shape, cpVect pos, cpVect rot) asm("cpShapeUpdate");
cpBool ShapePointQuery(cpShape *shape, cpVect p) asm("cpShapePointQuery");
cpBody* (*ShapeGetBody)(const cpShape *shape) asm("_cpShapeGetBody");
void ShapeSetBody(cpShape *shape, cpBody *body) asm("cpShapeSetBody");
cpBB (*ShapeGetBB)(const cpShape *shape) asm("_cpShapeGetBB");
cpBool (*ShapeGetSensor)(const cpShape *shape) asm("_cpShapeGetSensor");
 void (*ShapeSetSensor)(cpShape *shape, cpBool value) asm("_cpShapeSetSensor");
cpFloat (*ShapeGetElasticity)(const cpShape *shape) asm("_cpShapeGetElasticity");
 void (*ShapeSetElasticity)(cpShape *shape, cpFloat value) asm("_cpShapeSetElasticity");
cpFloat (*ShapeGetFriction)(const cpShape *shape) asm("_cpShapeGetFriction");
 void (*ShapeSetFriction)(cpShape *shape, cpFloat value) asm("_cpShapeSetFriction");
cpVect (*ShapeGetSurfaceVelocity)(const cpShape *shape) asm("_cpShapeGetSurfaceVelocity");
 void (*ShapeSetSurfaceVelocity)(cpShape *shape, cpVect value) asm("_cpShapeSetSurfaceVelocity");
cpDataPointer (*ShapeGetUserData)(const cpShape *shape) asm("_cpShapeGetUserData");
 void (*ShapeSetUserData)(cpShape *shape, cpDataPointer value) asm("_cpShapeSetUserData");
cpCollisionType (*ShapeGetCollisionType)(const cpShape *shape) asm("_cpShapeGetCollisionType");
 void (*ShapeSetCollisionType)(cpShape *shape, cpCollisionType value) asm("_cpShapeSetCollisionType");
cpGroup (*ShapeGetGroup)(const cpShape *shape) asm("_cpShapeGetGroup");
 void (*ShapeSetGroup)(cpShape *shape, cpGroup value) asm("_cpShapeSetGroup");
cpLayers (*ShapeGetLayers)(const cpShape *shape) asm("_cpShapeGetLayers");
 void (*ShapeSetLayers)(cpShape *shape, cpLayers value) asm("_cpShapeSetLayers");
void ResetShapeIdCounter(void) asm("cpResetShapeIdCounter");
cpBool ShapeSegmentQuery(cpShape *shape, cpVect a, cpVect b, cpSegmentQueryInfo *info) asm("cpShapeSegmentQuery");
cpVect (*SegmentQueryHitPoint)(const cpVect start, const cpVect end, const cpSegmentQueryInfo info) asm("_cpSegmentQueryHitPoint");
cpFloat (*SegmentQueryHitDist)(const cpVect start, const cpVect end, const cpSegmentQueryInfo info) asm("_cpSegmentQueryHitDist");
typedef struct cpCircleShape {
 cpShape shape;
 cpVect c, tc;
 cpFloat r;
} cpCircleShape;
cpCircleShape* CircleShapeAlloc(void) asm("cpCircleShapeAlloc");
cpCircleShape* CircleShapeInit(cpCircleShape *circle, cpBody *body, cpFloat radius, cpVect offset) asm("cpCircleShapeInit");
cpShape* CircleShapeNew(cpBody *body, cpFloat radius, cpVect offset) asm("cpCircleShapeNew");
cpVect CircleShapeGetOffset(const cpShape *shape) asm("cpCircleShapeGetOffset");
cpFloat CircleShapeGetRadius(const cpShape *shape) asm("cpCircleShapeGetRadius");
typedef struct cpSegmentShape {
 cpShape shape;
 cpVect a, b, n;
 cpVect ta, tb, tn;
 cpFloat r;
 cpVect a_tangent, b_tangent;
} cpSegmentShape;
cpSegmentShape* SegmentShapeAlloc(void) asm("cpSegmentShapeAlloc");
cpSegmentShape* SegmentShapeInit(cpSegmentShape *seg, cpBody *body, cpVect a, cpVect b, cpFloat radius) asm("cpSegmentShapeInit");
cpShape* SegmentShapeNew(cpBody *body, cpVect a, cpVect b, cpFloat radius) asm("cpSegmentShapeNew");
void SegmentShapeSetNeighbors(cpShape *shape, cpVect prev, cpVect next) asm("cpSegmentShapeSetNeighbors");
cpVect SegmentShapeGetA(const cpShape *shape) asm("cpSegmentShapeGetA");
cpVect SegmentShapeGetB(const cpShape *shape) asm("cpSegmentShapeGetB");
cpVect SegmentShapeGetNormal(const cpShape *shape) asm("cpSegmentShapeGetNormal");
cpFloat SegmentShapeGetRadius(const cpShape *shape) asm("cpSegmentShapeGetRadius");
typedef struct cpPolyShapeAxis {
 cpVect n;
 cpFloat d;
} cpPolyShapeAxis;
typedef struct cpPolyShape {
 cpShape shape;
 int numVerts;
 cpVect *verts, *tVerts;
 cpPolyShapeAxis *axes, *tAxes;
} cpPolyShape;
cpPolyShape* PolyShapeAlloc(void) asm("cpPolyShapeAlloc");
cpPolyShape* PolyShapeInit(cpPolyShape *poly, cpBody *body, int numVerts, cpVect *verts, cpVect offset) asm("cpPolyShapeInit");
cpShape* PolyShapeNew(cpBody *body, int numVerts, cpVect *verts, cpVect offset) asm("cpPolyShapeNew");
cpPolyShape* BoxShapeInit(cpPolyShape *poly, cpBody *body, cpFloat width, cpFloat height) asm("cpBoxShapeInit");
cpPolyShape* BoxShapeInit2(cpPolyShape *poly, cpBody *body, cpBB box) asm("cpBoxShapeInit2");
cpShape* BoxShapeNew(cpBody *body, cpFloat width, cpFloat height) asm("cpBoxShapeNew");
cpShape* BoxShapeNew2(cpBody *body, cpBB box) asm("cpBoxShapeNew2");
cpBool PolyValidate(const cpVect *verts, const int numVerts) asm("cpPolyValidate");
int PolyShapeGetNumVerts(cpShape *shape) asm("cpPolyShapeGetNumVerts");
cpVect PolyShapeGetVert(cpShape *shape, int idx) asm("cpPolyShapeGetVert");
typedef cpBool (*cpCollisionBeginFunc)(cpArbiter *arb, cpSpace *space, void *data);
typedef cpBool (*cpCollisionPreSolveFunc)(cpArbiter *arb, cpSpace *space, void *data);
typedef void (*cpCollisionPostSolveFunc)(cpArbiter *arb, cpSpace *space, void *data);
typedef void (*cpCollisionSeparateFunc)(cpArbiter *arb, cpSpace *space, void *data);
struct cpCollisionHandler {
 cpCollisionType a;
 cpCollisionType b;
 cpCollisionBeginFunc begin;
 cpCollisionPreSolveFunc preSolve;
 cpCollisionPostSolveFunc postSolve;
 cpCollisionSeparateFunc separate;
 void *data;
};
typedef struct cpContact cpContact;
typedef enum cpArbiterState {
 cpArbiterStateFirstColl,
 cpArbiterStateNormal,
 cpArbiterStateIgnore,
 cpArbiterStateCached,
} cpArbiterState;
struct cpArbiterThread {
 struct cpArbiter *next, *prev;
};
struct cpArbiter {
 cpFloat e;
 cpFloat u;
 cpVect surface_vr;
 cpShape *a_private;
 cpShape *b_private;
 cpBody *body_a_private;
 cpBody *body_b_private;
 struct cpArbiterThread thread_a_private;
 struct cpArbiterThread thread_b_private;
 int numContacts_private;
 cpContact *contacts_private;
 cpTimestamp stamp_private;
 cpCollisionHandler *handler_private;
 cpBool swappedColl_private;
 cpArbiterState state_private;
};
cpFloat (*ArbiterGetElasticity)(const cpArbiter *arb) asm("_cpArbiterGetElasticity");
 void (*ArbiterSetElasticity)(cpArbiter *arb, cpFloat value) asm("_cpArbiterSetElasticity");
cpFloat (*ArbiterGetFriction)(const cpArbiter *arb) asm("_cpArbiterGetFriction");
 void (*ArbiterSetFriction)(cpArbiter *arb, cpFloat value) asm("_cpArbiterSetFriction");
cpVect (*ArbiterGetSurfaceVelocity)(const cpArbiter *arb) asm("_cpArbiterGetSurfaceVelocity");
 void (*ArbiterSetSurfaceVelocity)(cpArbiter *arb, cpVect value) asm("_cpArbiterSetSurfaceVelocity");
cpVect ArbiterTotalImpulse(const cpArbiter *arb) asm("cpArbiterTotalImpulse");
cpVect ArbiterTotalImpulseWithFriction(const cpArbiter *arb) asm("cpArbiterTotalImpulseWithFriction");
cpFloat ArbiterTotalKE(const cpArbiter *arb) asm("cpArbiterTotalKE");
void ArbiterIgnore(cpArbiter *arb) asm("cpArbiterIgnore");
void (*ArbiterGetShapes)(const cpArbiter *arb, cpShape **a, cpShape **b) asm("_cpArbiterGetShapes");
void (*ArbiterGetBodies)(const cpArbiter *arb, cpBody **a, cpBody **b) asm("_cpArbiterGetBodies");
cpBool (*ArbiterIsFirstContact)(const cpArbiter *arb) asm("_cpArbiterIsFirstContact");
int (*ArbiterGetCount)(const cpArbiter *arb) asm("_cpArbiterGetCount");
typedef struct cpContactPointSet {
 int count;
 struct {
  cpVect point;
  cpVect normal;
  cpFloat dist;
 } points[4];
} cpContactPointSet;
cpContactPointSet ArbiterGetContactPointSet(const cpArbiter *arb) asm("cpArbiterGetContactPointSet");
cpVect ArbiterGetNormal(const cpArbiter *arb, int i) asm("cpArbiterGetNormal");
cpVect ArbiterGetPoint(const cpArbiter *arb, int i) asm("cpArbiterGetPoint");
cpFloat ArbiterGetDepth(const cpArbiter *arb, int i) asm("cpArbiterGetDepth");
typedef struct cpConstraintClass cpConstraintClass;
typedef void (*cpConstraintPreStepImpl)(cpConstraint *constraint, cpFloat dt);
typedef void (*cpConstraintApplyCachedImpulseImpl)(cpConstraint *constraint, cpFloat dt_coef);
typedef void (*cpConstraintApplyImpulseImpl)(cpConstraint *constraint);
typedef cpFloat (*cpConstraintGetImpulseImpl)(cpConstraint *constraint);
struct cpConstraintClass {
 cpConstraintPreStepImpl preStep;
 cpConstraintApplyCachedImpulseImpl applyCachedImpulse;
 cpConstraintApplyImpulseImpl applyImpulse;
 cpConstraintGetImpulseImpl getImpulse;
};
typedef void (*cpConstraintPreSolveFunc)(cpConstraint *constraint, cpSpace *space);
typedef void (*cpConstraintPostSolveFunc)(cpConstraint *constraint, cpSpace *space);
struct cpConstraint {
 const cpConstraintClass *klass_private;
 cpBody *a;
 cpBody *b;
 cpSpace *space_private;
 cpConstraint *next_a_private;
 cpConstraint *next_b_private;
 cpFloat maxForce;
 cpFloat errorBias;
 cpFloat maxBias;
 cpConstraintPreSolveFunc preSolve;
 cpConstraintPostSolveFunc postSolve;
 cpDataPointer data;
};
void ConstraintDestroy(cpConstraint *constraint) asm("cpConstraintDestroy");
void ConstraintFree(cpConstraint *constraint) asm("cpConstraintFree");
void (*ConstraintActivateBodies)(cpConstraint *constraint) asm("_cpConstraintActivateBodies");
cpBody* (*ConstraintGetA)(const cpConstraint *constraint) asm("_cpConstraintGetA");
cpBody* (*ConstraintGetB)(const cpConstraint *constraint) asm("_cpConstraintGetB");
cpFloat (*ConstraintGetMaxForce)(const cpConstraint *constraint) asm("_cpConstraintGetMaxForce");
 void (*ConstraintSetMaxForce)(cpConstraint *constraint, cpFloat value) asm("_cpConstraintSetMaxForce");
cpFloat (*ConstraintGetErrorBias)(const cpConstraint *constraint) asm("_cpConstraintGetErrorBias");
 void (*ConstraintSetErrorBias)(cpConstraint *constraint, cpFloat value) asm("_cpConstraintSetErrorBias");
cpFloat (*ConstraintGetMaxBias)(const cpConstraint *constraint) asm("_cpConstraintGetMaxBias");
 void (*ConstraintSetMaxBias)(cpConstraint *constraint, cpFloat value) asm("_cpConstraintSetMaxBias");
cpConstraintPreSolveFunc (*ConstraintGetPreSolveFunc)(const cpConstraint *constraint) asm("_cpConstraintGetPreSolveFunc");
 void (*ConstraintSetPreSolveFunc)(cpConstraint *constraint, cpConstraintPreSolveFunc value) asm("_cpConstraintSetPreSolveFunc");
cpConstraintPostSolveFunc (*ConstraintGetPostSolveFunc)(const cpConstraint *constraint) asm("_cpConstraintGetPostSolveFunc");
 void (*ConstraintSetPostSolveFunc)(cpConstraint *constraint, cpConstraintPostSolveFunc value) asm("_cpConstraintSetPostSolveFunc");
cpDataPointer (*ConstraintGetUserData)(const cpConstraint *constraint) asm("_cpConstraintGetUserData");
 void (*ConstraintSetUserData)(cpConstraint *constraint, cpDataPointer value) asm("_cpConstraintSetUserData");
cpFloat (*ConstraintGetImpulse)(cpConstraint *constraint) asm("_cpConstraintGetImpulse");
const cpConstraintClass *PinJointGetClass(void) asm("cpPinJointGetClass");
typedef struct cpPinJoint {
 cpConstraint constraint;
 cpVect anchr1, anchr2;
 cpFloat dist;
 cpVect r1, r2;
 cpVect n;
 cpFloat nMass;
 cpFloat jnAcc, jnMax;
 cpFloat bias;
} cpPinJoint;
cpPinJoint* PinJointAlloc(void) asm("cpPinJointAlloc");
cpPinJoint* PinJointInit(cpPinJoint *joint, cpBody *a, cpBody *b, cpVect anchr1, cpVect anchr2) asm("cpPinJointInit");
cpConstraint* PinJointNew(cpBody *a, cpBody *b, cpVect anchr1, cpVect anchr2) asm("cpPinJointNew");
cpVect (*PinJointGetAnchr1)(const cpConstraint *constraint) asm("_cpPinJointGetAnchr1");
 void (*PinJointSetAnchr1)(cpConstraint *constraint, cpVect value) asm("_cpPinJointSetAnchr1");
cpVect (*PinJointGetAnchr2)(const cpConstraint *constraint) asm("_cpPinJointGetAnchr2");
 void (*PinJointSetAnchr2)(cpConstraint *constraint, cpVect value) asm("_cpPinJointSetAnchr2");
cpFloat (*PinJointGetDist)(const cpConstraint *constraint) asm("_cpPinJointGetDist");
 void (*PinJointSetDist)(cpConstraint *constraint, cpFloat value) asm("_cpPinJointSetDist");
const cpConstraintClass *SlideJointGetClass(void) asm("cpSlideJointGetClass");
typedef struct cpSlideJoint {
 cpConstraint constraint;
 cpVect anchr1, anchr2;
 cpFloat min, max;
 cpVect r1, r2;
 cpVect n;
 cpFloat nMass;
 cpFloat jnAcc, jnMax;
 cpFloat bias;
} cpSlideJoint;
cpSlideJoint* SlideJointAlloc(void) asm("cpSlideJointAlloc");
cpSlideJoint* SlideJointInit(cpSlideJoint *joint, cpBody *a, cpBody *b, cpVect anchr1, cpVect anchr2, cpFloat min, cpFloat max) asm("cpSlideJointInit");
cpConstraint* SlideJointNew(cpBody *a, cpBody *b, cpVect anchr1, cpVect anchr2, cpFloat min, cpFloat max) asm("cpSlideJointNew");
cpVect (*SlideJointGetAnchr1)(const cpConstraint *constraint) asm("_cpSlideJointGetAnchr1");
 void (*SlideJointSetAnchr1)(cpConstraint *constraint, cpVect value) asm("_cpSlideJointSetAnchr1");
cpVect (*SlideJointGetAnchr2)(const cpConstraint *constraint) asm("_cpSlideJointGetAnchr2");
 void (*SlideJointSetAnchr2)(cpConstraint *constraint, cpVect value) asm("_cpSlideJointSetAnchr2");
cpFloat (*SlideJointGetMin)(const cpConstraint *constraint) asm("_cpSlideJointGetMin");
 void (*SlideJointSetMin)(cpConstraint *constraint, cpFloat value) asm("_cpSlideJointSetMin");
cpFloat (*SlideJointGetMax)(const cpConstraint *constraint) asm("_cpSlideJointGetMax");
 void (*SlideJointSetMax)(cpConstraint *constraint, cpFloat value) asm("_cpSlideJointSetMax");
const cpConstraintClass *PivotJointGetClass(void) asm("cpPivotJointGetClass");
typedef struct cpPivotJoint {
 cpConstraint constraint;
 cpVect anchr1, anchr2;
 cpVect r1, r2;
 cpVect k1, k2;
 cpVect jAcc;
 cpFloat jMaxLen;
 cpVect bias;
} cpPivotJoint;
cpPivotJoint* PivotJointAlloc(void) asm("cpPivotJointAlloc");
cpPivotJoint* PivotJointInit(cpPivotJoint *joint, cpBody *a, cpBody *b, cpVect anchr1, cpVect anchr2) asm("cpPivotJointInit");
cpConstraint* PivotJointNew(cpBody *a, cpBody *b, cpVect pivot) asm("cpPivotJointNew");
cpConstraint* PivotJointNew2(cpBody *a, cpBody *b, cpVect anchr1, cpVect anchr2) asm("cpPivotJointNew2");
cpVect (*PivotJointGetAnchr1)(const cpConstraint *constraint) asm("_cpPivotJointGetAnchr1");
 void (*PivotJointSetAnchr1)(cpConstraint *constraint, cpVect value) asm("_cpPivotJointSetAnchr1");
cpVect (*PivotJointGetAnchr2)(const cpConstraint *constraint) asm("_cpPivotJointGetAnchr2");
 void (*PivotJointSetAnchr2)(cpConstraint *constraint, cpVect value) asm("_cpPivotJointSetAnchr2");
const cpConstraintClass *GrooveJointGetClass(void) asm("cpGrooveJointGetClass");
typedef struct cpGrooveJoint {
 cpConstraint constraint;
 cpVect grv_n, grv_a, grv_b;
 cpVect anchr2;
 cpVect grv_tn;
 cpFloat clamp;
 cpVect r1, r2;
 cpVect k1, k2;
 cpVect jAcc;
 cpFloat jMaxLen;
 cpVect bias;
} cpGrooveJoint;
cpGrooveJoint* GrooveJointAlloc(void) asm("cpGrooveJointAlloc");
cpGrooveJoint* GrooveJointInit(cpGrooveJoint *joint, cpBody *a, cpBody *b, cpVect groove_a, cpVect groove_b, cpVect anchr2) asm("cpGrooveJointInit");
cpConstraint* GrooveJointNew(cpBody *a, cpBody *b, cpVect groove_a, cpVect groove_b, cpVect anchr2) asm("cpGrooveJointNew");
cpVect (*GrooveJointGetGrooveA)(const cpConstraint *constraint) asm("_cpGrooveJointGetGrooveA");
void GrooveJointSetGrooveA(cpConstraint *constraint, cpVect value) asm("cpGrooveJointSetGrooveA");
cpVect (*GrooveJointGetGrooveB)(const cpConstraint *constraint) asm("_cpGrooveJointGetGrooveB");
void GrooveJointSetGrooveB(cpConstraint *constraint, cpVect value) asm("cpGrooveJointSetGrooveB");
cpVect (*GrooveJointGetAnchr2)(const cpConstraint *constraint) asm("_cpGrooveJointGetAnchr2");
 void (*GrooveJointSetAnchr2)(cpConstraint *constraint, cpVect value) asm("_cpGrooveJointSetAnchr2");
typedef struct cpDampedSpring cpDampedSpring;
typedef cpFloat (*cpDampedSpringForceFunc)(cpConstraint *spring, cpFloat dist);
const cpConstraintClass *DampedSpringGetClass(void) asm("cpDampedSpringGetClass");
struct cpDampedSpring {
 cpConstraint constraint;
 cpVect anchr1, anchr2;
 cpFloat restLength;
 cpFloat stiffness;
 cpFloat damping;
 cpDampedSpringForceFunc springForceFunc;
 cpFloat target_vrn;
 cpFloat v_coef;
 cpVect r1, r2;
 cpFloat nMass;
 cpVect n;
};
cpDampedSpring* DampedSpringAlloc(void) asm("cpDampedSpringAlloc");
cpDampedSpring* DampedSpringInit(cpDampedSpring *joint, cpBody *a, cpBody *b, cpVect anchr1, cpVect anchr2, cpFloat restLength, cpFloat stiffness, cpFloat damping) asm("cpDampedSpringInit");
cpConstraint* DampedSpringNew(cpBody *a, cpBody *b, cpVect anchr1, cpVect anchr2, cpFloat restLength, cpFloat stiffness, cpFloat damping) asm("cpDampedSpringNew");
cpVect (*DampedSpringGetAnchr1)(const cpConstraint *constraint) asm("_cpDampedSpringGetAnchr1");
 void (*DampedSpringSetAnchr1)(cpConstraint *constraint, cpVect value) asm("_cpDampedSpringSetAnchr1");
cpVect (*DampedSpringGetAnchr2)(const cpConstraint *constraint) asm("_cpDampedSpringGetAnchr2");
 void (*DampedSpringSetAnchr2)(cpConstraint *constraint, cpVect value) asm("_cpDampedSpringSetAnchr2");
cpFloat (*DampedSpringGetRestLength)(const cpConstraint *constraint) asm("_cpDampedSpringGetRestLength");
 void (*DampedSpringSetRestLength)(cpConstraint *constraint, cpFloat value) asm("_cpDampedSpringSetRestLength");
cpFloat (*DampedSpringGetStiffness)(const cpConstraint *constraint) asm("_cpDampedSpringGetStiffness");
 void (*DampedSpringSetStiffness)(cpConstraint *constraint, cpFloat value) asm("_cpDampedSpringSetStiffness");
cpFloat (*DampedSpringGetDamping)(const cpConstraint *constraint) asm("_cpDampedSpringGetDamping");
 void (*DampedSpringSetDamping)(cpConstraint *constraint, cpFloat value) asm("_cpDampedSpringSetDamping");
cpDampedSpringForceFunc (*DampedSpringGetSpringForceFunc)(const cpConstraint *constraint) asm("_cpDampedSpringGetSpringForceFunc");
 void (*DampedSpringSetSpringForceFunc)(cpConstraint *constraint, cpDampedSpringForceFunc value) asm("_cpDampedSpringSetSpringForceFunc");
typedef cpFloat (*cpDampedRotarySpringTorqueFunc)(struct cpConstraint *spring, cpFloat relativeAngle);
const cpConstraintClass *DampedRotarySpringGetClass(void) asm("cpDampedRotarySpringGetClass");
typedef struct cpDampedRotarySpring {
 cpConstraint constraint;
 cpFloat restAngle;
 cpFloat stiffness;
 cpFloat damping;
 cpDampedRotarySpringTorqueFunc springTorqueFunc;
 cpFloat target_wrn;
 cpFloat w_coef;
 cpFloat iSum;
} cpDampedRotarySpring;
cpDampedRotarySpring* DampedRotarySpringAlloc(void) asm("cpDampedRotarySpringAlloc");
cpDampedRotarySpring* DampedRotarySpringInit(cpDampedRotarySpring *joint, cpBody *a, cpBody *b, cpFloat restAngle, cpFloat stiffness, cpFloat damping) asm("cpDampedRotarySpringInit");
cpConstraint* DampedRotarySpringNew(cpBody *a, cpBody *b, cpFloat restAngle, cpFloat stiffness, cpFloat damping) asm("cpDampedRotarySpringNew");
cpFloat (*DampedRotarySpringGetRestAngle)(const cpConstraint *constraint) asm("_cpDampedRotarySpringGetRestAngle");
 void (*DampedRotarySpringSetRestAngle)(cpConstraint *constraint, cpFloat value) asm("_cpDampedRotarySpringSetRestAngle");
cpFloat (*DampedRotarySpringGetStiffness)(const cpConstraint *constraint) asm("_cpDampedRotarySpringGetStiffness");
 void (*DampedRotarySpringSetStiffness)(cpConstraint *constraint, cpFloat value) asm("_cpDampedRotarySpringSetStiffness");
cpFloat (*DampedRotarySpringGetDamping)(const cpConstraint *constraint) asm("_cpDampedRotarySpringGetDamping");
 void (*DampedRotarySpringSetDamping)(cpConstraint *constraint, cpFloat value) asm("_cpDampedRotarySpringSetDamping");
cpDampedRotarySpringTorqueFunc (*DampedRotarySpringGetSpringTorqueFunc)(const cpConstraint *constraint) asm("_cpDampedRotarySpringGetSpringTorqueFunc");
 void (*DampedRotarySpringSetSpringTorqueFunc)(cpConstraint *constraint, cpDampedRotarySpringTorqueFunc value) asm("_cpDampedRotarySpringSetSpringTorqueFunc");
const cpConstraintClass *RotaryLimitJointGetClass(void) asm("cpRotaryLimitJointGetClass");
typedef struct cpRotaryLimitJoint {
 cpConstraint constraint;
 cpFloat min, max;
 cpFloat iSum;
 cpFloat bias;
 cpFloat jAcc, jMax;
} cpRotaryLimitJoint;
cpRotaryLimitJoint* RotaryLimitJointAlloc(void) asm("cpRotaryLimitJointAlloc");
cpRotaryLimitJoint* RotaryLimitJointInit(cpRotaryLimitJoint *joint, cpBody *a, cpBody *b, cpFloat min, cpFloat max) asm("cpRotaryLimitJointInit");
cpConstraint* RotaryLimitJointNew(cpBody *a, cpBody *b, cpFloat min, cpFloat max) asm("cpRotaryLimitJointNew");
cpFloat (*RotaryLimitJointGetMin)(const cpConstraint *constraint) asm("_cpRotaryLimitJointGetMin");
 void (*RotaryLimitJointSetMin)(cpConstraint *constraint, cpFloat value) asm("_cpRotaryLimitJointSetMin");
cpFloat (*RotaryLimitJointGetMax)(const cpConstraint *constraint) asm("_cpRotaryLimitJointGetMax");
 void (*RotaryLimitJointSetMax)(cpConstraint *constraint, cpFloat value) asm("_cpRotaryLimitJointSetMax");
const cpConstraintClass *RatchetJointGetClass(void) asm("cpRatchetJointGetClass");
typedef struct cpRatchetJoint {
 cpConstraint constraint;
 cpFloat angle, phase, ratchet;
 cpFloat iSum;
 cpFloat bias;
 cpFloat jAcc, jMax;
} cpRatchetJoint;
cpRatchetJoint* RatchetJointAlloc(void) asm("cpRatchetJointAlloc");
cpRatchetJoint* RatchetJointInit(cpRatchetJoint *joint, cpBody *a, cpBody *b, cpFloat phase, cpFloat ratchet) asm("cpRatchetJointInit");
cpConstraint* RatchetJointNew(cpBody *a, cpBody *b, cpFloat phase, cpFloat ratchet) asm("cpRatchetJointNew");
cpFloat (*RatchetJointGetAngle)(const cpConstraint *constraint) asm("_cpRatchetJointGetAngle");
 void (*RatchetJointSetAngle)(cpConstraint *constraint, cpFloat value) asm("_cpRatchetJointSetAngle");
cpFloat (*RatchetJointGetPhase)(const cpConstraint *constraint) asm("_cpRatchetJointGetPhase");
 void (*RatchetJointSetPhase)(cpConstraint *constraint, cpFloat value) asm("_cpRatchetJointSetPhase");
cpFloat (*RatchetJointGetRatchet)(const cpConstraint *constraint) asm("_cpRatchetJointGetRatchet");
 void (*RatchetJointSetRatchet)(cpConstraint *constraint, cpFloat value) asm("_cpRatchetJointSetRatchet");
const cpConstraintClass *GearJointGetClass(void) asm("cpGearJointGetClass");
typedef struct cpGearJoint {
 cpConstraint constraint;
 cpFloat phase, ratio;
 cpFloat ratio_inv;
 cpFloat iSum;
 cpFloat bias;
 cpFloat jAcc, jMax;
} cpGearJoint;
cpGearJoint* GearJointAlloc(void) asm("cpGearJointAlloc");
cpGearJoint* GearJointInit(cpGearJoint *joint, cpBody *a, cpBody *b, cpFloat phase, cpFloat ratio) asm("cpGearJointInit");
cpConstraint* GearJointNew(cpBody *a, cpBody *b, cpFloat phase, cpFloat ratio) asm("cpGearJointNew");
cpFloat (*GearJointGetPhase)(const cpConstraint *constraint) asm("_cpGearJointGetPhase");
 void (*GearJointSetPhase)(cpConstraint *constraint, cpFloat value) asm("_cpGearJointSetPhase");
cpFloat (*GearJointGetRatio)(const cpConstraint *constraint) asm("_cpGearJointGetRatio");
void GearJointSetRatio(cpConstraint *constraint, cpFloat value) asm("cpGearJointSetRatio");
const cpConstraintClass *SimpleMotorGetClass(void) asm("cpSimpleMotorGetClass");
typedef struct cpSimpleMotor {
 cpConstraint constraint;
 cpFloat rate;
 cpFloat iSum;
 cpFloat jAcc, jMax;
} cpSimpleMotor;
cpSimpleMotor* SimpleMotorAlloc(void) asm("cpSimpleMotorAlloc");
cpSimpleMotor* SimpleMotorInit(cpSimpleMotor *joint, cpBody *a, cpBody *b, cpFloat rate) asm("cpSimpleMotorInit");
cpConstraint* SimpleMotorNew(cpBody *a, cpBody *b, cpFloat rate) asm("cpSimpleMotorNew");
cpFloat (*SimpleMotorGetRate)(const cpConstraint *constraint) asm("_cpSimpleMotorGetRate");
 void (*SimpleMotorSetRate)(cpConstraint *constraint, cpFloat value) asm("_cpSimpleMotorSetRate");
typedef struct cpContactBufferHeader cpContactBufferHeader;
typedef void (*cpSpaceArbiterApplyImpulseFunc)(cpArbiter *arb);
struct cpSpace {
 int iterations;
 cpVect gravity;
 cpFloat damping;
 cpFloat idleSpeedThreshold;
 cpFloat sleepTimeThreshold;
 cpFloat collisionSlop;
 cpFloat collisionBias;
 cpTimestamp collisionPersistence;
 cpBool enableContactGraph;
 cpDataPointer data;
 cpBody *staticBody;
 cpTimestamp stamp_private;
 cpFloat curr_dt_private;
 cpArray *bodies_private;
 cpArray *rousedBodies_private;
 cpArray *sleepingComponents_private;
 cpSpatialIndex *staticShapes_private;
 cpSpatialIndex *activeShapes_private;
 cpArray *arbiters_private;
 cpContactBufferHeader *contactBuffersHead_private;
 cpHashSet *cachedArbiters_private;
 cpArray *pooledArbiters_private;
 cpArray *constraints_private;
 cpArray *allocatedBuffers_private;
 int locked_private;
 cpHashSet *collisionHandlers_private;
 cpCollisionHandler defaultHandler_private;
 cpHashSet *postStepCallbacks_private;
 cpSpaceArbiterApplyImpulseFunc arbiterApplyImpulse_private;
 cpBody _staticBody_private;
};
cpSpace* SpaceAlloc(void) asm("cpSpaceAlloc");
cpSpace* SpaceInit(cpSpace *space) asm("cpSpaceInit");
cpSpace* SpaceNew(void) asm("cpSpaceNew");
void SpaceDestroy(cpSpace *space) asm("cpSpaceDestroy");
void SpaceFree(cpSpace *space) asm("cpSpaceFree");
int (*SpaceGetIterations)(const cpSpace *space) asm("_cpSpaceGetIterations");
 void (*SpaceSetIterations)(cpSpace *space, int value) asm("_cpSpaceSetIterations");
cpVect (*SpaceGetGravity)(const cpSpace *space) asm("_cpSpaceGetGravity");
 void (*SpaceSetGravity)(cpSpace *space, cpVect value) asm("_cpSpaceSetGravity");
cpFloat (*SpaceGetDamping)(const cpSpace *space) asm("_cpSpaceGetDamping");
 void (*SpaceSetDamping)(cpSpace *space, cpFloat value) asm("_cpSpaceSetDamping");
cpFloat (*SpaceGetIdleSpeedThreshold)(const cpSpace *space) asm("_cpSpaceGetIdleSpeedThreshold");
 void (*SpaceSetIdleSpeedThreshold)(cpSpace *space, cpFloat value) asm("_cpSpaceSetIdleSpeedThreshold");
cpFloat (*SpaceGetSleepTimeThreshold)(const cpSpace *space) asm("_cpSpaceGetSleepTimeThreshold");
 void (*SpaceSetSleepTimeThreshold)(cpSpace *space, cpFloat value) asm("_cpSpaceSetSleepTimeThreshold");
cpFloat (*SpaceGetCollisionSlop)(const cpSpace *space) asm("_cpSpaceGetCollisionSlop");
 void (*SpaceSetCollisionSlop)(cpSpace *space, cpFloat value) asm("_cpSpaceSetCollisionSlop");
cpFloat (*SpaceGetCollisionBias)(const cpSpace *space) asm("_cpSpaceGetCollisionBias");
 void (*SpaceSetCollisionBias)(cpSpace *space, cpFloat value) asm("_cpSpaceSetCollisionBias");
cpTimestamp (*SpaceGetCollisionPersistence)(const cpSpace *space) asm("_cpSpaceGetCollisionPersistence");
 void (*SpaceSetCollisionPersistence)(cpSpace *space, cpTimestamp value) asm("_cpSpaceSetCollisionPersistence");
cpBool (*SpaceGetEnableContactGraph)(const cpSpace *space) asm("_cpSpaceGetEnableContactGraph");
 void (*SpaceSetEnableContactGraph)(cpSpace *space, cpBool value) asm("_cpSpaceSetEnableContactGraph");
cpDataPointer (*SpaceGetUserData)(const cpSpace *space) asm("_cpSpaceGetUserData");
 void (*SpaceSetUserData)(cpSpace *space, cpDataPointer value) asm("_cpSpaceSetUserData");
cpBody* (*SpaceGetStaticBody)(const cpSpace *space) asm("_cpSpaceGetStaticBody");
cpFloat (*SpaceGetCurrentTimeStep)(const cpSpace *space) asm("_cpSpaceGetCurrentTimeStep");
cpBool (*SpaceIsLocked)(cpSpace *space) asm("_cpSpaceIsLocked");
void cpSpaceSetDefaultCollisionHandler(
 cpSpace *space,
 cpCollisionBeginFunc begin,
 cpCollisionPreSolveFunc preSolve,
 cpCollisionPostSolveFunc postSolve,
 cpCollisionSeparateFunc separate,
 void *data
);
void cpSpaceAddCollisionHandler(
 cpSpace *space,
 cpCollisionType a, cpCollisionType b,
 cpCollisionBeginFunc begin,
 cpCollisionPreSolveFunc preSolve,
 cpCollisionPostSolveFunc postSolve,
 cpCollisionSeparateFunc separate,
 void *data
);
void SpaceRemoveCollisionHandler(cpSpace *space, cpCollisionType a, cpCollisionType b) asm("cpSpaceRemoveCollisionHandler");
cpShape* SpaceAddShape(cpSpace *space, cpShape *shape) asm("cpSpaceAddShape");
cpShape* SpaceAddStaticShape(cpSpace *space, cpShape *shape) asm("cpSpaceAddStaticShape");
cpBody* SpaceAddBody(cpSpace *space, cpBody *body) asm("cpSpaceAddBody");
cpConstraint* SpaceAddConstraint(cpSpace *space, cpConstraint *constraint) asm("cpSpaceAddConstraint");
void SpaceRemoveShape(cpSpace *space, cpShape *shape) asm("cpSpaceRemoveShape");
void SpaceRemoveStaticShape(cpSpace *space, cpShape *shape) asm("cpSpaceRemoveStaticShape");
void SpaceRemoveBody(cpSpace *space, cpBody *body) asm("cpSpaceRemoveBody");
void SpaceRemoveConstraint(cpSpace *space, cpConstraint *constraint) asm("cpSpaceRemoveConstraint");
cpBool SpaceContainsShape(cpSpace *space, cpShape *shape) asm("cpSpaceContainsShape");
cpBool SpaceContainsBody(cpSpace *space, cpBody *body) asm("cpSpaceContainsBody");
cpBool SpaceContainsConstraint(cpSpace *space, cpConstraint *constraint) asm("cpSpaceContainsConstraint");
typedef void (*cpPostStepFunc)(cpSpace *space, void *obj, void *data);
void SpaceAddPostStepCallback(cpSpace *space, cpPostStepFunc func, void *obj, void *data) asm("cpSpaceAddPostStepCallback");
typedef void (*cpSpacePointQueryFunc)(cpShape *shape, void *data);
void SpacePointQuery(cpSpace *space, cpVect point, cpLayers layers, cpGroup group, cpSpacePointQueryFunc func, void *data) asm("cpSpacePointQuery");
cpShape *SpacePointQueryFirst(cpSpace *space, cpVect point, cpLayers layers, cpGroup group) asm("cpSpacePointQueryFirst");
typedef void (*cpSpaceSegmentQueryFunc)(cpShape *shape, cpFloat t, cpVect n, void *data);
void SpaceSegmentQuery(cpSpace *space, cpVect start, cpVect end, cpLayers layers, cpGroup group, cpSpaceSegmentQueryFunc func, void *data) asm("cpSpaceSegmentQuery");
cpShape *SpaceSegmentQueryFirst(cpSpace *space, cpVect start, cpVect end, cpLayers layers, cpGroup group, cpSegmentQueryInfo *out) asm("cpSpaceSegmentQueryFirst");
typedef void (*cpSpaceBBQueryFunc)(cpShape *shape, void *data);
void SpaceBBQuery(cpSpace *space, cpBB bb, cpLayers layers, cpGroup group, cpSpaceBBQueryFunc func, void *data) asm("cpSpaceBBQuery");
typedef void (*cpSpaceShapeQueryFunc)(cpShape *shape, cpContactPointSet *points, void *data);
cpBool SpaceShapeQuery(cpSpace *space, cpShape *shape, cpSpaceShapeQueryFunc func, void *data) asm("cpSpaceShapeQuery");
void SpaceActivateShapesTouchingShape(cpSpace *space, cpShape *shape) asm("cpSpaceActivateShapesTouchingShape");
typedef void (*cpSpaceBodyIteratorFunc)(cpBody *body, void *data);
void SpaceEachBody(cpSpace *space, cpSpaceBodyIteratorFunc func, void *data) asm("cpSpaceEachBody");
typedef void (*cpSpaceShapeIteratorFunc)(cpShape *shape, void *data);
void SpaceEachShape(cpSpace *space, cpSpaceShapeIteratorFunc func, void *data) asm("cpSpaceEachShape");
typedef void (*cpSpaceConstraintIteratorFunc)(cpConstraint *constraint, void *data);
void SpaceEachConstraint(cpSpace *space, cpSpaceConstraintIteratorFunc func, void *data) asm("cpSpaceEachConstraint");
void SpaceReindexStatic(cpSpace *space) asm("cpSpaceReindexStatic");
void SpaceReindexShape(cpSpace *space, cpShape *shape) asm("cpSpaceReindexShape");
void SpaceReindexShapesForBody(cpSpace *space, cpBody *body) asm("cpSpaceReindexShapesForBody");
void SpaceUseSpatialHash(cpSpace *space, cpFloat dim, int count) asm("cpSpaceUseSpatialHash");
void SpaceStep(cpSpace *space, cpFloat dt) asm("cpSpaceStep");
extern const char *cpVersionString;
void InitChipmunk(void) asm("cpInitChipmunk");
cpFloat MomentForCircle(cpFloat m, cpFloat r1, cpFloat r2, cpVect offset) asm("cpMomentForCircle");
cpFloat AreaForCircle(cpFloat r1, cpFloat r2) asm("cpAreaForCircle");
cpFloat MomentForSegment(cpFloat m, cpVect a, cpVect b) asm("cpMomentForSegment");
cpFloat AreaForSegment(cpVect a, cpVect b, cpFloat r) asm("cpAreaForSegment");
cpFloat MomentForPoly(cpFloat m, int numVerts, const cpVect *verts, cpVect offset) asm("cpMomentForPoly");
cpFloat AreaForPoly(const int numVerts, const cpVect *verts) asm("cpAreaForPoly");
cpVect CentroidForPoly(const int numVerts, const cpVect *verts) asm("cpCentroidForPoly");
void RecenterPoly(const int numVerts, cpVect *verts) asm("cpRecenterPoly");
cpFloat MomentForBox(cpFloat m, cpFloat width, cpFloat height) asm("cpMomentForBox");
cpFloat MomentForBox2(cpFloat m, cpBB box) asm("cpMomentForBox2");
