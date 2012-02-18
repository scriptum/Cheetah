package.name = "cheetah"
package.kind = "dll"
package.language = "c"
package.files = { 
	matchfiles(
		"src/*.h", "src/*.c"
		--~ "src/SOIL/*.h", 
		--~ "src/SOIL/*.c"
		--~ "Graphics/*.h", 
		--~ "Graphics/*.c"
		)
}
project.configs = {"Release"}
project.config["Release"].bindir = "bin/Release"
package.buildflags = {"optimize-size", "no-frame-pointer"}
package.libpaths = {"lib"}
package.includepaths = {
	--~ "thirdparty/LuaJIT/src", 
	"inc"
}
package.links = {"SDL", "SDLmain", "GL", --[["luajit",]]}
--~ package.buildoptions = { "`sdl-config --cflags` -s -ffast-math -I/usr/include/lua5.1" }
package.linkoptions = { "-s" }

package.postbuildcommands = {
	"./genheader.sh"
}