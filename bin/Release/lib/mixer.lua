--[[****************************************************************************

Copyright (c) 2012 Pavel Roschin (aka RPG) <rpg89@post.ru>

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to
deal in the Software without restriction, including without limitation the
rights to use, copy, modify, merge, publish, distribute, sublicense, and/or
sell copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:  The above
copyright notice and this permission notice shall be included in all copies
or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS
IN THE SOFTWARE.

******************************************************************************]]

local ffi = require 'ffi'

ffi.cdef[[
typedef struct Mix_Chunk {
	int allocated;
	unsigned char *abuf;
	unsigned int alen;
	unsigned char volume;
} Mix_Chunk;
typedef struct _Mix_Music Mix_Music;
int Mix_OpenAudio(int frequency, unsigned short int format, int channels, int chunksize);
int Mix_AllocateChannels(int numchans);
Mix_Music * Mix_LoadMUS(const char *file);
Mix_Chunk * Mix_QuickLoad_RAW(const unsigned char *mem, unsigned int len);
int Mix_PlayChannelTimed(int channel, Mix_Chunk *chunk, int loops, int ticks);
void Mix_FreeChunk(Mix_Chunk *chunk);
void Mix_FreeMusic(Mix_Music *music);

int Mix_Volume(int channel, int volume);
int Mix_VolumeChunk(Mix_Chunk *chunk, int volume);
int Mix_VolumeMusic(int volume);

/* Pause/Resume a particular channel */
void Mix_Pause(int channel);
void Mix_Resume(int channel);
int Mix_Paused(int channel);

/* Pause/Resume the music stream */
int Mix_PlayMusic(Mix_Music *music, int loops);
void Mix_PauseMusic(void);
void Mix_ResumeMusic(void);
void Mix_RewindMusic(void);
int Mix_PausedMusic(void);

int Mix_HaltMusic(void);

void Mix_CloseAudio(void);

char * SDL_GetError(void);

typedef struct Sound {
	Mix_Chunk * chunk;
	unsigned int channel;
	char type;
} Sound;
]]

local CHANNELS = 64

local Mix = cheetah.loadDLL 'SDL_mixer'

--TODO: endiannness in format
if Mix.Mix_OpenAudio(44100, 0x8010, 2, 1024)==-1 then
	cheetah.myError("Mix_OpenAudio: %s\n", Mix.SDL_GetError())
end

Mix.Mix_AllocateChannels(CHANNELS);

cheetah.newSound = function(file)
	local buf = cheetah.getFile(file)
	local chunk = Mix.Mix_QuickLoad_RAW(buf, #buf)
	if not chunk then cheetah.myError("newSound: %s\n", Mix.SDL_GetError()) end
	local ptr = ffi.new('Sound')
	ptr.chunk = chunk
	return ptr
end

cheetah.playMusic = function(file, loops)
	if not cheetah.fileExists(file) then
		print('Music file "'..file..'" not found!')
		return nil
	end
	local mus = Mix.Mix_LoadMUS(file)
	Mix.Mix_PlayMusic(mus, loops or 0)
end

cheetah.soundVolume = function(volume)
	Mix.Mix_Volume(-1, volume)
end

--~ cheetah.soundStop = function()
--~ end

ffi.metatype('Sound', {
	__index = {
		play = function(s, loops)
			local channel = Mix.Mix_PlayChannelTimed(-1, s.chunk, (loops or 1) - 1, -1)
			if channel == -1 then
				cheetah.myError("playing sound: %s\n", Mix.SDL_GetError())
			end
			s.channel = channel
		end,
		setVolume = function(s, volume)
			Mix.Mix_VolumeChunk(s.chunk, volume)
		end,
		getVolume = function(s)
			return Mix.Mix_VolumeChunk(s.chunk, -1)
		end,
		pause = function(s)
			if s.channel > 0 then Mix.Mix_Pause(s.channel) end
		end,
		resume = function(s)
			if s.channel > 0 then Mix.Mix_Resume(s.channel) end
		end,
	},
	__gc = function(s) if s.chunk then Mix.Mix_FreeChunk(s.chunk) end end
})
