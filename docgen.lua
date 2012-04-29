#!/usr/bin/lua

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

local e = function(cmd, raw)
  local f = assert(io.popen(cmd, 'r'))
  local s = assert(f:read('*a'))
  f:close()
  return s
end

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

local res = e 'find -L src -name "*.c" -or -name "*.lua"'

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

for i in res:gmatch '[^\n]+' do
	local c = getFile(i)
	local desc, group, gr, sgr, ret, see, fname, fnametrim, farg, text
	for doc, func in c:gmatch '/%*%*(.-) %* %*/%s*([%d%a* _.]+%([%d%a,* _]*%))' do
		doc = doc:gsub('\n %* ', '\n')
		desc = doc:kwmatch('descr')
		assert(desc, 'No @descr secton for founction\n\t'..func..'\n')
		group = doc:kwmatch('group')
		assert(group, 'No @group secton for founction\n\t'..func..'\n')
		gr, sgr = group:match '([^%s]+)%s*/%s*([^%s]+)'
		if not gr then
			gr, sgr = group, 'nogroup'
		end
		ret = doc:kwmatch('return')
		see = doc:kwmatch('see')
		local vars, varnames = {}, {}
		for k in doc:kwgmatch 'var' do
			table.insert(vars, k:match '^%s*(.-)%s*$')
		end
		fname, farg = func:match '%s*([%d%a* _.]-)%s*%(([%d%a,* _]*)%)'
		fname = fname:gsub('%s*=%s*function%s*$', '')
		fnametrim = fname:match '([%d%a_.]+)%s*$'
		for k in farg:gmatch '[^,]+' do
			table.insert(varnames, k:match'([%d%a_]+)%s*$')
		end
		assert(#varnames == #vars, 'Error in vars count for function\n\t'..func..'\n')
		text = 
		'### cheetah.'..fnametrim ..
		' <a name="'..fnametrim..'"></a>\n\n' ..
		'`cheetah.'..fnametrim..'('..table.concat(varnames, ', ')..')`\n\n' ..
		desc
		if #varnames > 0 then
			text = text .. '\n\n**Parameters**\n'
			for i = 1, #varnames do
				text = text .. '\n* **'..varnames[i]..'** <br />\n'..vars[i]
			end
		end
		if ret then
			text = text .. '\n\n**Return value**\n\n'..ret
		end
		if see then
			text = text .. '\n\n**See also**\n\n'..see:gsub(' ?([^, ]+)', '* <a href="#wiki-%1">%1</a>\n')
		end
		if not docum[gr] then docum[gr] = {} end
		if not docum[gr][sgr] then docum[gr][sgr] = {} end
		table.insert(docum[gr][sgr], text)
	end
end

local docfiles = {}

for k, v in pairs(docum) do
	local text = ''
	for kk, vv in pairs(v) do
		text = text .. '## '..kk:gsub("^%l", string.upper)..
		' <a name="'..kk:gsub("^%l", string.upper)..'"></a>'..
		'\n\n'..table.concat(vv, '\n\n***\n\n')..'\n\n'
	end
	putFile('../Cheetah.wiki/'..k:gsub("^%l", string.upper)..'.md', text)
end
