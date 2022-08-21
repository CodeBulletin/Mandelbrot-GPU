#pragma once
#include "Slider.h"
class UI {
private:
	Vec2f min, max;
	bool isActive;
	sf::VertexArray v;
	sf::Vertex v1, v2, v3, v4;
	std::vector<sf::Text> texts;
	std::vector<SliderUI<float>> fsliders;
	std::vector<SliderUI<int>> isliders;
	std::vector<SliderUI<long double>> dsliders;
	std::vector<bool> activesliders;
	float val;
	bool inside(Vec2f Vec) {
		return Vec.x > min.x && Vec.x < max.x && Vec.y > min.y && Vec.y < max.y;
	}
	sf::Text t1, t2, tmain;
	sf::Font font;
public:
	UI(Vec2f _min, Vec2f _max, bool state, sf::Color background, sf::Font _font) : min(_min), max(_max), isActive(state) {
		v = sf::VertexArray(sf::Quads, 4);
		v1.position = Vec2f(min.x, min.y);
		v2.position = Vec2f(max.x, min.y);
		v3.position = Vec2f(max.x, max.y);
		v4.position = Vec2f(min.x, max.y);
		v1.color = background;
		v2.color = background;
		v3.color = background;
		v4.color = background;
		texts = std::vector<sf::Text>();
		font = _font;
		t1.setFont(font);
		t2.setFont(font);
		tmain.setFont(font);
		tmain.setString("Complex Fractals");
		t1.setCharacterSize(25);
		t2.setCharacterSize(25);
		tmain.setCharacterSize(25);
		tmain.setOrigin(tmain.getLocalBounds().left + tmain.getLocalBounds().width / 2.0f, tmain.getLocalBounds().top + tmain.getLocalBounds().height / 2.0f);
		t1.setFillColor(sf::Color::Black);
		t2.setFillColor(sf::Color::Black);
		tmain.setFillColor(sf::Color::Black);
		tmain.setPosition(200, 100);
		t1.setPosition(200, 175);
		t2.setPosition(200, 225);
	}
	bool collides(Vec2f mouse) {
		return isActive && inside(mouse);
	}
	void setActive(bool val) {
		isActive = val;
	}
	bool getActive() {
		return isActive;
	}
	sf::VertexArray& draw(std::string tSet, std::string tCol) {
		t1.setString("set: " + tSet);
		t2.setString("Coloring Algo: " + tCol);
		t1.setOrigin(t1.getLocalBounds().left + t1.getLocalBounds().width / 2.0f, t1.getLocalBounds().top + t1.getLocalBounds().height / 2.0f);
		t2.setOrigin(t2.getLocalBounds().left + t2.getLocalBounds().width / 2.0f, t2.getLocalBounds().top + t2.getLocalBounds().height / 2.0f);
		v.clear();
		texts.clear();
		texts.push_back(tmain);
		texts.push_back(t1);
		texts.push_back(t2);
		if (isActive) {
			v.append(v1);
			v.append(v2);
			v.append(v3);
			v.append(v4);
			int i = 0;
			for (SliderUI<long double>& s : dsliders) {
				if (activesliders[i]) s.draw(v, texts);
				i++;
			}
			for (SliderUI<float>& s : fsliders) {
				if (activesliders[i]) s.draw(v, texts);
				i++;
			}
			for (SliderUI<int>& s : isliders) {
				if (activesliders[i]) s.draw(v, texts);
				i++;
			}
		}
		return v;
	}
	std::vector<sf::Text>& getText() {
		return texts;
	}
	bool dragged(sf::Event::MouseMoveEvent& mouse) {
		if (collides({(float)mouse.x, (float)mouse.y}) && isActive) {
			int i = 0;
			for (SliderUI<long double>& s : dsliders) {
				if (activesliders[i]) s.dragged(mouse);
				i++;
			}
			for (SliderUI<float>& s : fsliders) {
				if (activesliders[i]) s.dragged(mouse);
				i++;
			}
			for (SliderUI<int>& s : isliders) {
				if (activesliders[i]) s.dragged(mouse);
				i++;
			}
			return true;
		}
		else {
			return false;
		}
	}
	void updatedragged() {
		for (SliderUI<float>& s : fsliders) {
			s.setActive(false);
		}
		for (SliderUI<int>& s : isliders) {
			s.setActive(false);
		}
		for (SliderUI<long double>& s : dsliders) {
			s.setActive(false);
		}
	}
	bool clicked(const Vec2f& mouse) {
		if (collides(mouse) && isActive) {
			int i = 0;
			for (SliderUI<long double>& s : dsliders) {
				if (activesliders[i]) s.clicked(mouse);
				i++;
			}
			for (SliderUI<float>& s : fsliders) {
				if (activesliders[i]) s.clicked(mouse);
				i++;
			}
			for (SliderUI<int>& s : isliders) {
				if (activesliders[i]) s.clicked(mouse);
				i++;
			}
			return true;
		}
		else {
			return false;
		}
	}
	void addSlider(SliderUI<int> x) {
		isliders.push_back(x);
		activesliders.push_back(true);
	}
	void addSlider(SliderUI<float> x) {
		fsliders.push_back(x);
		activesliders.push_back(true);
	}
	void addSlider(SliderUI<long double> x) {
		dsliders.push_back(x);
		activesliders.push_back(true);
	}
	void RedoPos(std::vector<Vec2f> positions, std::vector<bool> aS) {
		int i = 0;
		activesliders = aS;
		for (SliderUI<long double>& s : dsliders) {
			if (activesliders[i]) s.setPos(positions[i]);
			i++;
		}
		for (SliderUI<float>& s : fsliders) {
			if (activesliders[i]) s.setPos(positions[i]);
			i++;
		}
		for (SliderUI<int>& s : isliders) {
			if (activesliders[i]) s.setPos(positions[i]);
			i++;
		}
	}
};