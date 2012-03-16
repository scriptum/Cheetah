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

table.serialize = function(t, norecursive, lvl)
	assert(type(t) == "table", "Can only serialize tables.")
	if not lvl then lvl = 1  end
	if not t then return nil end
	local s = "{\n"
	for k, v in pairs(t) do
		if type(k) == "string" then k = k
		elseif type(k) == "number" then k = "["..k.."]"
		else error("Attempted to serialize a table with an invalid key: "..tostring(k))
		end
		if type(v) == "string" then v = "\""..v.."\""			
		elseif type(v) == "table" then if norecursive then v = nil else v = table.serialize(v, norecursive, lvl + 1) end
		elseif type(v) == "boolean" then v = v and "true" or "false"
		elseif type(v) == "userdata" then v = ("%q"):format(tostring(v))
		end
		if v then s = s..string.rep("\t", lvl)..k.."="..v..",\n" end
	end
	return s.."}\n"
end

table.includes = function(list, value)
	for _, x in ipairs(list) do
		if (x == value) then return true end
	end
	return false
end

table.find = function(array, value)
  for k, v in ipairs(array) do
    if v == value then return k end
  end
  return -1
end

--возвращает последний элемент таблицы
table.last = function(t)
	return t[#t]
end

table.copy = function(t)
  local t2 = {}
  for k,v in pairs(t) do
		if type(v) == 'table' then
			t2[k] = table.copy(v)
		else
			t2[k] = v
		end
  end
  return t2
end