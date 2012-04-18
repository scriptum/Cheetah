require 'lib.cheetah'
--~ require 'lib.mixer'
require 'lib.lquery.init'
local C = cheetah
C.init('Test', 512, 512, 32, '')


print(cheetah.newShader(

[[#version 120

attribute vec3 position;
attribute vec3 color;

varying vec3 fragcolor;

void main(void)
{
  gl_Position = vec4(position, 1.0) * gl_ModelViewProjectionMatrix;
  fragcolor = color;
}]],

[[#version 120

varying vec3 fragcolor;

void main(void)
{
  gl_FragColor = vec4(fragcolor, 1.0);
}]]

))
