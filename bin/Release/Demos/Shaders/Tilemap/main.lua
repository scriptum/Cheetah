require 'lib.cheetah'
require 'lib.lquery.init'
local C = cheetah
--set window width and height
local scr_w, scr_h = 800, 600
--map size
local map_size = 64
local tile_size = 16
C.init('Lights', scr_w, scr_h, 32, '')
--print fps
C.printFPS = true
--tileset
local tileSet = C.newImage('tiles.png', '')
--generate random tilemap, nearest interpolation needed!
local tileMap = C.generate('noise', map_size, map_size, 'n')

local multi = C.newMultitexture(tileSet, tileMap)

local shader = C.newShader([[#version 120
uniform sampler2D tileSet;
uniform sampler2D tileMap;
uniform float tilesInRow;
uniform float scale;
void main() {
	//we have only 16 tiles in a row, discrete them
	vec2 offset = floor(texture2D(tileMap, gl_TexCoord[0].st).rg*tilesInRow)/tilesInRow;
	gl_FragColor = texture2D(tileSet, fract(gl_TexCoord[0].st*scale*tilesInRow)/tilesInRow + offset);
}
]])

E:new(screen):draw(function(s)
	shader:bind()
	shader:set('tileSet', 0)
	shader:set('tileMap', 1)
	shader:set('tilesInRow', tile_size)
	shader:set('scale', map_size*tile_size/tileSet.w)
	local total_size = map_size*tile_size
	multi:draw(-total_size/scr_w*lQuery.mX/20+scr_w/2, -total_size/scr_h*lQuery.mY/20+scr_h/2, total_size, total_size, time/1000)
end)

--do not forget about main loop!
C.mainLoop()