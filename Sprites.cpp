#include <_types/_uint8_t.h>
#include <fstream>
#include <sys/_types/_int16_t.h>
#include <sys/_types/_int8_t.h>
#include <vector>
#include <algorithm>

#include "PPU466.hpp"
#include "Sprites.hpp"
#include "read_write_chunk.hpp"

Sprites::Sprites() { }

Sprites Sprites::init(PPU466 &ppu, std::string const &filename) {
    std::ifstream file(filename, std::ios::binary);

    std::vector< PPU466::Tile > tile_table;
    std::vector< PPU466::Sprite > sprites;
    std::vector< PPU466::Palette > palette_table;
    read_chunk(file, "ttbl", &tile_table);
    read_chunk(file, "sprt", &sprites);
    read_chunk(file, "ptbl", &palette_table);
    std::copy(tile_table.begin(), tile_table.end(), ppu.tile_table.begin());
    std::copy(sprites.begin(), sprites.end(), ppu.sprites.begin());
    std::copy(palette_table.begin(), palette_table.end(), ppu.palette_table.begin());

    Sprites out;
    read_chunk(file, "meta", &out.sprites);
    return out;
}

void Sprites::Sprite::move(PPU466 &ppu, int16_t _x, int16_t _y) {
    int16_t dx = _x - x;
    int16_t dy = _y - y;
    for (uint8_t i = 0; i < num_sprites; ++i) {
        PPU466::Sprite &sprite = ppu.sprites[i + start_sprite_index];
        sprite.x += dx;
        sprite.y += dy;
    }
    x = _x;
    y = _y;
}