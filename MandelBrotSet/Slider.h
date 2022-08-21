#pragma once
#include "functions.h"

template<typename t>
class SliderUI {
private:
	t min, max;
	t* value;
	bool isActive;
	sf::Vector2f pos, barpos;
	float w, barW, h, size;
	t Init;
	sf::Color BarColor, SliderColor, onClickColor;
	sf::Text Text, Textmin, Textmax, Textcurr;
	std::string text, textmin, textmax, textcurr;
	sf::Vertex V1, V2, V3, V4, VB1, VB2, VB3, VB4;
public:
	SliderUI() {}
	SliderUI(const sf::Vector2f& pos, const float& w, const float& h, const float& barW, const sf::Color& SliderColor, const sf::Color& BarColor, const sf::Color& onClickColor, const t& min, const t& max, t& value, const std::optional<t>& Init = {}) {
		this->min = min;
		this->max = max;
		this->Init = Init.value_or(value);
		this->value = &value;
		this->pos = pos;
		this->w = w;
		this->h = h;
		this->barW = barW;
		this->BarColor = BarColor;
		this->SliderColor = SliderColor;
		this->onClickColor = onClickColor;
		this->isActive = false;
		*this->value = this->Init;
		this->barpos = { map((float)this->Init, (float)min, (float)max, (float)(pos.x - w / 2), float(pos.x + w / 2)), (float)pos.y };
		V1.color = SliderColor;
		V2.color = SliderColor;
		V3.color = SliderColor;
		V4.color = SliderColor;
		VB1.color = BarColor;
		VB2.color = BarColor;
		VB3.color = BarColor;
		VB4.color = BarColor;
		V1.position = { pos.x - w / 2, pos.y - h / 4 };
		V2.position = { pos.x + w / 2, pos.y - h / 4 };
		V3.position = { pos.x + w / 2, pos.y + h / 4 };
		V4.position = { pos.x - w / 2, pos.y + h / 4 };
		VB1.position = { barpos.x - barW / 2, barpos.y - h / 2.5f };
		VB2.position = { barpos.x + barW / 2, barpos.y - h / 2.5f };
		VB3.position = { barpos.x + barW / 2, barpos.y + h / 2.5f };
		VB4.position = { barpos.x - barW / 2, barpos.y + h / 2.5f };
	}
	void setText(const std::string& text, const sf::Font& font, const unsigned int& size, const sf::Color& textColor) {		
		this->text = text;
		this->textmin = toStr(min, 3);
		this->textmax = toStr(max, 3);
		this->size = size;
		Text.setFont(font);
		Text.setString(text);
		Text.setCharacterSize(size);
		Text.setOrigin(Text.getLocalBounds().left + Text.getLocalBounds().width / 2.0f, Text.getLocalBounds().top + Text.getLocalBounds().height / 2.0f);
		Text.setFillColor(textColor);
		Textmin.setFont(font);
		Textmax.setFont(font);
		Textmin.setString(textmin);
		Textmax.setString(textmax);
		Textmin.setCharacterSize(size);
		Textmax.setCharacterSize(size);
		Textmin.setOrigin(Textmin.getLocalBounds().left + Textmin.getLocalBounds().width / 2.0f, Textmin.getLocalBounds().top + Textmin.getLocalBounds().height / 2.0f);
		Textmax.setOrigin(Textmax.getLocalBounds().left + Textmax.getLocalBounds().width / 2.0f, Textmax.getLocalBounds().top + Textmax.getLocalBounds().height / 2.0f);
		Textmin.setFillColor(textColor);
		Textmax.setFillColor(textColor);
		Textcurr.setString(toStr(*value, 3));
		Textcurr.setFont(font);
		Textcurr.setCharacterSize(size);
		Textcurr.setOrigin(Textcurr.getLocalBounds().width / 2.0f, Textcurr.getLocalBounds().height / 2.0f);
		Textcurr.setFillColor(textColor);
		setPos();
	}
	void draw(sf::VertexArray& vert, std::vector<sf::Text>& texts) {
		vert.append(V1);
		vert.append(V2);
		vert.append(V3);
		vert.append(V4);
		vert.append(VB1);
		vert.append(VB2);
		vert.append(VB3);
		vert.append(VB4);
		texts.push_back(Text);
		texts.push_back(Textmin);
		texts.push_back(Textmax);
		texts.push_back(Textcurr);
	}
	Vec2f getPos() {
		return pos;
	}
	void setPos(Vec2f _pos) {
		if (pos.y != _pos.y) {
			pos = _pos;
			barpos = { map((float)*this->value, (float)min, (float)max, (float)(pos.x - w / 2), float(pos.x + w / 2)), (float)pos.y };
			V1.position = { pos.x - w / 2, pos.y - h / 4 };
			V2.position = { pos.x + w / 2, pos.y - h / 4 };
			V3.position = { pos.x + w / 2, pos.y + h / 4 };
			V4.position = { pos.x - w / 2, pos.y + h / 4 };
			VB1.position = { barpos.x - barW / 2, barpos.y - h / 2.5f };
			VB2.position = { barpos.x + barW / 2, barpos.y - h / 2.5f };
			VB3.position = { barpos.x + barW / 2, barpos.y + h / 2.5f };
			VB4.position = { barpos.x - barW / 2, barpos.y + h / 2.5f };
			setPos();
		}
	}
	void setPos() {
		Text.setPosition({ pos.x, pos.y - h / 2.5f - size / 2.0f });
		Textmin.setPosition({ pos.x - w / 2.0f - Textmin.getLocalBounds().left - Textmin.getLocalBounds().width / 2.0f - barW / 2.0f, pos.y});
		Textmax.setPosition({ pos.x + w / 2.0f + Textmax.getLocalBounds().left + Textmax.getLocalBounds().width / 2.0f + barW / 2.0f, pos.y});
		Textcurr.setPosition({ pos.x, pos.y + h / 2.5f + size / 2.0f });
	}
	bool dragged(sf::Event::MouseMoveEvent& mouse) {
		if ((mouse.x > VB1.position.x && mouse.x < VB3.position.x && mouse.y > VB1.position.y && mouse.y < VB3.position.y) || isActive) {
			float xval = constrain((float)mouse.x, (float)(pos.x - w / 2), (float)(pos.x + w / 2));
			t val = (t)map(xval, (float)(pos.x - w / 2), (float)(pos.x + w / 2), (float)min, (float)max);
			barpos.x = map((float)val, (float)min, (float)max, (float)(pos.x - w / 2), (float)(pos.x + w / 2));
			VB1.position.x = barpos.x - barW / 2;
			VB2.position.x = barpos.x + barW / 2;
			VB3.position.x = barpos.x + barW / 2;
			VB4.position.x = barpos.x - barW / 2;
			*value = val;
			setActive(true);
			Textcurr.setString(toStr(val, 3));
			Textcurr.setOrigin(Textcurr.getLocalBounds().width / 2.0f, Textcurr.getLocalBounds().height / 2.0f);
			return true;
		}
		else {
			return false;
		}
	}
	void clicked(const Vec2f& mouse) {
		if (mouse.x > V1.position.x && mouse.x < V3.position.x && mouse.y > V1.position.y && mouse.y < V3.position.y) {
			barpos.x = constrain((float)mouse.x, (float)(pos.x - w / 2), (float)(pos.x + w / 2));
			VB1.position.x = barpos.x - barW / 2;
			VB2.position.x = barpos.x + barW / 2;
			VB3.position.x = barpos.x + barW / 2;
			VB4.position.x = barpos.x - barW / 2;
			t val = (t)map((float)barpos.x, (float)(pos.x - w / 2), (float)(pos.x + w / 2), (float)min, (float)max);
			Textcurr.setString(toStr(val, 3));
			*value = val;
		}
	}
	void setActive(const bool& b) {
		isActive = b;
		if (!b) {
			VB1.color = BarColor;
			VB2.color = BarColor;
			VB3.color = BarColor;
			VB4.color = BarColor;
		}
		else {
			VB1.color = onClickColor;
			VB2.color = onClickColor;
			VB3.color = onClickColor;
			VB4.color = onClickColor;
		}
	}
};