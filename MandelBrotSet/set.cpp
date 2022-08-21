#include "set.h"

void Set::setHeight(float height) {
	Height = height;
	shader.setUniform("h", 2 - Height);
}

void Set::setAngle(float angle) {
	LightAngle = angle;
	shader.setUniform("vr", std::sinf(angle));
	shader.setUniform("vi", std::cosf(angle));
}

void Set::setShader() {
	std::ifstream f(shader_name);
	if (f) {
		std::ostringstream ss;
		ss << f.rdbuf();
		shader_raw_code = ss.str();
	}
	else {
		std::cout << "error unable to open " << shader_name;
		std::cin.get();
		exit(0);
	}
}

void Set::setLoaction(long double x, long double y, long double speed) {
	if (type == 1) {
		if (x != 0) center.x += x * zoom * speed / 10.0;
		if (y != 0) center.y += y * zoom * speed / 10.0;
	}
	else {
		long double r = 6.6 + 4.4 * (- R / 2 + 1.0);
		if (x != 0) center.x -= x * zoom * speed / r;
		if (y != 0) center.y -= y * zoom * speed / r;
	}
	setCenter(center);
}

void Set::setComplexNumber(Vec2d _complex_number) {
	if (type == 0) {
		complex_number = _complex_number;
		long double r = 0.5 * std::sqrt(4 * std::sqrt(complex_number.x * complex_number.x + complex_number.y * complex_number.y) + 1) + 0.5;
		R = getPositiveMin(r, -r);
		shader.setUniform("re_complex", float(complex_number.x));
		shader.setUniform("im_complex", float(complex_number.y));
	}
}

void Set::setR(long double _R) {
	R = _R;
	shader.setUniform("R", float(R));
}

void Set::setZoom(long double _zoom, long double _zoompow) {
	zoom = std::pow(_zoompow, -_zoom);
	shader.setUniform("zoom", float(zoom));
}

void Set::setCenter(Vec2d _center) {
	center = _center;
	shader.setUniform("cx", float(center.x));
	shader.setUniform("cy", float(center.y));
}

void Set::setZoomAtMouse(long double _zoom, long double _zoompow, Vec2d mousePos) {
	if (type == 0) {
		center = Vec2d(-2.0 * R * zoom * scale.x * (mousePos.x / (maxCoord.x - minCoord.x) - 0.5) + center.x,
			2.0 * R * zoom * scale.y * (mousePos.y / (maxCoord.y - minCoord.y) - 0.5) + center.y);
		zoom = std::pow(_zoompow, -_zoom);
		Vec2d Point = Vec2d(-2.0 * R * zoom * scale.x * (mousePos.x / (maxCoord.x - minCoord.x) - 0.5),
			2.0 * R * zoom * scale.y * (mousePos.y / (maxCoord.y - minCoord.y) - 0.5));
		center -= Point;
	}
	else if (type == 1) {
		center = Vec2d(2 * (mousePos.x / (maxCoord.x - minCoord.x) - 0.5) * scale.x * zoom + center.x,
			-2 * (mousePos.y / (maxCoord.y - minCoord.y) - 0.5) * scale.y * zoom + center.y);
		zoom = std::pow(_zoompow, -_zoom);
		Vec2d Point = Vec2d(2.0 * (mousePos.x / (maxCoord.x - minCoord.x) - 0.5) * scale.x * zoom,
			-2 * (mousePos.y / (maxCoord.y - minCoord.y) - 0.5) * scale.y * zoom);
		center -= Point;
	}
	shader.setUniform("zoom", float(zoom));
	shader.setUniform("cx", float(center.x));
	shader.setUniform("cy", float(center.y));
}

void Set::setIterations(int iter) {
	maxIteration = iter;
	shader.setUniform("maxIteration", iter);
}

void Set::compile_shader() {
	shader_code = shader_raw_code;
	replace_all(shader_code, "<SizeX>", toStr(maxCoord.x - minCoord.x));
	replace_all(shader_code, "<SizeY>", toStr(maxCoord.y - minCoord.y));
	replace_all(shader_code, "<cmod>", CMods[cmod]);
	replace_all(shader_code, "<rec>", toStr(complex_number.x));
	replace_all(shader_code, "<imc>", toStr(complex_number.y));
	replace_all(shader_code, "<ScaleX>", toStr(scale.x));
	replace_all(shader_code, "<ScaleY>", toStr(scale.y));
	replace_all(shader_code, "<setMainType>", SetTypes[type]);
	replace_all(shader_code, "<R>", toStr(R));
	replace_all(shader_code, "<zoom>", toStr(zoom));
	replace_all(shader_code, "<cx>", toStr(center.x));
	replace_all(shader_code, "<cy>", toStr(center.y));
	replace_all(shader_code, "<iter>", toStr(maxIteration));
	replace_all(shader_code, "<shift>", toStr(shift));
	if (!shader.loadFromMemory(shader_code, sf::Shader::Fragment)) {
		std::cout << "error";
		std::cin.get();
		exit(0);
	}
	setIterations(maxIteration);
	if (cmod != 2) setN(N);
	if (cmod == 0) setPalette(palette);
	if (type == 0) setComplexNumber(complex_number);
	if (type == 1) setR(2);
	if (cmod == 2) setHeight(Height);
	if (cmod == 2) setAngle(LightAngle);
	if (cmod == 2) setRGB(Red, Green, Blue);
}

void Set::setValues(UI& mainui) {
	compile_shader();
	if (type == 0 && cmod == 0) {
		mainui.RedoPos({ {200, 300}, {200, 400}, {200, 500}, {}, {}, {}, {}, {}, {200, 600} }, { true, true, true, false, false, false, false, false, true });
	}
	if (type == 0 && cmod == 1) {
		mainui.RedoPos({ {200, 300}, {200, 400}, {200, 500}, {}, {}, {}, {}, {}, {} }, { true, true, true, false, false, false, false, false, false });
	}
	if (type == 0 && cmod == 2) {
		mainui.RedoPos({ {}, {200, 300}, {200, 400}, {200, 500}, {200, 600}, {200, 700}, {200, 800}, {200, 900}, {} }, { false, true, true, true, true, true, true, true, false});
	}
	if (type == 1 && cmod == 0) {
		mainui.RedoPos({ {200, 300}, {}, {}, {}, {}, {}, {}, {}, {200, 400} }, { true, false, false, false, false, false, false, false, true });
	}
	if (type == 1 && cmod == 1) {
		mainui.RedoPos({ {200, 300}, {}, {}, {}, {}, {}, {}, {}, {} }, { true, false, false, false, false, false, false, false, false });
	}
	if (type == 1 && cmod == 2) {
		mainui.RedoPos({ {}, {}, {}, {200, 300}, {200, 400}, {200, 500}, {200, 600}, {200, 700}, {} }, { false, false, false, true, true, true, true, true, false });
	}
}