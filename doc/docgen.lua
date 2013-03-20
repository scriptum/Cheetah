#!/usr/bin/lua

local meta = getmetatable("") -- get the string metatable

function string.split(str, pat)
  local t = {}
  local fpat = "(.-)" .. pat
  local last_end = 1
  local s, e, cap = str:find(fpat, 1)
  while s do
    if s ~= 1 or cap ~= "" then
  table.insert(t,cap)
    end
    last_end = e+1
    s, e, cap = str:find(fpat, last_end)
  end
  if last_end <= #str then
    cap = str:sub(last_end)
    table.insert(t, cap)
  end
  return t
end

meta.__div = string.split -- the / operator

table.print = function(tt, indent, done)
  done = done or {}
  indent = indent or 0
  if type(tt) == "table" then
    for key, value in pairs (tt) do
      io.write(string.rep (" ", indent)) -- indent it
      if type (value) == "table" and not done [value] then
        done [value] = true
        io.write(string.format("[%s] => table\n", tostring (key)));
        io.write(string.rep (" ", indent+4)) -- indent it
        io.write("(\n");
        table.print (value, indent + 7, done)
        io.write(string.rep (" ", indent+4)) -- indent it
        io.write(")\n");
      else
        io.write(string.format("[%s] => %s\n",
            tostring (key), tostring(value)))
      end
    end
  else
    io.write(tt .. "\n")
  end
end

--~ local e = function(cmd, raw)
  --~ local f = assert(io.popen(cmd, 'r'))
  --~ local s = assert(f:read('*a'))
  --~ f:close()
  --~ return s
--~ end

local getFile = function(filename)
	local file = assert(io.open(filename))
	local contents = file:read '*a'
	file:close()
	return contents
end

local putFile = function(filename, str, writemode)
	local file = assert(io.open(filename, writemode or "w"))
	file:write(str)
	file:close()
end

--~ local res = e 'find -L src -name "*.c" -or -name "*.lua"'

local docum = {}

local function kw(a)
	return '@'..a..'%s+([^@]+)'
end
string.kwmatch = function(s, a)
	local r = s:match(kw(a))
	if r then
		return string.match(r, '^%s*(.-)%s*$')
	end
end
string.kwgmatch = function(s, a)
	return s:gmatch(kw(a))
end

--~ local docfiles = {}
--~ 
--~ for k, v in pairs(docum) do
	--~ local text = ''
	--~ for kk, vv in pairs(v) do
		--~ text = text .. '## '..kk:gsub("^%l", string.upper)..
		--~ ' <a name="'..kk:gsub("^%l", string.upper)..'"></a>'..
		--~ '\n\n'..table.concat(vv, '\n\n***\n\n')..'\n\n'
	--~ end
	--~ putFile('../Cheetah.wiki/'..k:gsub("^%l", string.upper)..'.md', text)
--~ end

local description, vars, group, seealso, class, advanced, varreturn, constructor, examples, notes
local lasttable
local function clear_vars()
	description = {}
	vars = {}
	examples = {}
	notes = {}
	varreturn = {}
	group = nil
	seealso = nil
	class = nil
	advanced = false
	constructor = false
	lasttable = nil
end
clear_vars()

local doc_classes = {} --functions by classes
local doc_functions = {} --functions by groups
docKeyWords = {} --to index missing keywords

local function add_func(funcname, func)
	--~ table.print({funcname, description, vars, group, seealso, examples, class, advanced, varreturn, constructor})
	--~ print('--------------------------------')
	docKeyWords[funcname] = true
	if class then
		for cls in class:gmatch('[a-zA-Z]+') do
			--add new class (if not exists)
			if not doc_classes[cls] then doc_classes[cls] = {} end
			table.insert(doc_classes[cls], {funcname = funcname, func = func, description = description, vars = vars, group = group, seealso = seealso, examples = examples, class = cls, advanced = advanced, varreturn = varreturn, constructor = constructor})
		end
	elseif group then
		if not doc_functions[group] then doc_functions[group] = {} end
		table.insert(doc_functions[group], {funcname = funcname, func = func, description = description, vars = vars, group = group, seealso = seealso, examples = examples, class = class, advanced = advanced, varreturn = varreturn, constructor = constructor})
	else
		io.write(io.stderr, 'Function'..funcname..' is without class or group!')
	end
	clear_vars()
end

--parser
for line in io.lines('cheetah.doc') do
	local line1 = line:sub(0,1)
	if line:sub(0,2) ~= '--' then
		if line:sub(0,7) == '@descr ' then
			table.insert(description, line:sub(8))
			lasttable = description
		elseif line:sub(0,7) == '@group ' then
			group = line:sub(8)
			lasttable = nil
		elseif line:sub(0,5) == '@see ' then
			seealso = line:sub(6)
			lasttable = nil
		elseif line:sub(0,7) == '@class ' then
			class = line:sub(8)
			lasttable = nil
		elseif line:sub(0,9) == '@advanced' then
			advanced = true
			lasttable = nil
		elseif line:sub(0,12) == '@constructor' then
			constructor = true
			lasttable = nil
		elseif line:sub(0,8) == '@return ' then
			table.insert(varreturn, line:sub(9))
			lasttable = varreturn
		elseif line:sub(0,5) == '@var ' then
			local vartable = {line:sub(6)}
			table.insert(vars, vartable)
			lasttable = vartable
		elseif line:sub(0,6) == '@note ' then
			local notetable = {line:sub(7)}
			table.insert(notes, notetable)
			lasttable = notetable
		elseif line:sub(0,9) == '@example ' then
			local exampletable = {line:sub(10)}
			table.insert(examples, exampletable)
			lasttable = exampletable
		else
			if line1 == '@' then
				lasttable = nil
			else
				funcname = line:match '^[^ ]* ?function ([^(]+)%(.*$'
				if funcname then
					add_func(funcname, line)
				elseif lasttable then
					--append another line for multiline descriptions
					table.insert(lasttable, line)
				end
			end
		end
	end
end
local function subst_class_general(str)
	return str:gsub('%%class%(([^)]+)%)', '<a href="#wiki-%1">%1</a>')
end
local function subst_class(str, class)
	return str:gsub('%%class', '<a href="#wiki-'..class..'">'..class..'</a>')
end
local function subst_method(str)
	local ret = str:gsub('([a-zA-Z0-9_]+)</a>:([a-zA-Z0-9_]+)%(', '%1</a>:<a href="#wiki-%1-%2">%2</a>(')
	return ret
end

local function process_description(descr, class)
	local ret = subst_class_general(descr)
	if class then
		ret = subst_class(ret, class)
	end
	return subst_method(ret)
end

local function format_doc(t)
	local func
	if t.class and not t.constructor then
		func = t.func:gsub('function ', ':')
		print('<a name="'..t.class..'-'..t.funcname..'"></a>\n#### <font color="gray">'..t.class..':</font>'..t.funcname..'\n')
	else
		local constructor = ''
		if t.constructor then constructor = ' <font color="gray">(constructor)</font>' end
		print('<a name="'..t.funcname..'"></a>\n#### cheetah.'..t.funcname..constructor..'\n')
		func = t.func:gsub('function ', 'cheetah.')
	end
	local funcname, args = func:match('^([^%(]+)%((.*)%)$')
	arg_split = args / ',%s+'
	if #arg_split > 2 or #args > 30 then
		func = funcname.."(\n\t"..table.concat(arg_split, ",\n\t").."\n)"
	end
	-- table.print(arg_split)
	-- print(funcname, args)
	print('<pre>'..process_description(func, t.class)..'</pre>\n')
	print(process_description(table.concat(t.description, '\n'), t.class))
	
	if #t.vars > 0 then
		print '\n\n**Parameters**\n'
	end
	for _, v in ipairs(t.vars) do
		print('* '..process_description(table.concat(v, '\n'), t.class))
		--text = text..'\n* **'..table.concat(v, '\n') --..'** <br />\n'..vars[i]
	end
	print()
end

print('## Classes')
for cls, functions in pairs(doc_classes) do
	print('\n<a name="'..cls..'"></a>\n### '..cls..'\n')
	for _, value in ipairs(functions) do
		format_doc(value)
	end
end

_blackList = [[
newVbo
vboDraw
newVboPoints
vboDrawSprites
newVboPoints3
vboDrawSprites3
deleteVbo
prepare
atlasDrawt
atlasDrawxy
myError
framebufferCheck
framebufferBind
framebufferUnbind
framebufferSaveBMP
imageDrawxy
imageDrawt
imageDrawqxy
imageDrawqt
borderImageDrawt
borderImageDrawxy
initMultitexture
deleteMultitexture
multitextureDrawxy
multitextureDrawt
multitextureDrawqxy
multitextureDrawqt
deleteImage
newParticleSystem
particleSystemUpdate
particleSystemDraw
deleteParticleSystem
newFragmentVertexShader
newFragmentShader
shaderCheck
deleteShader
shaderBind
shaderUnbind
GetUniformLocation
Uniform1i
Uniform1f
Uniform2f
Uniform3f
Uniform4f
deleteFont
newFramebufferOpt
deleteFramebuffer
generateImageData
generateImage
newImageOpt
newImageRaw
imageBind
fontWidth
fontHeight
fontPrintf
fontScale
fontInterval
fontGetScale
fontGetInterval
fontSetGlyph
fontSetKerning
newTilmapInternal
tilemapDraw
deleteTilemap
getModesSDL
loadfile
stencilFunc
stencilOp
drawToStencil
drawUsingStencil
enableTexture
disableTexture
]]
blackList = {}
for line in _blackList:gmatch("([^%s]+)") do
	blackList[line] = true
end

local missingKeysFirstTime = true

for line in io.lines '../bin/Release/lib/cheetah.h' do
	local key = line:match('([A-Za-z0-9_]+)%(')
	if key and not blackList[key] and not docKeyWords[key] then
		if missingKeysFirstTime then
			io.stderr:write('WARNING! There is missing functions:\n\n')
			missingKeysFirstTime = false
		end
		io.stderr:write(key, '\n')
	end
end
