#include <iostream>
#include <thread>
#include <chrono>
#include "game.hpp"

#include "ftxui/component/component.hpp"
#include "ftxui/component/screen_interactive.hpp"
#include "ftxui/dom/elements.hpp"
#include "ftxui/screen/canvas.hpp"

using namespace ftxui;

int main() {
	auto screen = ScreenInteractive::TerminalOutput();
	BirdGame game;
	
	auto renderer = Renderer([&] {
		std::lock_guard<std::mutex> lock(game.game_mutex);

		auto c = Canvas(game.width, game.height);

		for (int y = 0; y < game.height; ++y) {
			if (y < game.bar_y_up || y > game.bar_y_down) {
				c.DrawPoint(game.bar_x, y, true);
			}
		}

		if (!game.is_game_over) {
			c.DrawPoint(int(game.bird_x), int(game.bird_y), true);
		}

		return vbox({
			hbox({
				text(" 🐤 Flying Bird C++ PRO ") | bold | color(Color::Yellow),
				filler(),
				text("🏆 Max: " + std::to_string(game.high_score)) | color(Color::Gray),
				separatorText(""),
				text("✨ Score: " + std::to_string(game.score)) | bold | color(Color::Green)
			}),
			separatorDouble(),
			canvas(std::move(c)) | borderStyled(BorderStyle::ROUNDED) | color(Color::Cyan),
			separator(),
			game.is_game_over ? 
			vbox({
				text(" 💀 GAME OVER ") | blink | center | color(Color::Red) | bold,
				text(" Press [R] to Try Again / [Q] to Quit ") | center | color(Color::Gray)
			}) :
			text(" ⌨️  Controls: [Space] to Fly Up | [Q] to Quit ") | center | color(Color::DarkGreen)
		});
	});

	auto component = CatchEvent(renderer, [&](Event event) {
		if (event == Event::Character('q') || event == Event::Character('Q')) {
			screen.ExitLoopClosure()();
			return true;
		}
		if (event == Event::Character('r') || event == Event::Character('R')) {
			game.initGame();
			return true;
		}
		if (event == Event::(' ')) { 
			game.checkInput();
			return true;
		}
		return false;
	});
	
	std::thread physics_thread([&] {
		while (true) {
			std::this_thread::sleep_for(std::chrono::milliseconds(75));
			if (!game.is_game_over) {
				game.updatePhysics();
				screen.PostEvent(Event::Custom);
			}
		}
	});
	physics_thread.detach();
	
	screen.Loop(component);
	return 0;
}
