/**************************************
 * Example file - C++ 17 and up
 * Created by Kayla 11/3/23
 ***************************************/

#include "src/rendering.hpp"
#include <iostream>
#include <csignal>

int main(int argc, char** argv) {

	try {
		gfx::Render_Context my_render("Hello World", 600, 400, false);

		my_render.set_draw_color(255, 255, 255);
		my_render.clear();

		auto [width, height] = my_render.canvas_size();

		my_render.set_draw_color(0, 0, 0);
		my_render.draw_line(0, 0, width, height);
		my_render.draw_line(width, 0, 0, height);
		my_render.present();

		sleep(5);
	} catch (const std::exception& e) {
		std::cout << "Oh no! Failed to start the renderer." << std::endl;
		return -1;
	}

	return 0;
}