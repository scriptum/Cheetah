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

local C = cheetah

--image
local image_draw = function(s)
	s._image:draw(s.x, s.y, s.w, s.h, s.angle, s.ox, s.oy)
end
local image_draw_quad = function(s)
	s._image:drawq(s.x, s.y, s.w, s.h, s.qx, s.qy, s.w, s.h, s.angle, s.ox, s.oy)
end
function Entity:image(image, options)
 if image and not self._image then
		if type(image) == 'string' then
			image = C.newImage(image, options)
		end
		self._image = image
		self.w = image:getWidth()
		self.h = image:getHeight()
		self:draw(image_draw)
	end
	return self
end
function Entity:imageq(image, options)
 if image and not self._image then
		if type(image) == 'string' then
			image = C.newImage(image, options)
		end
		self._image = image
		self.w = image:getWidth()
		self.h = image:getHeight()
		self:draw(image_draw_quad)
		self.qx = 0
		self.qy = 0
		self.qw = self.w
		self.qh = self.h
	end
	return self
end
--border-image
function Entity:borderImage(image, top, right, bottom, left, options)
 if image and not self._image then
		if type(image) == 'string' then
			image = C.newBorderImage(image, top, right, bottom, left, options)
		end
		self._image = image
		self._draw = image_draw
		self.w = image:getWidth()
		self.h = image:getHeight()
	end
	return self
end

local end_camera = function()
	C.pop()
end
local draw_camera = function(s)
	C.push()
	C.move(s.x,s.y)
	C.scale(s.sx,s.sy)
end
function Entity:camera()
	self.sx = 1
	self.sy = 1
	self:draw(draw_camera)
	self:postprocess(end_camera)
	self.zoom = Entity.scale
	return self
end

--fps
local draw_fps = function(s)
	local defFont = C.fonts.default
	defFont._scale = s.scale
	defFont:print(C.FPS, s.x, s.y)
	defFont._scale = s.scale
end

function Entity:fps(scale)
	self.scale = scale or 2
	return self:draw(draw_fps)
end

--text
local draw_text = function(s)
	s.font:print(s.text, s.x, s.y)
end

function Entity:text(text, font, scale)
	self.font = font or C.fonts.default
	if font == nil then
		self.scale = scale or 2
	else
		self.scale = scale or 1
	end
	self.font:scale(self.scale)
	self.text = text or ''
	return self:draw(draw_text)
end
