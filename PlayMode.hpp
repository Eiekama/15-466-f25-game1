#include "PPU466.hpp"
#include "Sprites.hpp"
#include "Mode.hpp"

#include <glm/glm.hpp>

#include <vector>
#include <deque>

struct PlayMode : Mode {
	PlayMode();
	virtual ~PlayMode();

	//functions called by main loop:
	virtual bool handle_event(SDL_Event const &, glm::uvec2 const &window_size) override;
	virtual void update(float elapsed) override;
	virtual void draw(glm::uvec2 const &drawable_size) override;

	//----- game state -----

	//input tracking:
	struct Button {
		uint8_t downs = 0;
		uint8_t pressed = 0;
	} left, right, down, up,
	  w, a, s, d,
	  z, x,
	  n, m;

	//some weird background animation:
	float background_fade = 0.0f;

	//player position:
	glm::vec2 player1_at = glm::vec2(50, 120);
	glm::vec2 player2_at = glm::vec2(150, 120);

	//----- drawing handled by PPU466 -----
	
	Sprites sprites;

	PPU466 ppu;
};
