#pragma once

#include <string>
#include <vector>

#include "PPU466.hpp"

struct Sprites {
    Sprites();

    Sprites init(PPU466 &ppu, std::string const &filename);

    struct Sprite {
        uint8_t x;
        uint8_t y;
        uint8_t start_sprite_index;
        uint8_t num_sprites;

        void move(PPU466 &ppu, int16_t x, int16_t y);
    };
    std::vector< Sprite > sprites;
};