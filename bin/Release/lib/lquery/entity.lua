--[[****************************************************************************

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

******************************************************************************]]

local easing = require("lib/lquery/easing")

local C = cheetah

lQuery = {
	fx = true,
	hooks = {},
	_onresize = {},
	addhook = function(hook)
		table.insert(lQuery.hooks, hook)
	end,
	onresize = function(func)
		table.insert(lQuery._onresize, func)
	end,
	_MousePressedOwner = false,
	MousePressed = false,
	_last_click_millis = 0,
	_last_clickX = -1,
	_last_clickY = -1,
	dblclickInterval = 0.3
}


Entity = {} --Any object: box, circle, character etc
E = Entity --short name
local EntityMeta = {
	__index = Entity
}
--special metatable for objects that have update callbacks
local EntityMetaUpdate = {
	__index = function(t, k)
		if t.__data[k] then return t.__data[k] end
		if Entity[k] then return Entity[k] end
	end,
	__newindex = function(t, k, v)
		rawset(t.__data, k, v)
		if t.upd[k] then t.upd[k](t, v) end
	end
}
function Entity:new(parent)  -- constructor
	local object = {
		x = 0,   --x coord
		y = 0,   --y coord
		w = 1,
		h = 1,
		ox = 0,
		oy = 0,
		angle = 0,
		_visible = true --visibility
	}
	setmetatable(object, EntityMeta)  -- Inheritance
	if parent then parent:append(object) end
	return object
end

--Sets vars of entity
function Entity:set(vars)
	for k, v in pairs(vars) do
		self[k] = v
	end
	return self --so we can chain methods
end
--Sets x and y position of entity
function Entity:move(x, y)
	self.x = x or self.x or 0
	self.y = y or self.y or 0
	return self --so we can chain methods
end
--Sets x scale and y scale
function Entity:scale(sx, sy)
	self.sx = sx or 1
	self.sy = sy or 1
	return self --so we can chain methods
end
--Sets radius of entity
function Entity:radius(R)
	self.R = R or self.R or 0
	return self --so we can chain methods
end
--Sets width and height of entity
function Entity:size(w, h)
	self.w = w or self.w or 0
	self.h = h or self.h or 0
	return self --so we can chain methods
end
--Sets the origin of entity
function Entity:offset(ox, oy)
	self.ox = ox or self.ox or 0
	self.oy = oy or self.oy or 0
	return self --so we can chain methods
end
--Sets angle (rotation) of entity
function Entity:rotate(angle)
	self.angle = angle or self.angle or 0
	return self --so we can chain methods
end
--predefined colors
local colors = {
	aliceblue = 'f0f8ff',
	antiquewhite = 'faebd7',
	aqua = '00ffff',
	aquamarine = '7fffd4',
	azure = 'f0ffff',
	beige = 'f5f5dc',
	bisque = 'ffe4c4',
	black = '000000',
	blanchedalmond = 'ffebcd',
	blue = '0000ff',
	blueviolet = '8a2be2',
	brown = 'a52a2a',
	burlywood = 'deb887',
	cadetblue = '5f9ea0',
	chartreuse = '7fff00',
	chocolate = 'd2691e',
	coral = 'ff7f50',
	cornflowerblue = '6495ed',
	cornsilk = 'fff8dc',
	crimson = 'dc143c',
	cyan = '00ffff',
	darkblue = '00008b',
	darkcyan = '008b8b',
	darkgoldenrod = 'b8860b',
	darkgray = 'a9a9a9',
	darkgrey = 'a9a9a9',
	darkgreen = '006400',
	darkkhaki = 'bdb76b',
	darkmagenta = '8b008b',
	darkolivegreen = '556b2f',
	darkorange = 'ff8c00',
	darkorchid = '9932cc',
	darkred = '8b0000',
	darksalmon = 'e9967a',
	darkseagreen = '8fbc8f',
	darkslateblue = '483d8b',
	darkslategray = '2f4f4f',
	darkslategrey = '2f4f4f',
	darkturquoise = '00ced1',
	darkviolet = '9400d3',
	deeppink = 'ff1493',
	deepskyblue = '00bfff',
	dimgray = '696969',
	dimgrey = '696969',
	dodgerblue = '1e90ff',
	firebrick = 'b22222',
	floralwhite = 'fffaf0',
	forestgreen = '228b22',
	fuchsia = 'ff00ff',
	gainsboro = 'dcdcdc',
	ghostwhite = 'f8f8ff',
	gold = 'ffd700',
	goldenrod = 'daa520',
	gray = '808080',
	grey = '808080',
	green = '008000',
	greenyellow = 'adff2f',
	honeydew = 'f0fff0',
	hotpink = 'ff69b4',
	indianred  = 'cd5c5c',
	indigo  = '4b0082',
	ivory = 'fffff0',
	khaki = 'f0e68c',
	lavender = 'e6e6fa',
	lavenderblush = 'fff0f5',
	lawngreen = '7cfc00',
	lemonchiffon = 'fffacd',
	lightblue = 'add8e6',
	lightcoral = 'f08080',
	lightcyan = 'e0ffff',
	lightgoldenrodyellow = 'fafad2',
	lightgray = 'd3d3d3',
	lightgrey = 'd3d3d3',
	lightgreen = '90ee90',
	lightpink = 'ffb6c1',
	lightsalmon = 'ffa07a',
	lightseagreen = '20b2aa',
	lightskyblue = '87cefa',
	lightslategray = '778899',
	lightslategrey = '778899',
	lightsteelblue = 'b0c4de',
	lightyellow = 'ffffe0',
	lime = '00ff00',
	limegreen = '32cd32',
	linen = 'faf0e6',
	magenta = 'ff00ff',
	maroon = '800000',
	mediumaquamarine = '66cdaa',
	mediumblue = '0000cd',
	mediumorchid = 'ba55d3',
	mediumpurple = '9370d8',
	mediumseagreen = '3cb371',
	mediumslateblue = '7b68ee',
	mediumspringgreen = '00fa9a',
	mediumturquoise = '48d1cc',
	mediumvioletred = 'c71585',
	midnightblue = '191970',
	mintcream = 'f5fffa',
	mistyrose = 'ffe4e1',
	moccasin = 'ffe4b5',
	navajowhite = 'ffdead',
	navy = '000080',
	oldlace = 'fdf5e6',
	olive = '808000',
	olivedrab = '6b8e23',
	orange = 'ffa500',
	orangered = 'ff4500',
	orchid = 'da70d6',
	palegoldenrod = 'eee8aa',
	palegreen = '98fb98',
	paleturquoise = 'afeeee',
	palevioletred = 'd87093',
	papayawhip = 'ffefd5',
	peachpuff = 'ffdab9',
	peru = 'cd853f',
	pink = 'ffc0cb',
	plum = 'dda0dd',
	powderblue = 'b0e0e6',
	purple = '800080',
	red = 'ff0000',
	rosybrown = 'bc8f8f',
	royalblue = '4169e1',
	saddlebrown = '8b4513',
	salmon = 'fa8072',
	sandybrown = 'f4a460',
	seagreen = '2e8b57',
	seashell = 'fff5ee',
	sienna = 'a0522d',
	silver = 'c0c0c0',
	skyblue = '87ceeb',
	slateblue = '6a5acd',
	slategray = '708090',
	slategrey = '708090',
	snow = 'fffafa',
	springgreen = '00ff7f',
	steelblue = '4682b4',
	tan = 'd2b48c',
	teal = '008080',
	thistle = 'd8bfd8',
	tomato = 'ff6347',
	turquoise = '40e0d0',
	violet = 'ee82ee',
	wheat = 'f5deb3',
	white = 'ffffff',
	whitesmoke = 'f5f5f5',
	yellow = 'ffff00',
	yellowgreen = '9acd32',
}
--Sets color of entity
function Entity:color(r, g, b, a)
	if r and type(r) == 'string' then
		r = r:lower()
		if colors[r] then
			r = colors[r]
		elseif not r:match('^[a-f0-9][a-f0-9][a-f0-9][a-f0-9][a-f0-9][a-f0-9]$') then
			print('Cannot recognise color: '..r)
			return self
		end
		self.r = tonumber(r:sub(1,2), 16)
		self.g = tonumber(r:sub(3,4), 16)
		self.b = tonumber(r:sub(5,6), 16)
		return self
	end
	self.r = r or self.r or 255
	self.g = g or self.g or 255
	self.b = b or self.b or 255
	self.a = a or self.a or 255
	return self --so we can chain methods
end
--Sets opacity of entity
function Entity:opacity(a)
	self.a = a or self.a or 255
	return self --so we can chain methods
end
--hide entity (stop processing events and drawing) children will be hidden too
function Entity:hide()
	self._visible = false
	return self --so we can chain methods
end
--show hidden entity
function Entity:show()
	self._visible = true
	return self --so we can chain methods
end
--toggle entity
function Entity:toggle()
	self._visible = not self._visible
	return self --so we can chain methods
end
--append child
function Entity:append(child)
	if not self._child then self._child = {} end
	table.insert(self._child, child)
	child._parent = self
	return self --so we can chain methods
end

--Animates all values of entity to the given values in keys with the given speed
--examples:
--ent:animate({x=100,y=100}, 0.3) - move entity to 100, 100 for 300 msecs
--ent:animate({r=0,g=0,b=0,a=0}, {speed=0.3, easing='linear'}) - fade down with given easing function
--ent:animate({value=29, frame=53}, 2) - animate specific parameters of entity
local emptyArray={}
function Entity:animate(keys, options)
	if keys then
		if not self._animQueue then self._animQueue = {} end
		if not options then 
			options = emptyArray
		--each option has different type
		elseif type(options) == "number" then
			options = {speed = options}
		elseif type(options) == "function" then
			options = {cb = options}
		elseif type(options) == "string" then
			options = {queue = options}
		elseif type(options) == "boolean" then
			options = {loop = options}
		end
		local queue = options.queue or "main" --you can manage queues
		if not self._animQueue[queue] then self._animQueue[queue] = {} end
		table.insert(self._animQueue[queue], {
			keys = keys,
			old = {},
			speed = options.speed or 0.3,
			lasttime = nil, 
			easing = easing[options.easing] or easing.swing,
			loop = options.loop or false,
			callback = options.callback or options.cb,
			a = options.a,
			b = options.b
		})
	end
	return self --so we can chain methods
end

--delay between animations inn one queue
function Entity:delay(options)
	return self:animate({}, options)
end

--stop animation
--ent:stop() - stop all animations
--ent:stop('anim_group_1') - stop all animatios in queue 'anim_group_1'
function Entity:stop(queue)
	if not self._animQueue then self._animQueue = {} end
	if queue then
		self._animQueue[queue] = {}
	else
		self._animQueue = {}
	end
	return self --so we can chain methods
end

--bounding functions
Entity.bounds = {
	rectangle = function(ent, mouseX, mouseY)
		return  ent.w and ent.h
						and ent.x < mouseX 
						and ent.y < mouseY 
						and ent.x + ent.w > mouseX 
						and ent.y + ent.h > mouseY
	end,
	circle = function(ent, mouseX, mouseY)
		return ent.R and (math.pow(mouseX-ent.x, 2)+math.pow(mouseY-ent.y, 2) < ent.R*ent.R)
	end
}

--set bounding function for interaction with mouse
function Entity:bound(callback)
	if callback then self._bound = callback end
	return self --so we can chain methods
end

--до сих пор охреневаю как я это сделал. Куча методов в пяти строчках кода
--callbacks
for k, v in pairs({'click', 'dblclick', 'mousepress', 'mouserelease', 'mouseover', 'mouseout', 'mousemove', 'keypress', 'keyrelease', 'keyrepeat', 'wheel'}) do
	Entity[v] = function (self, callback, a, b, c)
		if callback and type(callback) == 'function' then
			if not self._bound then self._bound = Entity.bounds.rectangle end
			self._control = true
			self['_' .. v] = callback
		else
			self['_' .. v](self, callback, a, b, c)
		end
		return self
	end
end
--aliases
Entity.keypressed = Entity.keypress
Entity.keyPressed = Entity.keypress
Entity.keyPress = Entity.keypress
Entity.keyreleased = Entity.keyrelease
Entity.keyRelease = Entity.keyrelease
Entity.keyReleased = Entity.keyrelease
Entity.keyRepeat = Entity.keyrepeat
Entity.doubleclick = Entity.dblclick
Entity.doubleClick = Entity.dblclick
Entity.mousepressed = Entity.mousepress
Entity.mousePressed = Entity.mousepress
Entity.mousePress = Entity.mousepress
Entity.mousereleased = Entity.mouserelease
Entity.mouseReleased = Entity.mouserelease
Entity.mouseRelease = Entity.mouserelease
Entity.mouseOver = Entity.mouseover
Entity.mouseOut = Entity.mouseout
Entity.mouseWheel = Entity.wheel
Entity.mousewheel = Entity.wheel
Entity.mouseclick = Entity.click
Entity.mouseClick = Entity.click
Entity.mouseMove = Entity.mousemove



--object can get focus
function Entity:focus(f)
	self._focus = f or true
end

--special event: calls when parameter key was changed
function Entity:update(key, func)
	if type(func) == 'function' then
		if not self.__data then
			local data = {}
			setmetatable(self, nil)
			for k, v in pairs(self) do
				data[k] = v
				self[k] = nil
			end
			self.__data = data
			self.upd = {}
			setmetatable(self, EntityMetaUpdate)
		end
		if type(key) == 'table' then
			for _, v in ipairs(key) do self.upd[v] = func end
		else
			self.upd[key] = func
		end
	end
	return self --so we can chain methods
end

local stdDrawTrans = function(s)
	C.push()
	C.translateObject(s.x, s.y, s.angle, s.w, s.h, s.ox, s.oy)
end

local stdDrawColor = function(s)
	C.color(s.r or 255, s.g or 255, s.b or 255, s.a or 255)
end

function Entity:draw(callback)
	--~ print(type(callback))
	--~ if type(callback) == 'function' then
		if not self._draw then
			self._draw = callback
		else
			if type(self._draw) ~= 'table' then self._draw = {self._draw} end
			table.insert(self._draw, callback)
		end
		--~ if not self._draw then
			--~ self._draw = {}
		--~ end
		--~ table.insert(self._draw, callback)
	--~ end
	return self --so we can chain methods
end

function Entity:translate(callback)
	self._translate = callback or stdDrawTrans
	return self --so we can chain methods
end

lQuery.drag_start = function(s, x, y)
	s._drag_x = x - s.x
	s._drag_y = y - s.y
	lQuery._drag_object = s
end
lQuery.drag_end = function(s)
	lQuery._drag_object = nil
end
lQuery.addhook(function()
	local s = lQuery._drag_object
	if s then
		s.x = mX - s._drag_x
		s.y = mY - s._drag_y
		if s._drag_bound then
			local a = s._drag_bound
			if s.x > a[2] then s.x = a[2] end
			if s.x < a[4] then s.x = a[4] end
			if s.y > a[3] then s.y = a[3] end
			if s.y < a[1] then s.y = a[1] end
		end
		if s._drag_callback then s._drag_callback(s, x, y) end
	end
end)
function Entity:draggable(options)
	local o = options or {}
	if o.bound then self._drag_bound = o.bound end --[[top, right, bottom, left]]
	if o.callback then self._drag_callback = o.callback end
	return self:mousepress(lQuery.drag_start):mouserelease(lQuery.drag_end)
end


--delete object
--how to remove object correctly and free memory:
--a = a:delete()
--~ function Entity:delete()
	--~ local i = 0
	--~ for k, v in pairs(self._parent._child) do
		--~ i = i + 1
		--~ if v == self then
			--~ table.remove(self._parent._child, i)
			--~ return nil
		--~ end
	--~ end
	--~ return nil
--~ end

--screen - parent entity for all entities. Drawing function recursively process all entities from it.
screen = Entity:new()

local function animate(ent)
	for i, j in pairs(ent._animQueue) do
		if j[1] then 
			aq = j[1]
			if not aq._keys then
				if type(aq.keys) == 'function' then
					aq._keys = aq.keys()
				else
					aq._keys = aq.keys
				end
			end
			if not aq.lasttime then
				aq.lasttime = time
				for k, v in pairs(aq._keys) do
					aq.old[k] = ent[k]
				end
			end
			
			if aq.lasttime + aq.speed <= time or lQuery.fx == false then
				for k, v in pairs(aq._keys) do
					ent[k] = v
				end
				if aq.loop == true then
					aq._keys = nil
					aq.lasttime = nil
					aq.old = {}
					table.insert(j, aq) 
				end
				table.remove(j, 1)
				if aq.callback then aq.callback(ent) end
				animate(ent)
			else
				for k, v in pairs(aq._keys) do
					if ent[k] and type(ent[k]) == 'number' then ent[k] = aq.easing(time - aq.lasttime, aq.old[k], v - aq.old[k], aq.speed, aq.a, aq.b) end
				end
			end --if aq.lasttime + vv.speed <= time
		end --if j[1]
	end --for
end


--some events
local function events(v)
	if lQuery.KeyPressed == true then 
		if v._keypress then
			if not v._key or v._key ~= lQuery.KeyPressedKey then
				v._keypress(v, lQuery.KeyPressedKey, lQuery.KeyPressedUni)
			end
		end
		if not v._key or v._key ~= lQuery.KeyPressedKey then
			v._KeyPressedCounter = 1
		end
		if v._keyrepeat and (v._KeyPressedCounter == 1 or 
				 v._KeyPressedCounter == 2 and time - v._KeyPressedTime > 0.3 or
				 v._KeyPressedCounter > 2 and time - v._KeyPressedTime > 0.05) then 
			v._KeyPressedTime = time
			v._KeyPressedCounter = v._KeyPressedCounter + 1
			v._keyrepeat(v, lQuery.KeyPressedKey, lQuery.KeyPressedUni)
		end
		v._key = lQuery.KeyPressedKey
	else
		if v._keyrelease then
			if v._key and v._key == true then
				v._keyrelease(v, lQuery.KeyPressedKey, lQuery.KeyPressedUni)
			end
		end
		v._key = false
	end
	if v._bound and v._bound(v, mX, mY) then
		if v._mousemove then 
			v._mousemove(v, mX, mY)
		end 
		if lQuery.MouseButton == "wu" and lQuery.MousePressed == true then 
			if v._wheel then
				lQuery.MouseButton = nil
				v._wheel(v, mX, mY, "u")
			end
		elseif lQuery.MouseButton == "wd" and lQuery.MousePressed == true then 
			if v._wheel then
				lQuery.MouseButton = nil
				v._wheel(v, mX, mY, "d")
			end
		elseif lQuery.MousePressed == true and lQuery._MousePressedOwner == true then 
			lQuery.MousePressedOwner = v
		end
		lQuery.hover = v
	--~ else
		--~ if lQuery._hover == v then 
			--~ lQuery._hover = nil
			--~ lQuery.hover = nil
			--~ if v._mouseout then v._mouseout(v, mX, mY) end
			--if v == lQuery.MousePressedOwner then lQuery.MousePressedOwner = nil end
		--~ end
	end
end

local function process_entities(ent)
	if ent._visible == true then 
		if ent._animQueue then 
			animate(ent) 
		end
		if ent._control then --if controlled
			events(ent)
		end
		if ent._draw then
			if ent.r then C.color(ent.r or 255, ent.g or 255, ent.b or 255, ent.a or 255) end
			if ent._translate then 
				ent._translate(ent)
			end
			if type(ent._draw) == 'function' then
				ent._draw(ent)
			else
				for i = 1, #ent._draw do
					ent._draw[i](ent)
				end
			end
			if ent._translate then 
				C.pop()
			end
		end
		if ent._child then 
			for i = 1, #ent._child do
				process_entities(ent._child[i])
			end
		end
	end
end

lQuery.event = function(e, a, b, c)
	if e == "mp" then
		lQuery.MousePressed = true
		lQuery.MouseButton = c
		lQuery._MousePressedOwner = true
	elseif e == "mr" then 
		lQuery.MousePressed = false
		lQuery.MouseButton = c
		--click handler
		local v = lQuery.MousePressedOwner
		if v --[[and v._bound and v._bound(v, mX, mY)]] then
			local v = lQuery.MousePressedOwner
			if v._mouserelease then 
				v._mouserelease(v, mX, mY, c)
			end
			if v._bound and v._bound(v, mX, mY) then 
				if  lQuery._last_click_millis + lQuery.dblclickInterval > time 
				and lQuery._last_clickX == mX and lQuery._last_clickY == mY then
					if v._dblclick then v._dblclick(v, mX, mY, c) end
				else
					if v._click then v._click(v, mX, mY, c) end
				end
				lQuery._last_click_millis = time
				lQuery._last_clickX = mX
				lQuery._last_clickY = mY
				if v._focus then lQuery.focus = v end
			end
		end
		lQuery.MousePressedOwner = nil
	elseif e == "kp" then
		lQuery.KeyPressed = true
		lQuery.KeyPressedKey = a
		lQuery.KeyPressedUni = b
	elseif e == "kr" then
		lQuery.KeyPressed = false
	elseif e == "rz" then
		screen_width = a
		screen_height = b
		if lQuery._onresize[1] then
			for i = 1, #lQuery._onresize do
				lQuery._onresize[i](a, b)
			end
		end
	elseif e == "q" then
		if atexit then atexit() end
	end
end

lQuery.process = function()
	for _, v in pairs(lQuery.hooks) do v() end
	lQuery.hover = nil
	if screen then process_entities(screen) end
	if Console then process_entities(Console) end
	
	--fix mousepress bug
	local v = lQuery.MousePressedOwner
	if v and lQuery._MousePressedOwner == true then
		if v._mousepress then 
			v._mousepress(v, mX, mY, lQuery.MouseButton)
		end
	end
	
	local v = lQuery.lasthover
	if v ~= lQuery.hover then
		if v and v._mouseout then v._mouseout(v, mX, mY) end --out MUST be before over
		v = lQuery.hover
		if v and v._mouseover then v._mouseover(v, mX, mY) end
		lQuery.lasthover = lQuery.hover
	end
	lQuery._MousePressedOwner = false
end
