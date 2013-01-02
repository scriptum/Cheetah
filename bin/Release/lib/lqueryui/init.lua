local ui = {}

local C = cheetah

ui.default = {
	color = {255,255,255},
	borderColor = {255,255,255},
	--~ font = Fonts["Arial"][9]
}

ui.setColor = function(color)
	if #color == 3 then
		C.color(unpack(color), 255)
	elseif #color == 4 then
		C.color(unpack(color))
	end
end

ui.defaultStyles = {}
ui.setDefaultStyle = function(widgetType, style)
	ui.defaultStyles[widgetType] = style
end

local setStyle = function(entity, style)
	entity.uiStyle = style
end

ui.loadStyle = function(name)
	return require ('lib.lqueryui.styles.' .. name .. '.style')
end

local arrayCopy = function(to, from)
	for k, v in pairs(from) do to[k] = v end
end

local mT, mR, mB, mL
local parseMargin = function(m)
	if type(m) == 'table' then
		if #m == 4 then
			mT, mR, mB, mL = m[1], m[2], m[3], m[4]
		elseif #m == 2 then
			mT, mR, mB, mL = m[1], m[2], m[1], m[2]
		end
	else
		mT, mR, mB, mL = m, m, m, m
	end
end
local style = {}, hover, click
local x,y,w,h
local possibleCSS = {'margin', 'background', 'border', 'borderColor', 'color', 'font', 'backgroundImage', 'borderImage', 'align', 'valign'}
ui.draw = function(s)
	local x, y, w, h
	if s.uiStyle then
		--~ if love then G.setLineStyle( "rough" ) else G.setSmooth(false) end
		local x, y, w, h = s.x, s.y, s.w, s.h
		local st = s.uiStyle
		for _, v in ipairs(possibleCSS) do
			style[v] = st[v]
		end
		if lQuery.hover == s and st.hover then
			arrayCopy(style, st.hover)
		end
		if lQuery.MousePressedOwner == s and st.click then
			arrayCopy(style, st.click)
		end
		if style.margin then
			parseMargin(style.margin)
			x, y, w, h = x + mL, y + mT, w - mR - mL, h - mT - mB
		end
		if style.background then
			if type(style.background) == 'table' then
				ui.setColor(style.background)
			end
		end
		if style.backgroundImage then
			if style.borderImage then
				parseMargin(style.borderImage)
				if love and not s._image then
					s:border_image(style.backgroundImage, mT, mR, mB, mL) 
					s._draw = ui.draw
				end
				s._image = style.backgroundImage
				if not s.top or love then
					s.orig_w = s._image:getWidth()
					s.orig_h = s._image:getHeight()
					s.top = mT
					s.left = mL
					s.bottom = mB
					s.right = mR
				end
				lQuery.border_image_draw(s)
			else
				if love then G.draw(style.backgroundImage, s.x, s.y) else
					style.backgroundImage:draw(s)
				end
			end
		end
		if style.border then
			if style.borderColor then
				ui.setColor(style.borderColor)
			else
				ui.setColor(ui.default.borderColor)
			end
			G.setLineWidth(style.border+0.4999)
			if love then G.rectangle("line",x,y,w,h) else G.rectangle(x,y,w,h) end
		end
		if style.color then 
			ui.setColor(style.color)
		else
			ui.setColor(ui.default.color)
		end
		if style.font then
			if love then G.setFont(style.font) else style.font:select() end
		--~ else
			--~ ui.default.font:select()
		end
		if s._text then
			local pos
			if style.valign then
				if style.valign == 'top' then
					pos = y
				elseif style.valign == 'bottom' then
					if love then pos = y + h - style.font:getLineHeight() else pos = y + h - G.stringHeight()end
				end
			end
			style.font:print(s._text, x, pos or y + (h - style.font:getLineHeight())/2*0.6), w, style.align or 'center')
		end
	end
end

--local emptyChild = {}
local layoutUpdate = function(s)
	--table.print(s)
	if not s._child then return end
	local spacing = 0 --px space between widgets
	local padding = 0
	if s.uiStyle then 
		if s.uiStyle.padding then padding = s.uiStyle.padding end
		if s.uiStyle.spacing then spacing = s.uiStyle.spacing end
	end
	parseMargin(padding)
	local posX, pos2X, sizeX, size2X, minHeight, maxHeight, minWidth, maxWidth
	if s.layoutStyle == "vertical" then
		posX, pos2X, sizeX, size2X, minHeight, maxHeight, minWidth, maxWidth = "y", "x", "h", "w", "minHeight", "maxHeight", "minWIdth", "maxWidth"
	else
		posX, pos2X, sizeX, size2X, minHeight, maxHeight, minWidth, maxWidth = "x", "y", "w", "h", "minWIdth", "maxWidth", "minHeight", "maxHeight"
		mT, mR, mB, mL = mL, mB, mR, mT
	end
	local child = s._child --or emptyChild
	local childCount = #child
	
	--compute all childs with fixed height, other childs will be stretched
	local childFixedCount = 0
	local fixedOverall = 0
	local fluid = (s[sizeX] - mT - mB - (childCount - 1) * spacing) / childCount
	for _, v in ipairs(child) do
		if v.__data then rawset(v.__data, sizeX, 0) else v[sizeX] = 0 end
		if v.uiStyle and v.uiStyle[maxHeight] then
			--compute height
			if v.uiStyle[maxHeight] < fluid then
				v[sizeX] = v.uiStyle[maxHeight]
				fixedOverall = fixedOverall + v[sizeX]
				childFixedCount = childFixedCount + 1
			end
		end
	end
	local childFluidCount = childCount - childFixedCount
	
	fluid = math.ceil((s[sizeX] - fixedOverall - mT - mB - (childCount - 1) * spacing) / childFluidCount)
	local fluid2 = s[size2X] - mL - mR
	local offset_y = s[posX] + mT
	local offset_x = s[pos2X] + mL
	local sW = s[size2X] - mL - mR
	for _, v in ipairs(s._child) do
		v[posX] = offset_y
		if v[sizeX] == 0 then v[sizeX] = fluid end
		--compute width
		if v.uiStyle and v.uiStyle[maxWidth] then
			v[size2X] = math.min(v.uiStyle[maxWidth], fluid2)
		elseif v.uiStyle and v.uiStyle.minWidth then
			v[size2X] = math.max(v.uiStyle[minWidth], fluid2)
		else
			v[size2X] = fluid2
		end
		v[pos2X] = offset_x + (sW - v[size2X]) / 2
		offset_y = offset_y + v[sizeX] + spacing
		if v._update then v:_update() end
	end
end

local mouseover = function() end
local initui = function(parent, t)
	local e = Entity:new(parent):move(10,0):size(0,0)
	if ui.defaultStyles[t] then e.uiStyle = ui.defaultStyles[t] end
	e._draw = ui.draw
	e:mouseover(mouseover)
	e.setStyle = setStyle
	return e
end
local layoutUpdateKeys = {'y', 'x', 'h', 'w'}
ui.newLayout = function(parent, layoutStyle --[[horisontal/vertical]])
	local e = initui(parent, 'layout')
	if parent == screen then e:update(layoutUpdateKeys, layoutUpdate) end
	if layoutStyle then e.layoutStyle = layoutStyle else e.layoutStyle = "vertical" end
	e._update = layoutUpdate
	return e
end

ui.newButton = function(parent, text)
	local e = initui(parent, 'button')
	e._text = text
	return e
end

ui.newLabel = function(parent, text)
	local e = initui(parent, 'label')
	e._text = text
	return e
end

local sliderUpdate = function(s)
	local trough = s._child[1]
	local slider = s._child[2]
	--trough
	if s.uiStyle and s.uiStyle.trough then
		trough.w = s.uiStyle.trough.maxWidth or 100
		trough.h = s.uiStyle.trough.maxHeight or s.h
	else
		trough.w = 100
		trough.h = s.h
	end
	trough.x = s.x + s.w - trough.w
	trough.y = s.y + math.floor((s.h - trough.h) / 2)
	if not trough.uiStyle then trough.uiStyle = s.uiStyle.trough end
	--slider
	if s.uiStyle and s.uiStyle.slider then
		slider.w = s.uiStyle.slider.maxWidth or 28
		slider.h = s.uiStyle.slider.maxHeight or s.h
	else
		slider.w = 28
		slider.h = s.h
	end
	slider.x = trough.x + (s.value - s._min)/(s._max - s._min)*(trough.w - slider.w)
	slider.y = s.y + math.floor((s.h - slider.h) / 2)
	slider._drag_bound[1] = slider.y
	slider._drag_bound[3] = slider.y
	slider._drag_bound[2] = trough.x + trough.w - slider.w
	slider._drag_bound[4] = trough.x
	if not slider.uiStyle then slider.uiStyle = s.uiStyle.slider end
end
local sliderMove = function(s)
	local e = s._parent
	local value = (s.x - s._drag_bound[4])/(s._drag_bound[2] - s._drag_bound[4])*(e._max - e._min) + e._min
	if value ~= e.value then
		e.value = value
		--~ print(e.value)
	end
end
local sliderOnEditEnd = function(parent, callback)
	parent._child[2]:mouserelease(callback)
end
ui.newSlider = function(parent, text, min, current, max)
	local e = initui(parent, 'slider')
	e._text = text
	e.value = current or 0
	e._min = min or 0
	e._max = max or 1
	e:update(layoutUpdateKeys, sliderUpdate)
	e:update('uiStyle', sliderUpdate)
	setStyle(Entity:new(e):draw(ui.draw), e.uiStyle.trough) -- trough
	setStyle(Entity:new(e):draw(ui.draw):draggable({bound={0,0,0,0}, callback = sliderMove}), e.uiStyle.slider)
	e.onEditEnd = sliderOnEditEnd
	e._update = sliderUpdate
	sliderUpdate(e)
	return e
end

return ui
