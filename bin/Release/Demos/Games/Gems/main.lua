require 'lib.cheetah'
require 'lib.lquery.init'
local C = cheetah
C.init('Gems game')

local img = {gems = {}}
for i = 0, 7 do 
	table.insert(img.gems, C.newImage("data/gem_" .. i .. ".png"))
end
img.sel = C.newImage("data/gem_9.png")

local field = {} --references to gems entities

local gemSize = 74 --px
local maxGems = 4 --max displayed gems
 --internal vars for mouse
local gemPress = false
local gemx, gemy, gement, gemsel
local queue = {} --gems queue (new gems falls down instead removed)

local fall_callback = function(s)
	s.lock = false
end
local fall_gems = function()
	local row, delta, buf, arr
	local speed = 0.2
	local maxspeed = 0
	local q = queue
	queue = {}
	for i = 1, 8 do
		row = 8
		delta = 0
		arr = {}
		for j = 1, 8 do
			e = field[9-j][i]
			if e.t == false then
				table.insert(arr, e)
				if 9-j ~= row then
					e.lock = true --lock gem and fall down
					e:animate({y = (row - 1) * gemSize},{
						cb = fall_callback,
						easing = "linear",
						speed = delta * speed
					})
				end
				row = row - 1
			else
				delta = delta + 1
			end
		end
		if q[i] then
			for _, e in pairs(q[i]) do
					table.insert(arr, e)
					e.t = false
					e:animate({y = (row - 1 ) * gemSize},{
						cb = fall_callback,
						easing = "linear", --default is swing, looks ugly
						speed = #q[i] * speed
					})
					row = row - 1
			end
			if #q[i] > delta then delta = #q[i] end
		end
		for k, v in pairs(arr) do
			v.i = 9 - k
			field[9-k][i] = v
		end
		maxspeed = math.max(maxspeed,  delta)
	end
	if maxspeed > 0 then
	gems:delay({speed = maxspeed * speed + 0.1, cb = calculate_matches})
	end
end

local remove_callback = function(e)
	e.y = -e.row * gemSize
	--e.x = e.x - gemSize/2
	--e.w, e.h = gemSize, gemSize
	e.a = 255
	if e.sel == true then --remove selection
		e.sel = false
		gemsel = nil
	end
	math.randomseed(os.time() + math.random(0, 99999999))
	e.k = math.random(1,maxGems)
end
local check_row = function(i, j, vertical, c)
	if c > 1 then
		for l = 1, c + 1 do
			if vertical == 1 then
				field[j-l][i].m = true
			else
				field[i][j-l].m = true
			end
		end
	end
end
local get_field = function(i, j, num)
	if num == 0 then return field[i][j] else return field[j][i] end
end
calculate_matches = function(checkonly, i1, j1, i2, j2)
	local e
	local k = 0
	local c = 0
	local maxspeed = 0.1
	local speed = maxspeed
	--find and mark matches
	for i = 1, 8 do 
		for l = 0, 1 do
			k, c = 0, 0
			for j = 1, 8 do 
				e = get_field(i, j, l)
				if e.lock == false then
					if k == e.k then
						c = c + 1
					else 
						check_row(i, j, l, c)
						c = 0
					end
					k = e.k
				else
					check_row(i, j, l, c)
					k, c = 0, 0
				end
			end
			check_row(i, 9, l, c)
		end
	end
	--remove matches
	local res = false
	for i = 1, 8 do 
		for j = 1, 8 do
			e = field[9-i][j]
			if e.m == true then 
				if checkonly and checkonly == true and
					(e.i == i1 and j == j1 or e.i == i2 and j == j2) or 
					type(checkonly) ~= 'boolean' then res = true end
				e.m = false
				if type(checkonly) ~= 'boolean' then
					e.lock = true
					if not queue[e.j] then queue[e.j] = {} end
					table.insert(queue[e.j], e)
					e.row = #queue[e.j]
					e.t = true
					e:animate({a = 0},{cb = remove_callback, speed = 0.3})
					speed = 0.3 + maxspeed
				end
			end
		end
	end
	gems:stop():delay({speed = speed, cb = fall_gems})
	return res
end
--swap gems
local swap = function(i1, j1, i2, j2, v, o)
	if field[i2][j2].lock == false and field[i1][j1].lock == false then
		if gemsel then gemsel.sel = false end
		gemsel = nil
		local s1 = field[i1][j1] --just alias
		local s2 = field[i2][j2]
		local t1, t2 = {}, {} --animation keys
		t1[v] = s1[v] + o
		t2[v] = s1[v]
		--swap gems and try to find matches
		s1.i, s1.j = i2, j2
		s2.i, s2.j = i1, j1
		field[i1][j1] = s2
		field[i2][j2] = s1
		if calculate_matches(true, i1, j1, i2, j2) == false then --if no matches - wrong move
			gems:stop()
			--swap back (that was wrong move)
			s1.i, s1.j = i1, j1
			s2.i, s2.j = i2, j2
			field[i1][j1] = s1
			field[i2][j2] = s2
			s1.lock = true --lock gems while animated
			s2.lock = true
			s1:animate(t1):animate(t2)
			s2:animate(t2):animate(t1, {cb = function()
				s1.lock = false --unlock gems
				s2.lock = false
				calculate_matches()
			end})
		else
			gems:stop()
			s1.lock = true --lock gems while animated
			s2.lock = true
			s1:animate(t1)
			s2:animate(t2, {cb = function()
				s1.lock = false --unlock gems
				s2.lock = false
				calculate_matches()
			end})
		end
	end	
end
local gem_press = function(s, x, y)
	if s.lock then return end
	gement = s
	gemPress = true
	gemx = x
	gemy = y
end
local gem_release = function(s)
	if s.lock then return end
	gemPress = false
	if math.abs(lQuery.mX-gemx) < 10 and math.abs(lQuery.mY-gemy) < 10 then
		if gemsel then
			if gemsel.i == s.i and gemsel.j == s.j - 1 then
				swap(gemsel.i, gemsel.j, s.i, s.j, 'x', gemSize)
			elseif gemsel.i == s.i and gemsel.j == s.j + 1 then
				swap(gemsel.i, gemsel.j, s.i, s.j, 'x', -gemSize)
			elseif gemsel.i == s.i - 1 and gemsel.j == s.j then
				swap(gemsel.i, gemsel.j, s.i, s.j, 'y', gemSize)
			elseif gemsel.i == s.i + 1 and gemsel.j == s.j then
				swap(gemsel.i, gemsel.j, s.i, s.j, 'y', -gemSize)
			elseif gemsel == s then
				s.sel = false
				gemsel = nil
			else
				gemsel.sel = false
				s.sel = true
				gemsel = s
			end
		else
			s.sel = true
			gemsel = s
		end
	end
end
--mouse gesture handler
lQuery.addhook(function()
	if lQuery.MousePressed == true and gemPress == true and gement.lock == false then
		local dx, dy = gemx - lQuery.mX, gemy - lQuery.mY
		local i, j = gement.i, gement.j
		if dx < -10 then
			if gement.j < 8 then
				swap(i, j, i, j+1, 'x', gemSize)
			end
			gemPress = false
		elseif dx > 10 then
			if gement.j > 1 then
				swap(i, j, i, j-1, 'x', -gemSize)
			end
			gemPress = false
		elseif dy < -10 then
			if gement.i < 8 then 
				swap(i, j, i+1, j, 'y', gemSize)
			end
			gemPress = false
		elseif dy > 10 then
			if gement.i > 1 then
				swap(i, j, i-1, j, 'y', -gemSize)
			end
			gemPress = false
		end
	end
end)

local gem_draw = function(s)
	img.gems[s.k]:draw(s.x, s.y, gemSize,  gemSize)
	if s.sel == true then img.sel:draw(s.x, s.y, gemSize,  gemSize) end
end

math.randomseed(os.time())
--create gems
gems = E:new(screen)
for i = 1, 8 do
	field[i] = {}
	for j = 1, 8 do 
		field[i][j] = E:new(gems)
		:move(gemSize * j - gemSize + 4, gemSize * i - gemSize)
		:draw(gem_draw)
		:set({i=i,j=j,
			k = math.random(1,maxGems), --gem number
			lock = false, --can't move
			m = false, --mark (to remove)
			t = false, --gem in queue
			sel = false, --gem selected by mouse
			a = 255 -- alpha
		})
		:color(255,255,255,255)
		:size(gemSize,gemSize)
		:mousepress(gem_press)
		:mouserelease(gem_release)
	end
end
calculate_matches()

C.mainLoop()