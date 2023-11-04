/*
 * rendering.cpp
 */

#include "rendering.hpp"
#include <iostream>

namespace gfx {

	// custom deleters for the wrapped pointers
	void SDLw_delete::operator()(SDL_Window *wind) {
		SDL_DestroyWindow(wind);
	}

	void SDLw_delete::operator()(SDL_Renderer *rend) {
		SDL_DestroyRenderer(rend);
	}

	void SDLw_delete::operator()(SDL_Texture *text) {
		SDL_DestroyTexture(text);
	}

	void SDLw_delete::operator()(SDL_Surface *surf) {
		SDL_FreeSurface(surf);
	}

	namespace {
		auto create_window(const char *title, int width, int height, bool fullscreen)
		// returns a unique pointer for SDL_CreateWindow
		{
			return Wind_Ptr(
					SDL_CreateWindow(
							title,
							SDL_WINDOWPOS_UNDEFINED,
							SDL_WINDOWPOS_UNDEFINED,
							width,
							height,
							fullscreen ? SDL_WINDOW_FULLSCREEN_DESKTOP : 0
					), SDLw_delete());
		}

		auto create_renderer(Wind_Ptr &wind, bool accelerated, bool vsync)
		// returns a unique pointer for SDL_CreateRenderer
		{
			if (!accelerated && vsync)
				std::cerr
						<< "Warning: Vsync is not available with software rendering. Setting both will have no effect."
						<< std::endl;

			return Rend_Ptr(
					SDL_CreateRenderer(
							wind.get(),
							-1,
							vsync ? ((accelerated ? SDL_RENDERER_ACCELERATED : SDL_RENDERER_SOFTWARE) |
							         SDL_RENDERER_PRESENTVSYNC) :
							(accelerated ? SDL_RENDERER_ACCELERATED : SDL_RENDERER_SOFTWARE)
					), SDLw_delete());
		}

		void init_graphics()
		// initialize SDL Video
		{
			if (SDL_InitSubSystem(SDL_INIT_VIDEO) != 0) {
				throw std::runtime_error(SDL_GetError());
			}
		}

		void close_graphics()
		// close out the gfx
		{
			SDL_QuitSubSystem(SDL_INIT_VIDEO);
		}

		#ifndef GFX_OMIT_IMAGES

		void init_images(int img_flags = IMG_INIT_PNG)
		// initialize SDL_image
		{
			if (!IMG_Init(img_flags)) {
				throw std::runtime_error(IMG_GetError());
			}
		}

		void close_images()
		// close out image library
		{
			IMG_Quit();
		}

		#endif

		void multimedia_start()
		// start all graphics
		{
			#ifndef GFX_OMIT_IMAGES
			init_images();
			#endif
			init_graphics();
		}

		void multimedia_quit()
		// close out all graphics
		{
			#ifndef GFX_OMIT_IMAGES
			close_images();
			#endif
			close_graphics();
		}
	}

	Render_Context::Render_Context(const std::string &title, int width, int height, bool fullscreen, bool accelerated, bool vsync) {
		multimedia_start();

		m_window = create_window(title.c_str(), width, height, fullscreen);
		m_render = create_renderer(m_window, accelerated, vsync);
	}

	Render_Context::~Render_Context() {
		// cleanup SDL
		multimedia_quit();
	}

	std::pair<int, int> Render_Context::display_size() {
		std::pair<int, int> screen_dims;
		SDL_DisplayMode current_display;

		for (auto i = 0; i < SDL_GetNumVideoDisplays(); ++i) {
			if (SDL_GetCurrentDisplayMode(i, &current_display) != 0) {
				throw std::runtime_error(SDL_GetError());
			} else {
				screen_dims.first = current_display.w;
				screen_dims.second = current_display.h;
			}
		}

		return screen_dims;
	}

	void Render_Context::set_draw_color(int r, int g, int b, int a) {
		SDL_SetRenderDrawColor(m_render.get(), r, g, b, a);
		SDL_SetRenderDrawBlendMode(m_render.get(), SDL_BLENDMODE_MOD);
	}

	void Render_Context::draw_point(int x, int y) {
		SDL_RenderDrawPoint(m_render.get(), x, y);
	}

	void Render_Context::draw_points(const std::vector<Point> &points) {
		SDL_RenderDrawPoints(m_render.get(), points.data(), static_cast<int>(points.size()));
	}

	void Render_Context::draw_line(int x1, int y1, int x2, int y2) {
		SDL_RenderDrawLine(m_render.get(), x1, y1, x2, y2);
	}

	void Render_Context::draw_lines(const std::vector<Point> &points) {
		SDL_RenderDrawLines(m_render.get(), points.data(), static_cast<int>(points.size()));
	}

	void Render_Context::draw_rect(Rect &dest) {
		SDL_RenderDrawRect(m_render.get(), &dest);
	}

	void Render_Context::draw_rects(const std::vector<Rect> &rects) {
		SDL_RenderDrawRects(m_render.get(), rects.data(), static_cast<int>(rects.size()));
	}

	void Render_Context::fill_rect(Rect &dest) {
		SDL_RenderFillRect(m_render.get(), &dest);
	}

	void Render_Context::fill_rects(const std::vector<Rect> &rects) {
		SDL_RenderFillRects(m_render.get(), rects.data(), static_cast<int>(rects.size()));
	}

	#ifndef GFX_OMIT_IMAGES
	Text_Ptr Render_Context::load_image(const std::string& filename) {
		Text_Ptr result;

		Surf_Ptr loaded_surface(IMG_Load(filename.c_str()), SDLw_delete());

		if (loaded_surface == nullptr) {
			throw std::runtime_error(IMG_GetError());
		} else {
			result = Text_Ptr(SDL_CreateTextureFromSurface(m_render.get(), loaded_surface.get()), SDLw_delete());
		}

		return result;
	}
	#endif

	void Render_Context::render_texture(Text_Ptr &texture, Rect *src, Rect *dest, double rot)
	// renders a texture with src and dest rectangles
	{
		SDL_RenderCopyEx(m_render.get(), texture.get(), src, dest, rot, nullptr, SDL_FLIP_NONE);
	}

	void Render_Context::clear() {
		SDL_RenderClear(m_render.get());
	}

	void Render_Context::present() {
		SDL_RenderPresent(m_render.get());
	}
}


