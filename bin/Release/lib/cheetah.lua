local ffi = require "ffi"

cheetah = {}

cheetah.getFile = function(filename)
	local file = assert(io.open(filename))
	local contents = file:read '*a'
	file:close()
	return contents
end

cheetah.putFile = function(filename, str, writemode)
	local file = assert(io.open(filename, writemode or "w"))
	file:write(str)
	file:close()
end

cheetah.fileExists = function(name)
	local f = io.open(name, 'r')
	if f then
		f:close()
		return true
	end
	return false
end

cheetah.appendFile = function(filename, str)
	cheetah.putFile(filename, str, "wa")
end

local archToInt = {x86 = 32, x64 = 64}

cheetah.loadDLL = function(filename)
	local res
	print(filename)
	if ffi.os == "Windows" then
		res = ffi.load('./bin/win32/' .. filename .. '.dll')
	elseif ffi.os == "OSX" then
		res = ffi.load('./bin/macosx/' .. filename .. '.dynlib')
	else
		res = ffi.load('./bin/linux'..archToInt[jit.arch]..'/lib' .. filename .. '.so')
	end
	if res then
		local header = 'lib/'..filename..'.h'
		if cheetah.fileExists(header) then
			ffi.cdef(cheetah.getFile(header))
		end
		print('Successfully loaded module: '..filename)
		return res
	else
		print('Error: cannot load module '..filename)
		return nil
	end
end
cheetah.module = cheetah.loadDLL

--stupid win api
if ffi.os == "Windows" then
	ffi.cdef[[struct dirent {
		long		ino;		/* Always zero. */
		unsigned short	reclen;	/* Always zero. */
		unsigned short	namlen;	/* Length of name in name. */
		char		name[256]; /* File name. */
	};]]
else
	ffi.cdef[[struct dirent {
		unsigned int ino;
		unsigned int off;
		unsigned short int reclen;
		unsigned char type;
		char name[256];
	};]]
end

ffi.cdef 'void printf(const char * str, ...);'

local libcheetah = cheetah.loadDLL 'cheetah'

assert(libcheetah, 'Cannot load cheetah library!')

--~ if not libcheetah.initSDL() then print 'Cannot init SDL!' end
--~ libcheetah.init('Cheetah engine', 640, 480, 32, '')

require "lib.gl"
--~ 
--~ local gl = GL

local lua_keys = require 'lib.keys'
local button_names = {"l","m","r","wu","wd"}
local lua_events = {"q","kp","kr","mp","mr","rz"}

local done = 0
local FPS = 60
cheetah.FPS = "60"
cheetah.printFPS = false
local time = 0
local lasttime = 0

cheetah.mainLoop = function()
	while done == 0 do
		time = libcheetah.getTime()
		libcheetah.doAutoScale()
		if cheetah.render then cheetah.render() end
		libcheetah.swapBuffers()
		FPS = (FPS + 1) / (1 + (libcheetah.getTime() - time));
		if time - lasttime > 0.5 then
			lasttime = time
			if cheetah.printFPS then print(FPS) end
			cheetah.FPS = tostring(math.floor(FPS))
		end
	end
end

--first time when you try to load an image this function autoloads SOIL library
cheetah.newImage = function(name, opt)
	require 'lib.SOIL'
	return cheetah.newImage(name, opt)
end

cheetah.setColor = function(r,g,b,a)
	if type(r) == 'table' then
		libcheetah.Color(r[1] or 255, r[2] or 255, r[3] or 255, r[4] or 255)
	else
		libcheetah.Color(r or 255, g or 255, b or 255, a or 255)
	end
end

cheetah.getWindowSize = function()
	return libcheetah.getWindowWidth(), libcheetah.getWindowHeight()
end

cheetah.poll = function()
	local e = lua_events[libcheetah.getEventType()]
	local a, b, c
	if e == 'q' then
		done = 1
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

cheetah.getMousePos = function()
	return libcheetah.getMouseX(), libcheetah.getMouseY()
end

cheetah.getFps = function()
	return FPS
end

local uniforms = {}
cheetah.newShader = function(ver, frag)
	local shader, str
	if frag then
		shader = libcheetah.newVertexFragmentShader(ver, frag)
		str = ver .. frag
	else
		shader = libcheetah.newFragmentShader(ver)
		str = ver
	end
	local location, float
	if shader then
		uniforms[shader.id] = {}
		for a, b in string.gmatch(str, "uniform[ \t]+([%a%d]+)[ \t]+([%a%d]+);") do
			location = libcheetah.GetUniformLocation(shader.id, b)
			--~ print(b, location)
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

cheetah.drawMultitexture = function(...)
	if #arg < 2 then
		libcheetah.myError 'drawMultitexture: pass at least 2 images.'
	end
	libcheetah.enableTexture2D()
	for i, v in ipairs(arg) do
		libcheetah.activeTexture(i-1)
		libcheetah.bindTexture(v.id)
	end
	libcheetah.activeTexture(0)
	libcheetah.rectangle(true)
	libcheetah.disableTexture2D()
end

cheetah.fileExt = function(name)
	return name:gsub('^.*%.', '')
end

cheetah.fileName = function(name)
	return name:gsub('%..*', '')
end

local _exts = {
	image = {
		jpg = true,
		png = true,
		bmp = true,
		dds = true,
		JPG = true,
		PNG = true,
		BMP = true,
		DDS = true
	}
}

--recursive resource loader
cheetah.resLoader = function(dirname, recursive)
	local t = {}
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
	local s, ext, n, name
	while libcheetah.isPointer(de) do
		n = ffi.string(de.name)
		s = dirname..'/'..n
		if de.name[0] ~= 46 then
			--~ print(s)
			if libcheetah.isDir(s) and recursive then
				t[n] = cheetah.resLoader(s)
			else
				ext = s:gsub('^.*%.', '')
				if _exts.image[ext] then
					name = n:gsub('%..*$','')
					if t[name] then
						libcheetah.myError('resLoader: resourse %s already exists (replaced by with %s)', name, n)
					end
					t[name] = cheetah.newImage(s)
				end
			end
		end
		de = libcheetah.readDir(dir)
	end
	return t
end

--exec func for each file in directory
cheetah.fileEach = function(dirname, func)
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
		func(ffi.string(de.name))
		de = libcheetah.readDir(dir)
	end
	return t
end

setmetatable(cheetah, { __index = libcheetah})

ffi.metatype('Image', {
	__index = {
		draw = libcheetah.imageDraw,
		drawq = libcheetah.imageDrawq,
	}, 
	__gc = libcheetah.deleteImage
})

ffi.metatype('Font', {
	__index = {
		setGlyph = libcheetah.fontSetGlyph, 
		select = libcheetah.fontSelect,
		print = function(font, text, width, align)
			font:select()
			libcheetah.fontPrint(text, width or 10000, align or 1)
		end,
		printf = function(font, text, width, align)
			font:select()
			libcheetah.fontPrintf(text, width, align or 1)
		end
	},
	__gc = libcheetah.deleteFont
})

ffi.metatype('Framebuffer', {
	__index = {
		draw = libcheetah.framebufferDraw,
		drawq = libcheetah.framebufferDrawq,
		bind = libcheetah.framebufferBind,
		unbind = function()libcheetah.framebufferUnbind()end
	},
	__gc = libcheetah.deleteFramebuffer
})

ffi.metatype('Shader', {
	__index = {
		bind = libcheetah.useShader,
		unbind = function()libcheetah.disableShader()end,
		--this method allows you to send uniforms to shader without thinking about its types
		--type of uniform is detecting in cheetah.newShader
		set = function(shader, name, a, b, c, d)
			local buf = uniforms[shader.id][name]
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
	},
	__gc = libcheetah.deleteShader
})
