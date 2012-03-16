local ffi = require "ffi"

ffi.cdef[[
unsigned char*
	SOIL_load_image_from_memory
	(
		const unsigned char *const buffer,
		int buffer_length,
		int *width, int *height, int *channels,
		int force_channels
	);
unsigned int
	SOIL_internal_create_OGL_texture
	(
		const unsigned char *const data,
		int width, int height, int channels,
		unsigned int reuse_texture_ID,
		unsigned int flags,
		unsigned int opengl_texture_type,
		unsigned int opengl_texture_target,
		unsigned int texture_check_size_enum
	);
void
	SOIL_free_image_data
	(
		unsigned char *img_data
	);
const char*
	SOIL_last_result
	(
		void
	);
enum
{
	SOIL_FLAG_POWER_OF_TWO = 1,
	SOIL_FLAG_MIPMAPS = 2,
	SOIL_FLAG_TEXTURE_REPEATS = 4,
	SOIL_FLAG_MULTIPLY_ALPHA = 8,
	SOIL_FLAG_INVERT_Y = 16,
	SOIL_FLAG_COMPRESS_TO_DXT = 32,
	SOIL_FLAG_DDS_LOAD_DIRECT = 64,
	SOIL_FLAG_NTSC_SAFE_RGB = 128,
	SOIL_FLAG_CoCg_Y = 256,
	SOIL_FLAG_TEXTURE_RECTANGLE = 512
};
]]
local C = cheetah
local SOIL = C.loadDLL 'SOIL'

cheetah.newImage = function(name, options)
	local width = ffi.new('int[1]')
	local height = ffi.new('int[1]')
	local channel = ffi.new('int[1]')
	local size = ffi.new('int[1]')
	local buf, img, tex_id
	if C.isInit() then
		buf = C.loadfile(name, size)
		img = SOIL.SOIL_load_image_from_memory(buf, size[0], width, height, channel, 0)
		if not C.isPointer(img) then
			C.myError("newImage: can't load image %s %s", name, SOIL.SOIL_last_result());
		else
			tex_id = SOIL.SOIL_internal_create_OGL_texture(
				img, width[0], height[0], channel[0],
				0, SOIL.SOIL_FLAG_TEXTURE_REPEATS,
				C.GL_TEXTURE_2D, C.GL_TEXTURE_2D,
				C.GL_MAX_TEXTURE_SIZE);
		end
	--~ else
		--~ C.myError("newImage: call init function first")
	end
	local ptr = ffi.new('Image')
	ptr.id, ptr.w, ptr.h = tex_id or 0, width[0], height[0]
	return ptr
end