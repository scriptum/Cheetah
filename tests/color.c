#include <time.h>
#include <stdlib.h>
#include <stdio.h>

typedef struct Color {
	unsigned char r, g, b, a;
} Color;

typedef struct Colorf {
	float r, g, b, a;
} Colorf;

volatile unsigned prevUColor;
volatile Color prevCColor;

#define TEST_FUNC static void __attribute__ ((noinline))

TEST_FUNC color_shift(unsigned r, unsigned g, unsigned b, unsigned a) {
	unsigned low = b << 8 | (a & 0xff);
	unsigned high = r << 8 | g;
	unsigned c = (low & 0xffff) | (high << 16);
	if(c != prevUColor)
	{
		prevUColor = c;
	}
}

TEST_FUNC color_union(unsigned char r, unsigned char g, unsigned char b, unsigned char a) {
	union {
		unsigned uColor;
		Color cColor;
	} uniColor;
	uniColor.cColor = (Color){r,g,b,a};
	if(uniColor.uColor != prevUColor)
	{
		prevUColor = uniColor.uColor;
	}
}

TEST_FUNC color_struct(unsigned char r, unsigned char g, unsigned char b, unsigned char a) {
	Color c = {r,g,b,a};
	if(c.r != prevCColor.r || c.g != prevCColor.g || c.b != prevCColor.b || c.a != prevCColor.a)
	{
		prevCColor.r = r;
		prevCColor.g = g;
		prevCColor.b = b;
		prevCColor.a = a;
	}
}

TEST_FUNC color_struct_ptr(unsigned char r, unsigned char g, unsigned char b, unsigned char a) {
	Color c = {r,g,b,a};
	if(*(unsigned*)(&c) != *(unsigned*)(&prevCColor))
	{
		*(unsigned*)(&prevCColor) = *(unsigned*)(&c);
	}
}

TEST_FUNC color_pass_struct(Color c) {
	union {
		unsigned uColor;
		Color cColor;
	} uniColor;
	uniColor.cColor = c;
	if(uniColor.uColor != prevUColor)
	{
		prevUColor = uniColor.uColor;
	}
}

TEST_FUNC color_pass_array(char c[4]) {
	if(*((unsigned*)c) != prevUColor)
	{
		prevUColor = *((unsigned*)c);
	}
}

TEST_FUNC color_float(float r, float g, float b, float a) {
	unsigned c = 
		((unsigned)(r * 255.f)) << 24 | 
		((unsigned)(g * 255.f)) << 16 | 
		((unsigned)(b * 255.f)) << 8 | 
		((unsigned)(a * 255.f));
	if(c != prevUColor)
	{
		prevUColor = c;
	}
}

#define T(i,f) f(c[i].r, c[i].g, c[i].b, c[i].a);
#define TEST(description, f) begin = clock();\
for (i = 0; i < 50000000; i++)\
{T(0,f)T(1,f)T(2,f)T(3,f)T(4,f)T(5,f)T(6,f)T(7,f)T(8,f)T(9,f)}\
end = clock();\
time_spent = (double)(end - begin) / CLOCKS_PER_SEC;\
printf("%-40s %.3g s\n", description, time_spent);

int main() {
	clock_t begin, end;
	double time_spent;
	int i;
	volatile Color c[10] = {{0,0,0,0},{0,0,0,67},{0,0,23,54},{0,5,6,70},{0,50,70,80},{0,40,60,70},{0,50,30,20},{0,80,90,0},{0,236,0,30},{244,222,50,0}};
	volatile Colorf cf[10] = {{0.f,0.f,0.f,0.f},{0.f,0.f,0.f,67.f},{0.f,0.f,23.f,54.f},{0.f,5.f,6.f,70.f},{0.f,50.f,70.f,80.f},{0.f,40.f,60.f,70.f},{0.f,50.f,30.f,20.f},{0.f,80.f,90.f,0.f},{0.f,236.f,0.f,30.f},{244.f,222.f,50.f,0.f}};
	TEST("Bit shift", color_shift)
	TEST("Union", color_union)
	TEST("Struct storage", color_struct)
	TEST("Struct storage with pointers magick", color_struct_ptr)
	TEST("Floats", color_float)
	#undef T
	#define T(i,f) f(c[i]);
	TEST("Pass Color struct", color_pass_struct)
	#undef T
	#define T(i,f) f((char*)&(c[i]));
	TEST("Pass array", color_pass_array)
	return 0;
}
