#version 400

uniform vec2 maxSize = vec2(<SizeX>, <SizeY>);
uniform float cx = <cx>;
uniform float cy = <cy>;
uniform float zoom = <zoom>;
uniform float scalex = <ScaleX>;
uniform float scaley = <ScaleY>;
uniform float R = <R>;
uniform float re_complex = <rec>;
uniform float im_complex = <imc>;
uniform int palettesize = 300;
uniform int maxIteration = <iter>;
uniform int shift = <shift>;
uniform sampler2D palette;
uniform vec4 black = vec4(0.0, 0.0, 0.0, 0.0);
uniform float h = 0;
uniform float vr = 1;
uniform float vi = 0;
uniform float RED = 1;
uniform float GREEN = 1;
uniform float BLUE = 1;
uniform float N = 1.0;

float zx, zy, x2, y2, x0, y0;
int iterations;

vec4 col;

in vec4 gl_FragCoord;

out vec4 FragColor;

vec4 CSet1(float V) {
	vec4 COL = vec4((1 - cos(3.32192809489*V))/2, (1 - cos(0.78298596083*V))/2, (1 - cos(0.413668099253*V))/2, 1.0);
	COL.a = 1.0;
	return COL;
}

float atan2(float x, float y) {
	return -sign(x*y)*atan((abs(x)-abs(y))/(abs(x)+abs(y)));
}

void JuliaSet1() {
	zx = 2 * R * (gl_FragCoord.x/maxSize.x - 0.5) * scalex * zoom - cx;
	zy = 2 * R * (gl_FragCoord.y/maxSize.y - 0.5) * scaley * zoom - cy;
	x2 = 0;
	y2 = 0;
	iterations = 0;
	while(x2 + y2<= R*R && iterations < maxIteration) {
		x2 = zx * zx;
		y2 = zy * zy;
		float xtemp = pow((x2 + y2), N / 2.0) * cos(N*atan(zy, zx)) + re_complex;
		zy = pow((x2 + y2), N / 2.0) * sin(N*atan(zy, zx)) + im_complex;
		zx = xtemp;
		iterations++;
	}
	if(iterations == maxIteration) {
		FragColor = black;
	} 
	else {
		col = texture2D(palette, vec2(((iterations+shift) % palettesize) / float(palettesize), 0)).rgba;
		FragColor = col;
	}
}

void JuliaSet2() {
	zx = 2 * R * (gl_FragCoord.x/maxSize.x - 0.5) * scalex * zoom - cx;
	zy = 2 * R * (gl_FragCoord.y/maxSize.y - 0.5) * scaley * zoom - cy;
	x2 = 0;
	y2 = 0;
	double POW = 1.0;
	for(iterations = 0; iterations < maxIteration; iterations++) {
		x2 = zx * zx;
		y2 = zy * zy;
		if (x2 + y2 > 100000) {
			double V = double(log(x2 + y2))/POW;
			col = CSet1(log(float(V))/3.0);
			break;
		}
		float xtemp = pow((x2 + y2), N / 2.0) * cos(N*atan(zy, zx)) + re_complex;
		zy = pow((x2 + y2), N / 2.0) * sin(N*atan(zy, zx)) + im_complex;
		zx = xtemp;
		POW *= 2;
	}
	FragColor = col;
}

void JuliaSet3() {
	zx = -2 * (gl_FragCoord.x/maxSize.x - 0.5) * scalex * zoom + cx;
	zy = -2 * (gl_FragCoord.y/maxSize.y - 0.5) * scaley * zoom + cy;
	float dcr = 1.0;
	float dci = 0.0;
	float der = 1.0;
	float dei = 0.0;
	x2 = 0;
	y2 = 0;
	int Reason = 0;
	for(iterations = 0; iterations < maxIteration; iterations++) {
		x2 = zx * zx;
		y2 = zy * zy;
		if( x2 + y2 > 10000) {
			Reason = 1;
			break;
		}
		float newZi = 2 * zx * zy + im_complex;
		float newZr = x2 - y2 + re_complex;
		float newDer = 2.0 * (der * zx - dei * zy) + dcr;
		float newDei = 2.0 * (der * zy + dei * zx) + dci;
		zx = newZr;
		zy = newZi;
		der = newDer;
		dei = newDei;
	}
	if(Reason == 0) {
		col = black;
	} 
	else {
		float ux = (zx * der + zy * dei)/(der * der + dei * dei);
		float uy = (zy * der - zx * dei)/(der * der + dei * dei);
		float absu = sqrt(ux * ux + uy * uy);
		ux /= absu;
		uy /= absu;
		float t = ux * vr + uy * vi + h;
		t /= (1 + h);
		if (t < 0) t = 0;
		col = mix(vec4(0.0, 0.0, 0.0, 1.0), vec4(RED, GREEN, BLUE, 1.0), t);
	}
	FragColor = col;
}

void Mandelbrot1() {
	x0 = -2 * (gl_FragCoord.x/maxSize.x - 0.5) * scalex * zoom - cx;
	y0 = -2 * (gl_FragCoord.y/maxSize.y - 0.5) * scaley * zoom - cy;
	zx = 0;
	zy = 0;
	x2 = 0;
	y2 = 0;
	iterations = 0;
	while(x2 + y2 <= R*R && iterations < maxIteration) {
		float xtemp = pow((x2 + y2), N / 2.0) * cos(N*atan(zy, zx)) + x0;
		zy =  pow((x2 + y2), N / 2.0) * sin(N*atan(zy, zx)) + y0;
		zx = xtemp;
		x2 = zx * zx;
		y2 = zy * zy;
		iterations++;
	}
	if(iterations == maxIteration) {
		FragColor = black;
	} 
	else {
		col = texture2D(palette, vec2(((iterations+shift) % palettesize) / float(palettesize), 0)).rgba;
		FragColor = col;
	}
}

void Mandelbrot2() {
	x0 = -2 * (gl_FragCoord.x/maxSize.x - 0.5) * scalex * zoom - cx;
	y0 = -2 * (gl_FragCoord.y/maxSize.y - 0.5) * scaley * zoom - cy;
	zx = 0;
	zy = 0;
	x2 = 0;
	y2 = 0;
	double POW = 1.0;
	for(iterations = 0; iterations < maxIteration; iterations++) {
		x2 = zx * zx;
		y2 = zy * zy;
		if (x2 + y2 > 100000) {
			double V = double(log(x2 + y2))/POW;
			col = CSet1(log(float(V))/3.0);
			break;
		}
		float xtemp = pow((x2 + y2), N / 2.0) * cos(N*atan(zy, zx)) + x0;
		zy =  pow((x2 + y2), N / 2.0) * sin(N*atan(zy, zx)) + y0;
		zx = xtemp;
		POW *= 2;
	}
	FragColor = col;
}

void Mandelbrot3() {
	x0 = -2 * (gl_FragCoord.x/maxSize.x - 0.5) * scalex * zoom - cx;
	y0 = -2 * (gl_FragCoord.y/maxSize.y - 0.5) * scaley * zoom - cy;
	zx = x0;
	zy = y0;
	float dcr = 1.0;
	float dci = 0.0;
	float der = 1.0;
	float dei = 0.0;
	x2 = 0;
	y2 = 0;
	int Reason = 0;
	for(iterations = 0; iterations < maxIteration; iterations++) {
		x2 = zx * zx;
		y2 = zy * zy;
		if( x2 + y2 > 10000) {
			Reason = 1;
			break;
		}
		float newZi = 2 * zx * zy + y0;
		float newZr = x2 - y2 + x0;
		float newDer = 2.0 * (der * zx - dei * zy) + dcr;
		float newDei = 2.0 * (der * zy + dei * zx) + dci;
		zx = newZr;
		zy = newZi;
		der = newDer;
		dei = newDei;
	}
	if(Reason == 0) {
		col = black;
	} 
	else {
		float ux = (zx * der + zy * dei)/(der * der + dei * dei);
		float uy = (zy * der - zx * dei)/(der * der + dei * dei);
		float absu = sqrt(ux * ux + uy * uy);
		ux /= absu;
		uy /= absu;
		float t = ux * vr + uy * vi + h;
		t /= (1 + h);
		if (t < 0) t = 0;
		col = mix(vec4(0.0, 0.0, 0.0, 1.0), vec4(RED, GREEN, BLUE, 1.0), t);
	}
	FragColor = col;
}

void main() {
	<setMainType><cmod>();
}
