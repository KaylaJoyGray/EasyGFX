#ifndef RENDERING_H
#define RENDERING_H

#include <SDL2/SDL.h>

#ifndef GFX_OMIT_IMAGES
#include <SDL2/SDL_image.h>
#endif

#include <string>
#include <memory>
#include <vector>

namespace gfx {

	struct SDLw_delete {
		void operator()(SDL_Window *wind);

		void operator()(SDL_Renderer *rend);

		void operator()(SDL_Texture *text);

		void operator()(SDL_Surface *surf);
	};

	using Wind_Ptr = std::unique_ptr<SDL_Window, SDLw_delete>;
	using Rend_Ptr = std::shared_ptr<SDL_Renderer>;
	using Text_Ptr = std::shared_ptr<SDL_Texture>;
	using Surf_Ptr = std::shared_ptr<SDL_Surface>;

	using Point = SDL_Point;
	using Rect = SDL_Rect;

	using BlendMode = SDL_BlendMode;
	constexpr BlendMode Blend_None = SDL_BLENDMODE_NONE;
	constexpr BlendMode Blend_Alpha = SDL_BLENDMODE_BLEND;
	constexpr BlendMode Blend_Add = SDL_BLENDMODE_ADD;
	constexpr BlendMode Blend_Mod = SDL_BLENDMODE_MOD;
	constexpr BlendMode Blend_Mul = SDL_BLENDMODE_MUL;

	class Render_Context {
		private:
			Wind_Ptr m_window = nullptr;
			Rend_Ptr m_render = nullptr;

			BlendMode m_mode = Blend_Alpha;
		public:
			Render_Context(const std::string &title, int width, int height, bool fullscreen = false, bool vsync = true,
			               bool accelerated = true);
			~Render_Context();

			// delete default, copy and move constructors/operators for singleton-like behavior
			Render_Context() = delete;
			Render_Context(const Render_Context&) = delete;
			Render_Context(Render_Context&&) = delete;
			Render_Context& operator=(const Render_Context&) = delete;
			Render_Context& operator=(Render_Context&&) = delete;

			std::pair<int, int> display_size();
			std::pair<int, int> canvas_size();

			void set_draw_color(int r, int g, int b, int a = 255);
			void set_draw_blend_mode(BlendMode mode);

			void draw_point(int x, int y);
			void draw_points(const std::vector<Point> &points);
			void draw_line(int x1, int y1, int x2, int y2);
			void draw_lines(const std::vector<Point> &points);
			void draw_rect(Rect &dest);
			void draw_rects(const std::vector<Rect> &rects);
			void fill_rect(Rect &dest);
			void fill_rects(const std::vector<Rect> &rects);

			#ifndef GFX_OMIT_IMAGE
			Text_Ptr load_image(const std::string& filename);
			#endif
			void set_texture_blend_mode(Text_Ptr &texture, BlendMode mode);
			void render_texture(Text_Ptr &texture, Rect *src = nullptr, Rect *dest = nullptr, double rot = 0);

			void clear();
			void present();
	};

}

#endif
