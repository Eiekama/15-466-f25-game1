import json, struct
from PIL import Image

def get_color_index(pixel):
    '''
    pixel is a (R,G,B,A) tuple
    returns index (0-3) into palette
    '''
    # using R channel arbitrarily
    r = pixel[0]
    if r < 64:
        return 0
    elif r < 128:
        return 1
    elif r < 192:
        return 2
    else:
        return 3

def process_sprite(sprite_data, start_tile_index):
    '''
    sprite_data is a dictionary with keys
    ['name', 'file', 'palette_indices']
    '''
    img = Image.open(f"sprites/{sprite_data['file']}")

    width, height = img.size
    rows, cols = height // 8, width // 8 # note this will chop off any excess pixels
    tiles = []
    sprites = []
    idx = start_tile_index
    for n in range(rows):
        for m in range(cols):
            palette_index = sprite_data['palette_indices'][n*cols + m]
            if palette_index < 0: continue
            bit0, bit1 = [], []
            for i in range(8):
                b0, b1 = 0, 0
                for j in range(8):
                    pixel_coord = (m*8 + (7-j), n*8 + i)
                    pixel = img.getpixel(pixel_coord)
                    color_index = get_color_index(pixel)
                    b0 = (b0 << 1) | (color_index & 1)
                    b1 = (b1 << 1) | ((color_index >> 1) & 1)
                bit0.insert(0, b0)
                bit1.insert(0, b1)
            tiles.append(struct.pack('16B', *(bit0 + bit1)))
            x = 8 * m
            y = height - 8 * (n + 1)
            tile_index = idx
            idx += 1
            sprites.append(struct.pack('4B', x, y, tile_index, palette_index))
    metadata = struct.pack('4B', 0, 0, start_tile_index, idx - start_tile_index)    
    return tiles, sprites, metadata

def process_palette(filepath="palette_table.png"):
    img = Image.open(filepath)
    w, h = img.size
    if (h > 8 or w != 4): raise ValueError
    palette_table = []
    for i in range(h):
        palette = []
        for j in range(w):
            r, g, b, a = img.getpixel((j, i))
            palette.append(struct.pack('4B', r, g, b, a))
        palette_table.extend(palette)
    return palette_table

def process_sprites(filepath="sprites/sprites_metadata.json"):
    tile_index = 0
    tile_table, sprite_table, metadata = [], [], []

    with open(filepath, "r") as f:
        sprites_data = json.load(f)
        for sprite_data in sprites_data:
            tiles, sprites, meta = process_sprite(sprite_data, tile_index)
            tile_table.extend(tiles)
            sprite_table.extend(sprites)
            metadata.append(meta)
            tile_index += meta[-1]
            assert tile_index == len(tile_table)
    
    # plain square tile for background
    tile_table.append(struct.pack('16B', *[0 for _ in range(16)]))

    return tile_table, sprite_table, metadata
        

def export_binary(tile_table, sprite_table, metadata, palette_table, outfile="sprites.bin"):
    blob = open(outfile, 'wb')

    blob.write(struct.pack('4s', b'ttbl'))
    tile_table = b''.join(tile_table)
    blob.write(struct.pack('I', len(tile_table)))
    blob.write(tile_table)

    blob.write(struct.pack('4s', b'sprt'))
    sprite_table = b''.join(sprite_table)
    blob.write(struct.pack('I', len(sprite_table)))
    blob.write(sprite_table)

    blob.write(struct.pack('4s', b'ptbl'))
    palette_table = b''.join(palette_table)
    blob.write(struct.pack('I', len(palette_table)))
    blob.write(palette_table)

    blob.write(struct.pack('4s', b'meta'))
    metadata = b''.join(metadata)
    blob.write(struct.pack('I', len(metadata)))
    blob.write(metadata)

    blob.close()


if __name__ == "__main__":
    tile_table, sprite_table, metadata = process_sprites()
    palette_table = process_palette()
    export_binary(tile_table, sprite_table, metadata, palette_table)
    # with open("sprites/sprites_metadata.json", "r") as f:
    #     sprites_data = json.load(f);
        # tiles, sprites, meta = process_sprite(sprites_data[0], 0)
        # print(tiles)
        # print(sprites)
        # print(meta)
        # print(palette)



# for each sprite, want to
#   load tiles into contiguous region of tile_table
#   load hardware sprites into contiguous region of sprites:
#       keep track of tile index
#       rmb to save palette index for each tile
#       initialize x, y positions for each hardware sprite
#   save sprite start index and num sprites as metadata
# save palette data from png