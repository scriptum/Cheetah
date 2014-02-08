local path = 'lib/lqueryui/styles/oxygen/'
local lineHeight = 28
local font
if love then
	S = {newImage = love.graphics.newImage}
	font = love.graphics.newFont( 12 )
else
	font = Fonts["Liberation Sans bold"][9]
end
local trough = S.newImage(path .. 'slider/trough-horizontal.png')
local slider = S.newImage(path .. 'slider/slider-horiz.png')
return {
	button = {
		backgroundImage = S.newImage(path .. 'buttons/button-normal.png'),
		borderImage = 7,
		margin = 4,
		color = {0,0,0},
		maxHeight = lineHeight,
		font = font,
		hover = {
			backgroundImage = S.newImage(path .. 'buttons/button-hover.png')
		},
		click = {
			backgroundImage = S.newImage(path .. 'buttons/button-active.png')
		}
	},
	slider = {
		color = {0,0,0},
		maxHeight = lineHeight,
		margin = 4,
		align = 'left',
		font = font,
		trough = {
			backgroundImage = trough,
			borderImage = {0, 2},
			maxHeight = trough:getHeight(),
			maxWidth = 100
		},
		slider = {
			backgroundImage = slider,
			maxWidth = slider:getWidth(),
			maxHeight = slider:getHeight()
		}
	},
	layout = {
		margin = 4,
		background = {200,200,200},
		padding = 8
	},
	
}
