#include "PlayMode.hpp"
#include "data_path.hpp"

//for the GL_ERRORS() macro:
#include "gl_errors.hpp"

//for glm::value_ptr() :
#include <glm/gtc/type_ptr.hpp>

#include <iostream>
#include <random>

PlayMode::PlayMode() {
	sprites = sprites.init(ppu, data_path("../data/sprites.bin"));

	// set background
	for (auto &b : ppu.background) {
		b = sprites.sprites[3].start_sprite_index + sprites.sprites[3].num_sprites;
	}
	
	// move projectiles offscreen
	sprites.sprites[2].move(ppu, -8, -8);
	sprites.sprites[3].move(ppu, -8, -8);
}

PlayMode::~PlayMode() {
}

bool PlayMode::handle_event(SDL_Event const &evt, glm::uvec2 const &window_size) {

	if (evt.type == SDL_EVENT_KEY_DOWN) {
		if (evt.key.key == SDLK_LEFT) {
			left.downs += 1;
			left.pressed = true;
			return true;
		} else if (evt.key.key == SDLK_RIGHT) {
			right.downs += 1;
			right.pressed = true;
			return true;
		} else if (evt.key.key == SDLK_UP) {
			up.downs += 1;
			up.pressed = true;
			return true;
		} else if (evt.key.key == SDLK_DOWN) {
			down.downs += 1;
			down.pressed = true;
			return true;
		}
		
		if (evt.key.key == SDLK_W) {
			w.downs += 1;
			w.pressed = true;
			return true;
		} else if (evt.key.key == SDLK_A) {
			a.downs += 1;
			a.pressed = true;
			return true;
		} else if (evt.key.key == SDLK_S) {
			s.downs += 1;
			s.pressed = true;
			return true;
		} else if (evt.key.key == SDLK_D) {
			d.downs += 1;
			d.pressed = true;
			return true;
		}
	} else if (evt.type == SDL_EVENT_KEY_UP) {
		if (evt.key.key == SDLK_LEFT) {
			left.pressed = false;
			return true;
		} else if (evt.key.key == SDLK_RIGHT) {
			right.pressed = false;
			return true;
		} else if (evt.key.key == SDLK_UP) {
			up.pressed = false;
			return true;
		} else if (evt.key.key == SDLK_DOWN) {
			down.pressed = false;
			return true;
		}

		if (evt.key.key == SDLK_W) {
			w.pressed = false;
			return true;
		} else if (evt.key.key == SDLK_A) {
			a.pressed = false;
			return true;
		} else if (evt.key.key == SDLK_S) {
			s.pressed = false;
			return true;
		} else if (evt.key.key == SDLK_D) {
			d.pressed = false;
			return true;
		}
	}

	return false;
}

void PlayMode::update(float elapsed) {

	constexpr float PlayerSpeed = 50.0f;
	if (left.pressed) player2_at.x -= PlayerSpeed * elapsed;
	if (right.pressed) player2_at.x += PlayerSpeed * elapsed;
	if (down.pressed) player2_at.y -= PlayerSpeed * elapsed;
	if (up.pressed) player2_at.y += PlayerSpeed * elapsed;

	if (a.pressed) player1_at.x -= PlayerSpeed * elapsed;
	if (d.pressed) player1_at.x += PlayerSpeed * elapsed;
	if (s.pressed) player1_at.y -= PlayerSpeed * elapsed;
	if (w.pressed) player1_at.y += PlayerSpeed * elapsed;

	//reset button press counters:
	left.downs = 0;
	right.downs = 0;
	up.downs = 0;
	down.downs = 0;
	a.downs = 0;
	d.downs = 0;
	w.downs = 0;
	s.downs = 0;
}

void PlayMode::draw(glm::uvec2 const &drawable_size) {
	//--- set ppu state based on game state ---

	//player sprite:
	sprites.sprites[0].move(ppu, player1_at.x, player1_at.y);
	sprites.sprites[1].move(ppu, player2_at.x, player2_at.y);
	// ppu.sprites[0].x = int8_t(player_at.x);
	// ppu.sprites[0].y = int8_t(player_at.y);
	// ppu.sprites[0].index = 32;
	// ppu.sprites[0].attributes = 7;

	//some other misc sprites:
	// for (uint32_t i = 1; i < 63; ++i) {
	// 	float amt = (i + 2.0f * background_fade) / 62.0f;
	// 	ppu.sprites[i].x = int8_t(0.5f * float(PPU466::ScreenWidth) + std::cos( 2.0f * M_PI * amt * 5.0f + 0.01f * player_at.x) * 0.4f * float(PPU466::ScreenWidth));
	// 	ppu.sprites[i].y = int8_t(0.5f * float(PPU466::ScreenHeight) + std::sin( 2.0f * M_PI * amt * 3.0f + 0.01f * player_at.y) * 0.4f * float(PPU466::ScreenWidth));
	// 	ppu.sprites[i].index = 32;
	// 	ppu.sprites[i].attributes = 6;
	// 	if (i % 2) ppu.sprites[i].attributes |= 0x80; //'behind' bit
	// }

	//--- actually draw ---
	ppu.draw(drawable_size);
}
