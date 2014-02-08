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

local key = {}

key["unknown"] = 0
key["backspace"] = 8
key["tab"] = 9
key["clear"] = 12
key["return"] = 13
key["pause"] = 19
key["escape"] = 27
key["space"] = 32
key["!"] = 33
key["\""] = 34
key["#"] = 35
key["$"] = 36
key["&"] = 38
key["'"] = 39
key["("] = 40
key[")"] = 41
key["*"] = 42
key["+"] = 43
key[","] = 44
key["-"] = 45
key["."] = 46
key["/"] = 47
key["0"] = 48
key["1"] = 49
key["2"] = 50
key["3"] = 51
key["4"] = 52
key["5"] = 53
key["6"] = 54
key["7"] = 55
key["8"] = 56
key["9"] = 57
key[":"] = 58
key[59] = ""
key["<"] = 60
key["="] = 61
key[">"] = 62
key["?"] = 63
key["@"] = 64

key["["] = 91
key["\\"] = 92
key["]"] = 93
key["^"] = 94
key["_"] = 95
key["`"] = 96
key["a"] = 97
key["b"] = 98
key["c"] = 99
key["d"] = 100
key["e"] = 101
key["f"] = 102
key["g"] = 103
key["h"] = 104
key["i"] = 105
key["j"] = 106
key["k"] = 107
key["l"] = 108
key["m"] = 109
key["n"] = 110
key["o"] = 111
key["p"] = 112
key["q"] = 113
key["r"] = 114
key["s"] = 115
key["t"] = 116
key["u"] = 117
key["v"] = 118
key["w"] = 119
key["x"] = 120
key["y"] = 121
key["z"] = 122
key["delete"] = 127


key["kp0"] = 256
key["kp1"] = 257
key["kp2"] = 258
key["kp3"] = 259
key["kp4"] = 260
key["kp5"] = 261
key["kp6"] = 262
key["kp7"] = 263
key["kp8"] = 264
key["kp9"] = 265
key["kp_period"] = 266
key["kp_divide"] = 267
key["kp_multiply"] = 268
key["kp_minus"] = 269
key["kp_plus"] = 270
key["kp_enter"] = 271
key["kp_equals"] = 272

key["up"] = 273
key["down"] = 274
key["right"] = 275
key["left"] = 276
key["insert"] = 277
key["home"] = 278
key["end"] = 279
key["pageup"] = 280
key["pagedown"] = 281

key["f1"] = 282
key["f2"] = 283
key["f3"] = 284
key["f4"] = 285
key["f5"] = 286
key["f6"] = 287
key["f7"] = 288
key["f8"] = 289
key["f9"] = 290
key["f10"] = 291
key["f11"] = 292
key["f12"] = 293
key["f13"] = 294
key["f14"] = 295
key["f15"] = 296

key["numlock"] = 300
key["capslock"] = 301
key["scrollock"] = 302
key["rshift"] = 303
key["lshift"] = 304
key["rctrl"] = 305
key["lctrl"] = 306
key["ralt"] = 307
key["lalt"] = 308
key["rmeta"] = 309
key["lmeta"] = 310
key["lsuper"] = 311
key["rsuper"] = 312
key["mode"] = 313
key["compose"] = 314

key["help"] = 315
key["print"] = 316
key["sysreq"] = 317
key["break"] = 318
key["menu"] = 319
key["power"] = 320
key["euro"] = 321
key["undo"] = 322

return key