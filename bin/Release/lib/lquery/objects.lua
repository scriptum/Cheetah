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

--point
local point_draw = function(s)
  C.point()
end
function Entity:point(R)
  self.R = R or 1
  self:draw(point_draw)
  self._bound = Entity.bounds.circle
  return self
end
--rect
local rect_draw = function(s)
  C.rectangle()
end
function Entity:rectangle()
  self:draw(rect_draw)
  return self
end
--circle
--~ local circle_draw = function(s)
  --~ G.circle("fill", s.x, s.y, s.R, 2*s.R)
--~ end
--~ function Entity:circle(radius)
  --~ self.R = radius or 10
  --~ self._draw=circle_draw
  --~ self._bound = Entity.bounds.circle
  --~ return self
--~ end
local C = cheetah
--text
local text_draw = function(s)
  --~ G.fontSize = s.fontSize or 12
  --~ if s.w then
    --~ Gprintf(s.text, s.x, s.y, s.w, s.align)
  --~ else
    --~ Gprint(s.text, s.x, s.y)
  --~ end
  S.print(s.text, s.x, s.y, s.w, s.align)
end
function Entity:text(text, font, align)
  self.font = font
  self.text = text or ''
  self.align = align
  self._draw = text_draw
  return self
end

--image
local image_draw = function(s)
  s._image:drawxy(s.x,s.y,s.w,s.h)
end
local image_draw_quad = function(s)
  s._image:drawq(s.qx, s.qy, s.w, s.h)
end
function Entity:image(image, options)
 if image and not self._image then
    if type(image) == 'string' then 
      image = C.newImage(image)
    end
    self._image = image
    self.w = image.w
    self.h = image.h
    if options and options.quad and options.quad == true then 
      self:draw(image_draw_quad)
      self.qx = 0
      self.qy = 0
      self.qw = self.w
      self.qh = self.h
    else
      self:draw(image_draw)
    end
  end
  return self
end

--border-image
local border_image_draw = function(s)
  local i = s._image
  local x = s.x
  local w = s.orig_w
  local h = s.orig_h
  --~ local sw = math.ceil(s.w)
  --~ local sw = math.ceil(s.h)
  local y = s.y
  local t = s.top
  local r = s.right
  local b = s.bottom
  local l = s.left
  if t > 0 then
  i:drawq(x,            y,            0, l,            t,           0,     0,       l,         t)
  i:drawq(x + l,        y,            0, s.w - l - r,  t,           l,     0,       w - l - r, t)
  i:drawq(x + s.w - r,  y,            0, r,            t,           w - r, 0,       r,         t)
  end
  
  i:drawq(x,            y + t,        0, l,            s.h - t - b, 0,     t,       l,         h - t - b)
  i:drawq(x + l,        y + t,        0, s.w - l - r,  s.h - t - b, l,     t,       w - l - r, h - t - b)
  i:drawq(x + s.w - r,  y + t,        0, r,            s.h - t - b, w - r, t,       r,         h - t - b)
  if b > 0 then
  i:drawq(x,            y + s.h - b,  0, l,            b,           0,     h - b,   l,         b)
  i:drawq(x + l,        y + s.h - b,  0, s.w - l - r,  b,           l,     h - b,   w - l - r, b)
  i:drawq(x + s.w - r,  y + s.h - b,  0, r,            b,           w - r, h - b,   r,         b)
  end
end
lQuery.border_image_draw = border_image_draw
function Entity:border_image(image, top, right, bottom, left)
 if image then
    if type(image) == 'string' then image = S.newImage(image) end
    self._image = image
    self._draw = border_image_draw
    w = image:getWidth()
    h = image:getHeight()
    self.w = w
    self.h = h
    self.orig_w = w
    self.orig_h = h
    self.top = top
    self.right = right
    self.bottom = bottom
    self.left= left
        
    --~ self.angle = 0
    --~ self.sx = 1
    --~ self.sy = 1
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



local draw_fps = function(s)
	local defFont = C.fonts.default
	defFont._scale = 2
	defFont:print('FPS: '..C.FPS, s.x, s.y)
	defFont._scale = 1
end

function Entity:fps()
	return self:draw(draw_fps)
end