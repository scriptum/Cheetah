local font
if love then
	S = {newImage = G.newImage}
	font = love.graphics.newFont( 12 )
else
	font = Fonts["Liberation Sans bold"][9]
end
return {
	button = {
		border = 1,
		borderColor = {200,200,200},
		background = {100,100,100},
		margin = 4,
		font = font,
		hover = {
			background = {150,150,150},
		},
		click = {
			background = {150,180,255}
		}
	},
	slider = {
		maxHeight = 28,
		margin = 4,
		align = 'left',
		font = font,
		trough = {
			background = {100,100,100},
			maxHeight = 3,
			maxWidth = 100
		},
		slider = {
			border = 1,
			borderColor = {200,200,200},
			background = {100,100,100},
			maxWidth = 8,
			maxHeight = 13
		}
	},
	layout = {
		border = 2,
		borderColor = {200,200,200},
		background = {0,0,0},
		margin = 4,
		padding = {8, 8, 8, 8},
		hover = {
			borderColor = {0,0,255} 
		},
	},
	
}
