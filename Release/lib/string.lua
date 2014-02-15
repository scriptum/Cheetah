local meta = getmetatable("") -- get the string metatable

meta.__add = function(a, b) -- the + operator
	return a..b
end

meta.__sub = function(a, b) -- the - operator
	return a:gsub(b,"")
end

meta.__mul = function(a, b) -- the * operator
	return a:rep(b)
end

function string.split(str, pat)
	local t = {}
	local fpat = "(.-)" .. pat
	local last_end = 1
	local s, e, cap = str:find(fpat, 1)
	while s do
		if s ~= 1 or cap ~= "" then
			table.insert(t,cap)
		end
		last_end = e + 1
		s, e, cap = str:find(fpat, last_end)
	end
	if last_end <= #str then
		cap = str:sub(last_end)
		table.insert(t, cap)
	end
	return t
end

meta.__div = function(a, b) -- the / operator
	return a:split(b)
end

--split with empty parts
function string.split2(str, pat)
	local t = {}
	local function helper(line) table.insert(t, line) return "" end
	helper((str:gsub("(.-)"..pat, helper)))
	return t
end

function string.trim(s)
	return s:gsub("^%s+", ""):gsub("%s+$", "")
end