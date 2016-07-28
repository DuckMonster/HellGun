#pragma comment(linker, "/SUBSYSTEM:windows /ENTRY:mainCRTStartup")
//#define FULLSCREEN

#include <SFML\Window.hpp>

#include <sstream>

#include <gl\glew.h>

#include <GLT\GLT.h>
#include <GLT\Mesh.hpp>
#include <GLT\Camera.hpp>
#include <GLT\Time.hpp>
#include <GLT\FrameBuffer.hpp>

#include "GameState.hpp"
#include "Game.hpp"

using namespace glt;
using namespace sf;
 
#define GAME_WIDTH 1920 / 4
#define GAME_HEIGHT 1024 / 4
//#define GAME_WIDTH 1920
//#define GAME_HEIGHT 1024
int main(int argc, char* argv[]) {
	ContextSettings settings;
	settings.depthBits = 24;
	settings.stencilBits = 8;

	VideoMode fullscreenMode = VideoMode().getDesktopMode();
	VideoMode windowedMode = VideoMode(1024, 768, 32);

	bool fullscreen = false;

	for (int i = 0; i < argc; i++)
		if (strcmp(argv[i], "-fullscreen") == 0)
			fullscreen = true;

#ifdef FULLSCREEN
	fullscreen = true;
#endif

	Window window(!fullscreen ? windowedMode : fullscreenMode, "GLT Game", !fullscreen ? Style::Close : Style::None, settings);
	window.setMouseCursorVisible(false);
	window.setVerticalSyncEnabled(false);
	//window.setFramerateLimit(200);

	gltInit();

	FrameBuffer::clientWidth = window.getSize().x;
	FrameBuffer::clientHeight = window.getSize().y;

	FrameBuffer buffer(GAME_WIDTH, GAME_HEIGHT);

	GameState::screenSize = vec2(window.getSize().x, window.getSize().y);
	Game game;

	auto oldMousePos = Mouse::getPosition();

	while (window.isOpen()) {
		//Poll event
		Event event;
		while (window.pollEvent(event)) {
			switch (event.type) {
			case Event::Closed:
				window.close();
				break;

			case Event::KeyPressed:
				if (event.key.code == sf::Keyboard::Escape)
					window.close();

				if (event.key.code == sf::Keyboard::F4 && event.key.alt)
					window.close();

				if (event.key.code == sf::Keyboard::Unknown)
					break;

				GameState::currentInput.keyboard.keys[event.key.code] = true;
				break;

			case Event::KeyReleased:
				if (event.key.code == sf::Keyboard::Unknown)
					break;

				GameState::currentInput.keyboard.keys[event.key.code] = false;
				break;

			case Event::MouseButtonPressed:
				GameState::currentInput.mouse.buttons[event.mouseButton.button] = true;
				break;

			case Event::MouseButtonReleased:
				GameState::currentInput.mouse.buttons[event.mouseButton.button] = false;
				break;

			case Event::MouseLeft: {
				Vector2i m = Mouse::getPosition();
				Vector2i pos = window.getPosition();
				Vector2u size = window.getSize();

				Mouse::setPosition(
					Vector2i(
					clamp(m.x, pos.x, pos.x + (int)size.x),
					clamp(m.y, pos.y, pos.y + (int)size.y)));
			}
				break;

			case Event::JoystickButtonPressed:
				GameState::currentInput.joystick[event.joystickButton.joystickId].buttons[event.joystickButton.button] = true;
				break;

			case Event::JoystickButtonReleased:
				GameState::currentInput.joystick[event.joystickButton.joystickId].buttons[event.joystickButton.button] = false;
				break;

			case Event::TextEntered:
				int key = event.text.unicode;

				if (key >= '0' && key <= '9' ||
					key >= 'a' && key <= 'z' ||
					key >= 'A' && key <= 'Z' ||
					key == '_' ||
					key == 32)
					GameState::keyboardString += key;

				if (key == 8 && GameState::keyboardString.size() > 0)
					GameState::keyboardString.erase(GameState::keyboardString.size() - 1);

				if (!game.nameEntered)
					game.setName(GameState::keyboardString);
			}
		}

		//Update joystick axis
		for (int i = 0; i < 4; i++) {
			for (int a = 0; a < Joystick::AxisCount; a++)
				GameState::currentInput.joystick[i].axes[a] = Joystick::getAxisPosition(i, (Joystick::Axis)a) / 100.f;
		}

		//Update camera
		Camera::active->screenSize = vec2(window.getSize().x, window.getSize().y);

		//Update mouse
		auto mpos = sf::Mouse::getPosition(window);
		vec2 mouseScreen = vec2(mpos.x, mpos.y);
		vec2 mouseWorld = vec2(game.camera.screenToWorld(mouseScreen).overlapPlane(vec3(0.f), vec3(0.f, 0.f, 1.f)));

		GameState::currentInput.mouse.screen = mouseScreen;
		GameState::currentInput.mouse.world = mouseWorld;

		//Delta
		GameState::deltaTime = glt::Time::delta();

		std::ostringstream buff;
		buff << (int)(1.f / GameState::deltaTime);

		window.setTitle(buff.str());

		buffer.bind();
		//Clear window
		glClearColor(0.1f, 0.1f, 0.1f, 1.f);
		glClear(GL_COLOR_BUFFER_BIT);

		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		game.logic();
		game.draw();

		glDrawArrays(GL_TRIANGLES, 0, 0);

		FrameBuffer::release();

		glClear(GL_COLOR_BUFFER_BIT);
		buffer.display();

		window.display();

		oldMousePos = Mouse::getPosition();

		//sf::sleep(sf::milliseconds(10));
		
	}
}