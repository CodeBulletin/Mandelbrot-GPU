#include "set.h"
#include "variables.h"
#include <math.h>

void keyboardevent(sf::RenderWindow&, sf::Event&, Set&, variables&, UI&, sf::Texture&, int&);
std::string int_to_tex(int a) {
	return std::to_string(a + 1) + ".png";
}

int main() {
	int width, height;
	std::string name = "Complex Set";
	std::vector<String> colstrings = { "Basic", "Potential", "Normal Map" };
	int tex = 0;
	sf::RenderWindow window;
	sf::VideoMode vid = sf::VideoMode::getDesktopMode();
	width = vid.width;
	height = vid.height;
	sf::ContextSettings settings;
	settings.antialiasingLevel = 8;
	window.create(vid, name, sf::Style::Fullscreen, settings);
	window.setVerticalSyncEnabled(true);
	sf::Event event;
	sf::Texture texture;
	if (!texture.loadFromFile(int_to_tex(tex)))
		return -10;
	sf::Font font;
	font.loadFromFile("arial.ttf");
	Set set = Set(0, 0, "set.frag", Vec2d(0, 0), texture, Vec2d(1920, 1080), Vec2d(0, 0), Vec2d(0.285, 0.01));
	variables v = variables(0, 2);
	UI mainui = UI(Vec2f(50.0, 50.0), Vec2f(350.0, 1030.0), true, sf::Color(255, 255, 255, 100), font);
	SliderUI<long double> s1 = SliderUI<long double>({ 200.0f, 100.0f }, 200, 30, 20, sf::Color::Red, sf::Color::Green, sf::Color::Blue, 2.0, 10.0, set.N, 2.0);
	s1.setText("power", font, 20, sf::Color::Black);
	SliderUI<long double> s2 = SliderUI<long double>({ 200.0, 300.0 }, 200, 30, 20, sf::Color::Red, sf::Color::Green, sf::Color::Blue, (long double)-1.25, (long double)1.25, set.complex_number.x, (long double)0.0);
	s2.setText("Real Value(julia set only)", font, 20, sf::Color::Black);
	SliderUI<long double> s3 = SliderUI<long double>({ 200.0, 400.0 }, 200, 30, 20, sf::Color::Red, sf::Color::Green, sf::Color::Blue, (long double)-1.25, (long double)1.25, set.complex_number.y, (long double)0.0);
	s3.setText("Complex Value(julia set only)", font, 20, sf::Color::Black);
	SliderUI<float> s4 = SliderUI<float>({ 200.0, 500.0 }, 200, 30, 20, sf::Color::Red, sf::Color::Green, sf::Color::Blue, 0.0f, 2.0f, set.Height, 1.5f);
	s4.setText("Height(only for normal map)", font, 20, sf::Color::Black);
	SliderUI<float> s5 = SliderUI<float>({ 200.0, 600.0 }, 200, 30, 20, sf::Color::Red, sf::Color::Green, sf::Color::Blue, 0.0f, 2*3.14159f, set.LightAngle, 0.0f);
	s5.setText("Light angle(only for normal map)", font, 20, sf::Color::Black);
	SliderUI<float> s6 = SliderUI<float>({ 200.0, 600.0 }, 200, 30, 20, sf::Color::Red, sf::Color::Green, sf::Color::Blue, 0.0f, 255.0f, set.Red, 255.0f);
	s6.setText("R", font, 20, sf::Color::Black);
	SliderUI<float> s7 = SliderUI<float>({ 200.0, 600.0 }, 200, 30, 20, sf::Color::Red, sf::Color::Green, sf::Color::Blue, 0.0f, 255.0f, set.Green, 255.0f);
	s7.setText("G", font, 20, sf::Color::Black);\
	SliderUI<float> s8 = SliderUI<float>({ 200.0, 600.0 }, 200, 30, 20, sf::Color::Red, sf::Color::Green, sf::Color::Blue, 0.0f, 255.0f, set.Blue, 255.0f);
	s8.setText("B", font, 20, sf::Color::Black);
	SliderUI<int> s9 = SliderUI<int>({ 200.0f, 200.0f }, 200, 30, 20, sf::Color::Red, sf::Color::Green, sf::Color::Blue, 0, 300, set.shift, 0);
	s9.setText("Color Shift", font, 20, sf::Color::Black);
	mainui.addSlider(s1);
	mainui.addSlider(s2);
	mainui.addSlider(s3);
	mainui.addSlider(s4);
	mainui.addSlider(s5);
	mainui.addSlider(s6);
	mainui.addSlider(s7);
	mainui.addSlider(s8);
	mainui.addSlider(s9);
	Vec2f mpos, lpos;
	bool SliderActive = false;
	mainui.RedoPos({ {200, 100}, {200, 200}, {200, 300}, {}, {}, {}, {}, {}, {200, 400}, }, { true, true, true, false, false, false, false, false, true });
	set.setValues(mainui);
	set.setValues(mainui);
	while (window.isOpen()) {
		while (window.pollEvent(event)) {
			switch (event.type)
			{
			case sf::Event::KeyPressed:
				keyboardevent(window, event, set, v, mainui, texture, tex);
				set.setValues(mainui);
				break;
			case sf::Event::MouseButtonReleased:
				v.hold = false;
				v.uihold = false;
				break;
			case sf::Event::MouseWheelScrolled:
				if (event.mouseWheel.x > 0) {
					auto mouse = sf::Mouse::getPosition();
					set.setZoomAtMouse(v.increaseI(), v.n, Vec2d(mouse.x, mouse.y));
				} 
				else if (event.mouseWheel.x < 0) {
					auto mouse = sf::Mouse::getPosition();
					set.setZoomAtMouse(v.decreaseI(), v.n, Vec2d(mouse.x, mouse.y));
				}
				break;
			case sf::Event::MouseMoved:
				if (!v.hold) {
					if (sf::Mouse::isButtonPressed(sf::Mouse::Left)) {
						if (mainui.dragged(event.mouseMove)) {
							SliderActive = true;
							set.setValues(mainui);
						}
					}
					else if (SliderActive) {
						mainui.updatedragged();
						set.setValues(mainui);
						SliderActive = false;
					}
					break;
				}
			default:
				break;
			}
			if (sf::Mouse::isButtonPressed(sf::Mouse::Left)) {
				auto mouse = sf::Mouse::getPosition();
				if (!mainui.collides(Vec2f(mouse.x, mouse.y)) && !v.uihold || v.hold) {
					mpos = Vec2f(mouse.x, -mouse.y);
					if (v.hold) {
						Vec2f diff = mpos - lpos;
						float mag = magnitude(diff);
						diff /= mag;
						if (mag >= 0.0001) {
							set.setLoaction(diff.x, diff.y, mag / 54.0);
						}
					}
					lpos = mpos;
					v.hold = true;
				}
				else {
					v.hold = false;
					if (!v.uihold) {
						v.uihold = true;
						mainui.clicked({ (float)mouse.x, (float)mouse.y });
						set.setValues(mainui);
					}
				}
			}
		}
		window.clear(sf::Color::Black);
		set.draw(window);
		window.draw(mainui.draw(set.SetTypes[set.type], colstrings[set.cmod]));
		for (sf::Text &t : mainui.getText()) {
			window.draw(t);
		}
		window.display();
	}
	return 0;
}

void keyboardevent(sf::RenderWindow& window, sf::Event& event, Set& set, variables& v, UI& mainui, sf::Texture& texture, int& tex) {
	if (event.key.code == sf::Keyboard::Escape)
		window.close();
	else if (event.key.code == sf::Keyboard::W)
		set.setLoaction(0, 1, v.speed);
	else if (event.key.code == sf::Keyboard::A)
		set.setLoaction(-1, 0, v.speed);
	else if (event.key.code == sf::Keyboard::S)
		set.setLoaction(0, -1, v.speed);
	else if (event.key.code == sf::Keyboard::D)
		set.setLoaction(1, 0, v.speed);
	else if (event.key.code == sf::Keyboard::Z)
		v.IncreaseSpeed();
	else if (event.key.code == sf::Keyboard::X)
		v.DecreaseSpeed();
	else if (event.key.code == sf::Keyboard::Up)
		set.setZoom(v.increaseI(), v.n);
	else if (event.key.code == sf::Keyboard::Down)
		set.setZoom(v.decreaseI(), v.n);
	else if (event.key.code == sf::Keyboard::Right)
		v.IncreaseN();
	else if (event.key.code == sf::Keyboard::Left)
		v.DecreaseN();
	else if (event.key.code == sf::Keyboard::Period)
		set.setIterations(v.increaseI2() * 500);
	else if (event.key.code == sf::Keyboard::Comma)
		set.setIterations(v.decreaseI2() * 500);
	else if (event.key.code == sf::Keyboard::Hyphen)
		mainui.setActive(!mainui.getActive());
	else if (event.key.code == sf::Keyboard::Num7) {
		tex = constrain(tex - 1, 0, 3);
		texture.loadFromFile(int_to_tex(tex));
		set.setPalette(texture);
	}
	else if (event.key.code == sf::Keyboard::Num8) {
		tex = constrain(tex + 1, 0, 3);
		texture.loadFromFile(int_to_tex(tex));
		set.setPalette(texture);
	}
	else if (event.key.code == sf::Keyboard::Num9)
		set.setCMod(constrain(set.cmod - 1, 0, 2));
	else if (event.key.code == sf::Keyboard::Num0)
		set.setCMod(constrain(set.cmod + 1, 0, 2));
	else if (event.key.code == sf::Keyboard::Num1)
		set.type = 0;
	else if (event.key.code == sf::Keyboard::Num2)
		set.type = 1;
}