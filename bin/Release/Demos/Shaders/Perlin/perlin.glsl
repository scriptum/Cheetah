uniform sampler2D noiseTex;
uniform int steps;              //4   number of iterations
uniform vec2 scale;             //4   noise scale
uniform float persist;          //0.5 iterations sum fading
uniform float contrast;         //1.0 contrast
uniform float brightness;       //0.0 brightness
uniform vec2 texel;
uniform vec2 noiseSize;
varying vec2 TexCoord;

/* Перлин работает на векторах, ориентированных случайным образом в пространстве.
* У нас простой случай, 2D, поэтому просто берём единичный двумерный вектор и
* поворачиваем его на случайный угол. Для большей точности можно соединить две-три
* компоненты пикселя (если брать только одну, случайных вращений будет всего 256). */
//~ vec2 noise2d(vec2 c)
//~ {
	//~ vec4 tex = texture2D(noiseTex, c);
	//~ float angle = (tex.r + tex.g * 390625e-8 /* + tex.b * 15258789e-12 */) * 8.0 * 3.1415926;
	//~ return vec2(cos(angle), sin(angle));
//~ }

/* Вариат с выборкой просто двух значений быстрее, но мы получаем ненормированный вектор */
vec2 noise2d(vec2 c)
{
	vec4 tex = texture2D(noiseTex, c);
	return (vec2(tex.r, tex.g) - vec2(0.5)) * 4.0;
}

/* безтекстурный вариант на случай, если синус на видеокарте будет дешевле чем 4 текстурных выборки */
//~ vec2 noise2d(vec2 c)
//~ {
	//~ float angle = fract(sin(dot(c, vec2(12.9898, 78.233))) * 43758.5453) * 8.0 * 3.1415926;
	//~ return vec2(cos(angle), sin(angle));
//~ }

/* придаёт шуму гладкость */
//~ float fade(float t) {
	//~ return t*t*t*(t*(t*6.0-15.0)+10.0);
//~ }

/* более быстрый вариант */
float fade(float t) {
	return smoothstep(0., 1., t);
}

/*
 * 2D классический шум Перлина. Наилучшее соотношение скорость/качество.
 */
float noisePerlin2D(vec2 c, vec2 rep)
{
	/* растягиваем интервал текстурных координат от 0 до 1 до нужного нам масштаба */
	vec2 cc = c*rep;
	/* позиция текстеля относительно пикселя шума, который обычно намного крупнее */
	vec2 pixelPosition = fract(cc);
	vec2 x11 = floor(cc);
	/* округление вектора сс даёт дискретные отсчёты шума с шагом rep */
	const vec4 offset = vec4(0., 0., 1., 1.);
	//~ vec4 Pi = mod(x11.xyxy + offset, rep.xyxy) * texel.xyxy;
	/* хитрый способ добиться бесшовности: используем возможность OpenGL повторять текстуру шума */
	vec4 Pi = (x11.xyxy + offset) / min(noiseSize.xyxy, rep.xyxy);
	vec4 Pf = (pixelPosition.xyxy - offset);

	/* выборка четырёх значений - расстояния векторов от текущей точки */
	float n00 = dot(noise2d(Pi.xy), Pf.xy);
	float n10 = dot(noise2d(Pi.zy), Pf.zy);
	float n01 = dot(noise2d(Pi.xw), Pf.xw);
	float n11 = dot(noise2d(Pi.zw), Pf.zw);

	/* Смешивание по оси Х */
	vec2 n_x = mix(vec2(n00, n01), vec2(n10, n11), fade(pixelPosition.x));

	/* Смешивание по оси Y */
	float n_xy = mix(n_x.x, n_x.y, fade(pixelPosition.y));

	/* Окончательное значение шума (-1, 1) */
	return n_xy;
}


void main( void )
{
	float n = 0.0;
	int i;
	float mul2 = 0.5;
	vec2 mul = scale;
	for(i = 1; i <= steps; i++)
	{
		n += noisePerlin2D(TexCoord, mul) * mul2;
		mul *= 2.0;
		mul2 *= persist;
	}
	gl_FragColor = vec4((vec3(n) * contrast + 1.0 + brightness) * 0.5, 1.0);
}
