local ffi = require 'ffi'
local chipmunk = cheetah.loadDLL 'chipmunk'
local cp = {}
cp.cpvzero = ffi.new('cpVect', {0,0})
ffi.metatype('cpBody', {
	__index = {
		init = chipmunk.cpBodyInit,
		initStatic = chipmunk.cpBodyInitStatic,
		newStatic = chipmunk.cpBodyNewStatic,
		destroy = chipmunk.cpBodyDestroy,
		sanityCheck = cpBodySanityCheck,
		activate = chipmunk.cpBodyActivate,
		activateStatic = chipmunk.cpBodyActivateStatic,
		sleep = chipmunk.cpBodySleep,
		sleepWithGroup = chipmunk.cpBodySleepWithGroup,
		isSleeping = chipmunk._cpBodyIsSleeping,
		isStatic = chipmunk._cpBodyIsStatic,
		isRogue = chipmunk._cpBodyIsRogue,
		getMass = chipmunk._cpBodyGetMass,
		setMass = chipmunk.cpBodySetMass,
		getMoment = chipmunk._cpBodyGetMoment,
		setMoment = chipmunk.cpBodySetMoment,
		getPos = chipmunk._cpBodyGetPos,
		setPos = chipmunk.cpBodySetPos,
		getVel = chipmunk._cpBodyGetVel,
		setVel = chipmunk._cpBodySetVel,
		getForce = chipmunk._cpBodyGetForce,
		setForce = chipmunk._cpBodySetForce,
		getAngle = chipmunk._cpBodyGetAngle,
		setAngle = chipmunk.cpBodySetAngle,
		getAngVel = chipmunk._cpBodyGetAngVel,
		setAngVel = chipmunk._cpBodySetAngVel,
		getTorque = chipmunk._cpBodyGetTorque,
		setTorque = chipmunk._cpBodySetTorque,
		getRot = chipmunk._cpBodyGetRot,
		getVelLimit = chipmunk._cpBodyGetVelLimit,
		setVelLimit = chipmunk._cpBodySetVelLimit,
		getAngVelLimit = chipmunk._cpBodyGetAngVelLimit,
		setAngVelLimit = chipmunk._cpBodySetAngVelLimit,
		getUserData = chipmunk._cpBodyGetUserData,
		setUserData = chipmunk._cpBodySetUserData,
		updateVelocity = chipmunk.cpBodyUpdateVelocity,
		updatePosition = chipmunk.cpBodyUpdatePosition,
		local2World = chipmunk._cpBodyLocal2World,
		world2Local = chipmunk._cpBodyWorld2Local,
		resetForces = chipmunk.cpBodyResetForces,
		applyForce = chipmunk.cpBodyApplyForce,
		applyImpulse = chipmunk.cpBodyApplyImpulse,
		getVelAtWorldPoint = chipmunk.cpBodyGetVelAtWorldPoint,
		getVelAtLocalPoint = chipmunk.cpBodyGetVelAtLocalPoint,
	},
	__gc = chipmunk.cpBodyFree
})

ffi.metatype('cpShape', {
	__index = {
		destroy = chipmunk.cpShapeDestroy,
		cacheBB = chipmunk.cpShapeCacheBB,
		update = chipmunk.cpShapeUpdate,
		pointQuery = chipmunk.cpShapePointQuery,
		getBody = chipmunk._cpShapeGetBody,
		setBody = chipmunk.cpShapeSetBody,
		getBB = chipmunk._cpShapeGetBB,
		getSensor = chipmunk._cpShapeGetSensor,
		setSensor = chipmunk._cpShapeSetSensor,
		getElasticity = chipmunk._cpShapeGetElasticity,
		setElasticity = chipmunk._cpShapeSetElasticity,
		getFriction = chipmunk._cpShapeGetFriction,
		setFriction = chipmunk._cpShapeSetFriction,
		getSurfaceVelocity = chipmunk._cpShapeGetSurfaceVelocity,
		setSurfaceVelocity = chipmunk._cpShapeSetSurfaceVelocity,
		getUserData = chipmunk._cpShapeGetUserData,
		setUserData = chipmunk._cpShapeSetUserData,
		getCollisionType = chipmunk._cpShapeGetCollisionType,
		setCollisionType = chipmunk._cpShapeSetCollisionType,
		getGroup = chipmunk._cpShapeGetGroup,
		setGroup = chipmunk._cpShapeSetGroup,
		getLayers = chipmunk._cpShapeGetLayers,
		setLayers = chipmunk._cpShapeSetLayers,
		segmentQuery = chipmunk.cpShapeSegmentQuery,
	},
	__gc = chipmunk.cpShapeFree
})

ffi.metatype('cpSpace', {
	__index = {
		destroy = chipmunk.cpSpaceDestroy,
		getIterations = chipmunk._cpSpaceGetIterations,
		setIterations = chipmunk._cpSpaceSetIterations,
		getGravity = chipmunk._cpSpaceGetGravity,
		setGravity = chipmunk._cpSpaceSetGravity,
		getDamping = chipmunk._cpSpaceGetDamping,
		setDamping = chipmunk._cpSpaceSetDamping,
		getIdleSpeedThreshold = chipmunk._cpSpaceGetIdleSpeedThreshold,
		setIdleSpeedThreshold = chipmunk._cpSpaceSetIdleSpeedThreshold,
		getSleepTimeThreshold = chipmunk._cpSpaceGetSleepTimeThreshold,
		setSleepTimeThreshold = chipmunk._cpSpaceSetSleepTimeThreshold,
		getCollisionSlop = chipmunk._cpSpaceGetCollisionSlop,
		setCollisionSlop = chipmunk._cpSpaceSetCollisionSlop,
		getCollisionBias = chipmunk._cpSpaceGetCollisionBias,
		setCollisionBias = chipmunk._cpSpaceSetCollisionBias,
		getCollisionPersistence = chipmunk._cpSpaceGetCollisionPersistence,
		setCollisionPersistence = chipmunk._cpSpaceSetCollisionPersistence,
		getEnableContactGraph = chipmunk._cpSpaceGetEnableContactGraph,
		setEnableContactGraph = chipmunk._cpSpaceSetEnableContactGraph,
		getUserData = chipmunk._cpSpaceGetUserData,
		setUserData = chipmunk._cpSpaceSetUserData,
		getStaticBody = chipmunk._cpSpaceGetStaticBody,
		getCurrentTimeStep = chipmunk._cpSpaceGetCurrentTimeStep,
		isLocked = chipmunk._cpSpaceIsLocked,
	},
	__gc = chipmunk.cpSpaceFree
})



setmetatable(cp, { __index = chipmunk})
return cp