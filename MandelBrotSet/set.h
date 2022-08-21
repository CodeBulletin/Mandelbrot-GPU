#pragma once
#include "functions.h"
#include "UI.h"

class Set {
public:
	String shader_name, shader_raw_code, shader_code;
	int type, cmod, palettesize, maxIteration, shift;
	Vec2d center, scale;
	Vec2d complex_number, maxCoord, minCoord;
	sf::Texture palette;
	sf::Color set_color;
	sf::Shader shader;
	long double zoom, R, N;
	float Height, LightAngle, Red, Green, Blue;
	std::vector<String> CMods = { "1", "2", "3", "4" };
	std::vector<String> SetTypes = { "JuliaSet", "Mandelbrot"};
	sf::VertexArray v;

	Set(int _type, int _cmod, String _shader_name, Vec2d _minCoord, sf::Texture& _palette,
		Vec2d _maxCoord, OVec2d _center = {}, OVec2d _complex_number = {}, OColor _col = {},
		ODouble _zoom = {}, ODouble _R = {}, OInt _palettesize = {}, OInt _maxIteration = {}, OInt _shift = {})
		: type(_type), cmod(_cmod), shader_name(_shader_name) {
		setShader();
		minCoord = _minCoord;
		maxCoord = _maxCoord;
		center = _center.value_or(Vec2d(0.5, 0));
		if (type == 1) {
			if (cmod == 0) R = _R.value_or(4.0);
			else R = 1000;
		}
		else {
			complex_number = _complex_number.value_or(Vec2d(0, 0));
			double r = 0.5 * std::sqrt(4 * std::sqrt(complex_number.x * complex_number.x + complex_number.y * complex_number.y) + 1) + 0.5;
			R = getPositiveMin(r, -r);
		}
		zoom = _zoom.value_or(1);
		scale = Vec2d(-(maxCoord.x - minCoord.x) / (maxCoord.y - minCoord.y), -1.0);
		maxIteration = _maxIteration.value_or(500);
		compile_shader();
		if(cmod == 0) setPalette(_palette);
		//setIterations(_maxIteration.value_or(50));
		v = sf::VertexArray(sf::Quads, 4);
		v[0].position = Vec2f(_minCoord.x, _minCoord.y);
		v[1].position = Vec2f(_maxCoord.x, _minCoord.y);
		v[2].position = Vec2f(_maxCoord.x, _maxCoord.y);
		v[3].position = Vec2f(_minCoord.x, _maxCoord.y);
		Red = 0;
		Blue = 0;
		Green = 0;
	};

	void setShader();
	void setComplexNumber(Vec2d);
	void setR(long double);
	void setCenter(Vec2d);
	void setZoom(long double, long double);
	void setZoomAtMouse(long double, long double, Vec2d);
	void setIterations(int iter);
	void setValues(UI&);
	void setLoaction(long double x, long double y, long double speed);
	void setHeight(float);
	void setAngle(float);
	void compile_shader();
	void setN(long double N) {
		shader.setUniform("N", float(N));
	}
	void setRGB(int R, int G, int B) {
		shader.setUniform("RED", R / 255.0f);
		shader.setUniform("GREEN", G / 255.0f);
		shader.setUniform("BLUE", B / 255.0f);
	}
	void setType(int _type) {
		type = _type;
	}
	void setCMod(int _cmod) {
		cmod = _cmod;
	}
	void setPalette(sf::Texture& _pallete) {
		palette = _pallete;
		shader.setUniform("palette", palette);
	}
	void draw(sf::RenderWindow& win) {
		win.draw(v, &shader);
	}

private:
	static void replace_all(String& main_string, String&& substring, String& with) {
		size_t pos = main_string.find(substring);
		size_t subsize = substring.size();
		size_t withsize = with.size();
		while (pos != std::string::npos)
		{
			main_string.replace(pos, subsize, with);
			pos = main_string.find(substring, pos + withsize);
		}
	}
	static void replace_all(String& main_string, String&& substring, String&& with) {
		size_t pos = main_string.find(substring);
		size_t subsize = substring.size();
		size_t withsize = with.size();
		while (pos != std::string::npos)
		{
			main_string.replace(pos, subsize, with);
			pos = main_string.find(substring, pos + withsize);
		}
	}
	static double getPositiveMin(double a, double b) {
		if (a < 0) {
			return b;
		}
		else if (b < 0) {
			return a;
		}
		else if (a > b) {
			return b;
		}
		else if (b > a) {
			return a;
		}
	}
};