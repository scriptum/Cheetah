ffi = require("ffi")

cheetah = {}

cheetah.getFile = function(filename)
	local file = assert(io.open(filename))
	local contents = file:read'*a'
	file:close()
	return contents
end

cheetah.loadDLL = function(filename)
	if ffi.os == "Windows" then
		return ffi.load('./' .. filename .. '.dll')
	if ffi.os == "OSX" then
		return ffi.load('./' .. filename .. '.dynlib')
	else
		return ffi.load('./' .. filename .. '.so')
	end
end

ffi.cdef(cheetah.getFile('cheetah.h'))

local libcheetah = cheetah.loadDLL('libcheetah')

local lua_keys = require 'keys'
local button_names = {"l","m","r","wu","wd"}
local lua_events = {"q","kp","kr","mp","mr","rz"}

local done = 0
local FPS = 60
local time = 0
local lasttime = 0

cheetah.mainLoop = function()
	while done == 0 do
		time = cheetah.getTime()
		if cheetah.render then cheetah.render() end
		cheetah.swapBuffers()
		FPS = (FPS + 1) / (1 + (cheetah.getTime() - time));
		if time - lasttime > 1 then
			lasttime = time
			print(FPS)
		end
	end
end

cheetah.setColor = function(r,g,b,a)
	if type(r) == 'table' then
		cheetah.Color(r[1] or 255, r[2] or 255, r[3] or 255, r[4] or 255)
	else
		cheetah.Color(r or 255, g or 255, b or 255, a or 255)
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
		a, b = lua_keys[libcheetah.getEventKey()], libcheetah.getEventKeyUnicode()
	elseif e == 'mp' or e == 'mr' then
		a, b, c = libcheetah.getEventMouseX(), libcheetah.getEventMouseY(), button_names[libcheetah.getEventMouseButton()]
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

cheetah.newShader = function(str)
	local shader = libcheetah.newPixelShader(str)
	local location, float
	if shader then
		uniforms[shader.id] = {}
		for a, b in string.gmatch(str, "uniform[ \t]+([%a%d]+)[ \t]+([%a%d]+);") do
			location = libcheetah.GetUniformLocation(shader.id, b)
			print(b, location)
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
		select = libcheetah.fontSelect
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
		--this method allows you send uniforms to shader without thinking about its types
		--type of uniform is detecting in cheetah.newShader
		set = function(shader, name, a, b, c, d)
			local buf = uniforms[shader.id][name]
			if buf[1] >= 0 then
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