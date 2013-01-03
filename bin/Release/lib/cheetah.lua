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


local ffi = require 'ffi'
require 'lib.table'
require 'lib.string'

cheetah = {}

local C = cheetah

C.getFile = function(filename)
	local file = assert(io.open(filename))
	local contents = file:read '*a'
	file:close()
	return contents
end

C.putFile = function(filename, str, writemode)
	local file = assert(io.open(filename, writemode or "w"))
	file:write(str)
	file:close()
end

C.fileExists = function(name)
	local f = io.open(name, 'r')
	if f then
		f:close()
		return true
	end
	return false
end

C.appendFile = function(filename, str)
	C.putFile(filename, str, "wa")
end

local archToInt = {x86 = 32, x64 = 64}

C.loadDLL = function(filename)
	local res
	if ffi.os == "Windows" then
		res = ffi.load('./bin/win32/' .. filename .. '.dll')
	elseif ffi.os == "OSX" then
		res = ffi.load('./bin/macosx/' .. filename .. '.dynlib')
	else
		res = ffi.load('./bin/linux'..archToInt[jit.arch]..'/lib' .. filename .. '.so')
	end
	if res then
		local header = 'lib/'..filename..'.h'
		if C.fileExists(header) then
			ffi.cdef(C.getFile(header))
		end
		print('Successfully loaded module: '..filename)
		return res
	else
		print('Error: cannot load module '..filename)
		return nil
	end
end
C.module = C.loadDLL

ffi.cdef [[
void printf(const char * str, ...);
int sscanf ( const char * str, const char * format, ...);
int (*grabCursor)(int mode);
]]

local libcheetah = C.loadDLL 'cheetah'
assert(libcheetah, 'Cannot load cheetah library!')
--~ if not libcheetah.initSDL() then print 'Cannot init SDL!' end
--~ libcheetah.init('C engine', 640, 480, 32, '')

require "lib.gl"
--~ 
--~ local gl = GL

local lua_keys = require 'lib.keys'
local keys_reverse = require 'lib.keys_reverse'
local button_names = {"l","m","r","wu","wd"}
local lua_events = {"q","kp","kr","mp","mr","rz"}

local done = false
local FPS = 60
C.FPS = "60"
C.printFPS = false
time = 0
local lasttime = 0
local realtime
C.mainLoop = function()
	assert(libcheetah.isInit(), 'You forgot about cheetah.init')
	while done == false do
		time = libcheetah.getTime()
		realtime = libcheetah.getRealTime()
		libcheetah.prepare()
		if C.render then C.render() end
		libcheetah.swapBuffers()
		--~ libcheetah.doAutoScale()
		FPS = (FPS + 1) / (1 + (libcheetah.getRealTime() - realtime));
		if time - lasttime > 0.5 then
			lasttime = time
			if C.printFPS then 
				print(C.FPS..' '..gcinfo())
				C.setTitle(C.FPS..' '..gcinfo())
			end
			C.FPS = tostring(math.floor(FPS))
		end
	end
end

C.quit = function()
	done = true
end

--~ --first time when you try to load an image this function autoloads SOIL library
--~ C.newImage = function(name, opt)
	--~ require 'lib.SOIL'
	--~ return C.newImage(name, opt)
--~ end

--~ C.setColor = function(r,g,b,a)
	--~ if type(r) == 'table' then
		--~ libcheetah.color(r[1] or 255, r[2] or 255, r[3] or 255, r[4] or 255)
	--~ else
		--~ libcheetah.color(r or 255, g or 255, b or 255, a or 255)
	--~ end
--~ end

C.getWindowSize = function()
	return libcheetah.getWindowWidth(), libcheetah.getWindowHeight()
end

C.poll = function()
	local e = lua_events[libcheetah.getEventType()]
	local a, b, c
	if e == 'q' then
		done = true
	elseif e == 'kp' or e == 'kr' then 
		a = libcheetah.getEventKey()
		a, b = lua_keys[a] or 'key_' .. a, libcheetah.getEventKeyUnicode()
	elseif e == 'mp' or e == 'mr' then 
		c = libcheetah.getEventMouseButton()
		a, b, c = libcheetah.getEventMouseX(), libcheetah.getEventMouseY(), button_names[c] or 'm_' .. c
	elseif e == 'rz' then
		a, b = libcheetah.getEventResizeW(), libcheetah.getEventResizeH()
	else
		return nil
	end
	return e, a, b, c
end

C.getMousePos = function()
	return libcheetah.getMouseX(), libcheetah.getMouseY()
end

C.getFps = function()
	return FPS
end

--~ C.drawMultitexture = function(...)
	--~ if #arg < 2 then
		--~ libcheetah.myError 'drawMultitexture: pass at least 2 images.'
	--~ end
	--~ libcheetah.enableTexture2D()
	--~ for i, v in ipairs(arg) do
		--~ libcheetah.activeTexture(i-1)
		--~ libcheetah.bindTexture(v.id)
	--~ end
	--~ libcheetah.activeTexture(0)
	--~ libcheetah.rectangle(true)
	--~ libcheetah.disableTexture2D()
--~ end

--============================================================================--
--                                 FILESYSTEM                                 --
--============================================================================--

--[[
@descr Get extension of file. Returns just last word after dot.
@group file
]]
C.fileExt = function(name)
	return name:gsub('^.*%.', '')
end

--[[
@descr Get file name (without extension). Returns just all before last dot.
@group file
]]
C.fileName = function(name)
	return name:gsub('%..*', '')
end


--exec func for each file in directory
C.fileEach = function(dirname, func)
	if not dirname or type(dirname) ~= 'string' then
		libcheetah.myError('fileEach: you must specify directory name')
		return
	end
	if not func then
		libcheetah.myError('fileEach: you must specify callback function')
		return
	end
	local dir = libcheetah.openDir(dirname)
	if not libcheetah.isPointer(dir) then
		libcheetah.myError('fileEach: cannot open dir %s', dirname)
		return
	end
	local de = libcheetah.readDir(dir)
	local s, ext, n, name
	while libcheetah.isPointer(de) do
		func(ffi.string(libcheetah.getDirentName(de)))
		de = libcheetah.readDir(dir)
	end
	return t
end

--[[
@descr Get full path to directory where you may save any data.
@group file
]]
C.getAppDataDir = function()
	if ffi.os == "Windows" then
		return os.getenv("APPDATA")
	else
		local home = os.getenv("HOME")
		if ffi.os == "OSX" then
			return home .. '/Library/Application Support'
		else
			return home .. '/.config'
		end
	end
end

setmetatable(C, { __index = libcheetah})

local keyState = libcheetah.getKeyState()

C.isKeyPressed = function(key)
	if key and keys_reverse[key] then
		return keyState[keys_reverse[key]] == 1
	end
	return false
end

--============================================================================--
--                            GENERAL DRAW FUNCTION                           --
--============================================================================--

local draw_general = function(d1, d2)
	return function(s, x, y, w, h, angle, ox, oy)
		if angle then
			d1(s, x or 0, y or 0, w or s.w, h or s.h, angle, ox or 0, oy or 0)
		else
			d2(s, x or 0, y or 0, w or s.w, h or s.h)
		end
	end
end
local drawq_general = function(d1, d2)
	return function(s, x, y, w, h, qx, qy, qw, qh, a, ox, oy)
		if angle then
			d1(s, x or 0, y or 0, w or s.w, h or s.h, 
				 qx or 0, qy or 0, qw or s.w, qh or s.h, 
				 angle, ox or 0, oy or 0)
		else
			d2(s, x or 0, y or 0, w or s.w, h or s.h, 
				 qx or 0, qy or 0, qw or s.w, qh or s.h)
		end
	end
end

--============================================================================--
--                                    IMAGE                                   --
--============================================================================--

C.generate = function(imageType, w, h, opt)
	local ptr = ffi.new('Image')
	libcheetah.generateImage(ptr, w, h, imageType, opt or '')
	return ptr
end

C.newImage = function(name, options)
	local ptr = ffi.new('Image')
	libcheetah.newImageOpt(ptr, name, options or '')
	return ptr
end

ffi.metatype('Image', {
	__index = {
		draw = draw_general(libcheetah.imageDrawt, libcheetah.imageDrawxy),
		drawq = drawq_general(libcheetah.imageDrawqt, libcheetah.imageDrawqxy),
		getWidth = function(s) return s.w end,
		getHeight = function(s) return s.h end
	}, 
	__gc = libcheetah.deleteImage
})

C.newImageData = function(w, h, channels)
	local ptr = ffi.new('ImageData')
	ptr.w = w or 1
	ptr.h = h or 1
	ptr.channels = channels == 3 and 3 or 4
	ptr.data = ffi.new('char[?]', w * h * channels)
	return ptr
end

C.newImageFromData = function(data, options)
	local ptr = ffi.new('Image')
	libcheetah._newImageFromData(ptr, data, options or '')
	return ptr
end

--============================================================================--
--                                BORDER IMAGE                                --
--============================================================================--

C.newBorderImage = function(name, top, right, bottom, left, options)
	local ptr = ffi.new('BorderImage')
	ptr.image = C.newImage(name, options)
	ptr.top = top or 0
	ptr.right = right or 0
	ptr.bottom = bottom or 0
	ptr.left = left or 0
	return ptr
end

ffi.metatype('BorderImage', {
	__index = {
		draw = draw_general(libcheetah.borderImageDrawt, libcheetah.borderImageDrawxy),
		drawBorderOnly = function(s, borderOnly) s.borderOnly = borderOnly or false end,
		getWidth = function(s) return s.image.w end,
		getHeight = function(s) return s.image.h end
	}
})

--============================================================================--
--                               MULTITEXTURE                                 --
--============================================================================--

C.newMultitexture = function(...)
	local arg = {...}
	if #arg < 2 then
		libcheetah.myError 'drawMultitexture: pass at least 2 images.'
		if arg[1] then return arg[1]
		else return nil end
	end
	p = ffi.new('Multitexture')
	p.w = arg[1].w
	p.h = arg[1].h
	p.size = #arg
	libcheetah.initMultitexture(p)
	for i, v in ipairs(arg) do
		p.images[i-1] = arg[i]
	end
	return p
end

ffi.metatype('Multitexture', {
	__index = {
		draw = draw_general(libcheetah.multitextureDrawt, libcheetah.multitextureDrawxy),
		drawq = drawq_general(libcheetah.multitextureDrawqt, libcheetah.multitextureDrawqxy),
		getWidth = function(s) return s.w end,
		getHeight = function(s) return s.h end
	}, 
	__gc = libcheetah.deleteMultitexture
})

--============================================================================--
--                                    ATLAS                                   --
--============================================================================--

C.newAtlas = function(image, x, y, w, h)
	local p = ffi.new('Atlas')
	p.image = image
	p.w, p.h = w, h
	p.tex[0] = x
	p.tex[1] = y
	p.tex[2] = x
	p.tex[3] = y + h
	p.tex[4] = x + w
	p.tex[5] = y + h
	p.tex[6] = x + w
	p.tex[7] = y
	return p
end

ffi.metatype('Atlas', {
	__index = {
		draw = draw_general(libcheetah.atlasDrawt, libcheetah.atlasDrawxy)
	}
})

--============================================================================--
--                                    FONT                                    --
--============================================================================--

local texturesArchive = {}
--~ C.newTilemap = function(file)
	--~ local f = assert(io.open(file))
	--~ local image, w, h, tw, th = f:read('*l'):match('([^ ]+) (%d+) (%d+) (%d+) (%d+)')
	--~ assert(h and w and image, 'Invalid tilemap file')
	--~ local ptr = ffi.new('Tilemap')
	--~ image = file:gsub('[^/]+$', image)
	--~ local img = C.newImage(image)
	--~ ptr.img = img
	--~ table.insert(texturesArchive, img)
	--~ ptr.w, ptr.h, ptr.tw, ptr.th = tonumber(w), tonumber(h), tonumber(tw), tonumber(th)
	--~ libcheetah.newTilmapInternal(ptr, file)
	--~ return ptr
--~ end

--~ ffi.metatype('Tilemap', {
	--~ __index = {
		--~ draw = function(s, x, y, angle, zoom, ox, oy)
			--~ libcheetah.tilemapDraw(s, x or 0, y or 0, angle or 0, zoom or 1, ox or 0, oy or 0)
		--~ end
	--~ }, 
	--~ __gc = libcheetah.deleteTilemap
--~ })

--~ local resLoadedImages
C.fonts = {}
C.newFont = function(name, scalable, codepage)
	local a, b, c, d, font, img
	local millis = C.getTicks()
	local glyphs = 0
	local bytes = 0
	local p = ffi.new('float[8]')
	for line in io.lines(name) do
		a = line:match('^textures: (.+)')
		if a then
			n = name:gsub('[^/]+$', a)
			img = ffi.new('Image')
			if scalable then
				libcheetah.newImageOpt(img, n, 'i') --disable delayed loader
			else
				libcheetah.newImageOpt(img, n, 'in') --disable delayed loader
			end
			table.insert(texturesArchive, img)
		else
			a, b, c = line:match('^([%w ]+) (%d+)(p[tx]) *([a-z]*)$')
			if a then
				if c == 'px' then b = math.ceil(b * 72 / 96) end
				if font then bytes = bytes + font.mem end
				font = ffi.new('Font')
				ffi.fill(font, ffi.sizeof('Font'))
				font.image = img
				font._scale = 1
				font._interval = 1
				font.scalable = scalable == true or false
				if not C.fonts[a] then C.fonts[a] = {} end
				C.fonts[a][tonumber(b)] = font
			else
				libcheetah.fontSetGlyph(font, line)
				glyphs = glyphs + 1
			end
		end
	end
	if font then bytes = bytes + font.mem end
	print('Loaded font '..name..' ('..glyphs..' glyphs, '..bytes..' bytes) in '..(C.getTicks()-millis)..' ms')
end

C.getFont = function(name, size)
	return C.fonts[name][size]
end

ffi.metatype('Font', {
	__index = {
		print = function(font, text, x, y, width, align)
			libcheetah.fontPrintf(font, text or 'undefined', x or 0, y or 0, width or 0, align or 0)
		end,
		interval = libcheetah.fontInterval,
		scale = libcheetah.fontScale,
		getInterval = libcheetah.fontGetInterval,
		getScale = libcheetah.fontGetScale
	},
	--~ __gc = libcheetah.deleteFont
})

--============================================================================--
--                                  RESOURCES                                 --
--============================================================================--

local resLoadImageCallback = function (path, t, dir, name, ext)
	local buf = dir..'/'..name
	if not C.fileExists(buf..'.fnt') and not C.fileExists(buf..'.atlas') then
		if t[name] then
			libcheetah.myError('resLoader: resourse %s already exists (replaced with %s)', name, n)
		end
		buf = tonumber(name)
		if tostring(buf) == name then
			name = buf
		end
		t[name] = C.newImage(path)
	end
end

local resLoadAtlasCallback = function (path, t, dir, name, ext)
	local img, imgname, x1, y1, x2, y2, cx, cy, w, h, r, p, a, buf
	for line in io.lines(path) do
		imgname = line:match('^textures?: (.+)')
		if imgname then
			img = ffi.new('Image')
			libcheetah.newImageOpt(img, dir..'/'..imgname, '')
			table.insert(texturesArchive, img)
		else
			imgname, x1, y1, x2, y2, cx, cy, w, h, r = 
					line:match('(.+)\t'..string.rep('(%d+)\t', 8)..'(r?)')
			if h then
				p = ffi.new('Atlas')
				p.image = img
				p.x, p.y = tonumber(cx), tonumber(cy)
				p.w, p.h = tonumber(w), tonumber(h)
				p.aw, p.ah = tonumber(x2), tonumber(y2)
				p.tex[0] = tonumber(x1) / img.w
				p.tex[1] = tonumber(y1) / img.h
				p.tex[2] = tonumber(x1) / img.w
				p.tex[3] = (tonumber(y1) + tonumber(y2)) / img.h
				p.tex[4] = (tonumber(x1) + tonumber(x2)) / img.w
				p.tex[5] = (tonumber(y1) + tonumber(y2)) / img.h
				p.tex[6] = (tonumber(x1) + tonumber(x2)) / img.w
				p.tex[7] = tonumber(y1) / img.h
				if r =='r' then
					p.x, p.y = p.y, p.h - p.x - p.aw
					p.aw, p.ah = p.ah, p.aw
					p.tex[0], p.tex[1], p.tex[2], p.tex[3], p.tex[4], p.tex[5], p.tex[6], p.tex[7] = 
					p.tex[6], p.tex[7], p.tex[0], p.tex[1], p.tex[2], p.tex[3], p.tex[4], p.tex[5]
				end
				a = t
				buf = nil
				for v in imgname:gmatch('[^/]+') do
					if buf then
						if not a[buf] then a[buf] = {} end
						a = a[buf]
					end
					buf = tonumber(v)
					if tostring(buf) ~= v then
						buf = v
					end
				end
				imgname = buf:match('^(.*)%.[^.]+$')
				buf = tonumber(imgname)
				if tostring(buf) == imgname then
					imgname = buf
				end
				if a[imgname] then
					libcheetah.myError('resLoader: resourse %s already exists, replacing', imgname)
				end
				a[imgname] = p
			end
		end
	end
end

local _exts = {
	jpg = resLoadImageCallback,
	png = resLoadImageCallback,
	bmp = resLoadImageCallback,
	dds = resLoadImageCallback,
	fnt = C.newFont,
	atlas = resLoadAtlasCallback,
}

C.resLoaderAddCallback = function(exts, callback)
	if type(exts) == 'table' then
		for _, v in ipairs(exts) do
			_exts[v] = callback
		end
	else
		_exts[ext] = callback
	end
end

--recursive resource loader
C.resLoader = function(dirname, recursive)
	local res_tree = {}
	if not dirname or type(dirname) ~= 'string' then
		libcheetah.myError('resLoader: you must specify directory name')
		return
	end
	local dir = libcheetah.openDir(dirname)
	if not libcheetah.isPointer(dir) then
		libcheetah.myError('resLoader: cannot open dir %s', dirname)
		return
	end
	local de = libcheetah.readDir(dir)
	
	local s, ext, n, name, callback
	--~ resLoadedImages = {}
	while libcheetah.isPointer(de) do
		local raw_n = libcheetah.getDirentName(de)
		n = ffi.string(raw_n)
		s = dirname..'/'..n
		if raw_n[0] ~= 46 then -- "."
			if libcheetah.isDir(s) then
				if recursive then
					local n_num = tonumber(n)
					if tostring(n_num) == n then
						res_tree[n_num] = C.resLoader(s)
					else
						res_tree[n] = C.resLoader(s)
					end
				end
			else
				name, ext = n:match('^(.*)%.([^.]+)$')
				if ext then
					ext = ext:lower()
					--~ if not ext then print(name) end
					callback = _exts[ext]
					if callback then
						callback(s, res_tree, dirname, name, ext)
					end
				end
			end
		end
		de = libcheetah.readDir(dir)
	end
	return res_tree
end

C.init = function(title, options)
	libcheetah.init(title or 'Cheetah 2D Engine', options or 'vsync')
	C.newFont('lib/font/DICE.fnt')
	C.fonts.default = C.fonts.DICE[6]
end

--============================================================================--
--                                 FRAMEBUFFER                                --
--============================================================================--

C.newFramebuffer = function(w, h, options)
	local ptr = ffi.new('Framebuffer')
	libcheetah.newFramebufferOpt(ptr, w, h, options or '')
	return ptr
end

ffi.metatype('Framebuffer', {
	__index = {
		draw = function(s, x, y, w, h, a, ox, oy)
			s.image:draw(x, y, w, h, a, ox, oy)
		end,
		drawq = function(s, x, y, w, h, qx, qy, qw, qh, a, ox, oy)
			s.image:drawq(x, y, w, h, qx, qy, qw, qh, a, ox, oy)
		end,
		bind = libcheetah.framebufferBind,
		save = function(s, name)
			libcheetah.framebufferSaveBMP(s, name)
		end,
		check = libcheetah.framebufferCheck,
		unbind = libcheetah.framebufferUnbind
	},
	__gc = libcheetah.deleteFramebuffer
})

--============================================================================--
--                                  SHADERS                                   --
--============================================================================--

local uniforms = {}
C.newShader = function(fragment, vertex)
	local str
	local shader = ffi.new('Shader')
	if C.fileExists(fragment) then
		fragment = C.getFile(fragment)
	end
	if vertex then
		if C.fileExists(vertex) then
			vertex = C.getFile(vertex)
		end
		libcheetah.newFragmentVertexShader(shader, fragment, vertex)
		str = vertex .. fragment
	else
		libcheetah.newFragmentShader(shader, fragment)
		str = fragment
	end
	local location, float
	if shader:check() then
		uniforms[shader.id] = {}
		for a, b in string.gmatch(str, "uniform[ \t]+([^ ]+)[ \t]+([^ ;]+);") do
			location = libcheetah.GetUniformLocation(shader.id, b)
			if location then
				if a == 'float' or a == 'vec2' or a == 'vec3' or a == 'vec4' then
					float = true
				else
					float = false
				end
				uniforms[shader.id][b] = {location, float}
			end
		end
	end
	return shader
end

ffi.metatype('Shader', {
	__index = {
		bind = libcheetah.shaderBind,
		unbind = libcheetah.shaderUnbind,
		check = libcheetah.shaderCheck,
		--this method allows you to send uniforms to shader without thinking about its types
		--type of uniform is detecting in C.newShader
		set = function(shader, name, a, b, c, d)
			local buf = uniforms[shader.id]
			if buf then
				buf = buf[name]
				if buf and buf[1] >= 0 then
					if d then
						libcheetah.Uniform4f(buf[1], a, b, c, d)
					elseif c then
						libcheetah.Uniform3f(buf[1], a, b, c)
					elseif b then
						libcheetah.Uniform2f(buf[1], a, b)
					elseif a then
						if buf[2] then libcheetah.Uniform1f(buf[1], a) else libcheetah.Uniform1i(buf[1], a) end
					end
				end
			end
		end
	},
	__gc = libcheetah.deleteShader
})

C.newPoints = ffi.typeof("Point[?]")

math.randomseed(os.time())
