package.name = "cheetah"
package.kind = "dll"
package.language = "c"
package.files = { 
	matchfiles(
		"src/*.h", "src/*.c",
		"src/SOIL/*.h", 
		"src/SOIL/*.c"
		--~ "Graphics/*.h", 
		--~ "Graphics/*.c"
		)
}
project.configs = {"Release"}
project.config["Release"].bindir = "bin/Release"
package.buildflags = {"optimize-speed", "no-frame-pointer"}
package.libpaths = {"lib"}
package.includepaths = {
	--~ "thirdparty/LuaJIT/src", 
	"inc"
}
package.links = {"SDL", "SDLmain", "GL", --[["luajit",]]}
--~ package.buildoptions = { "-ffast-math -mtune=generic -march=core2" }
package.linkoptions = { "-s" }
--~ package.buildoptions = { "-g" }
package.postbuildcommands = {
	"./genheader.sh"
}