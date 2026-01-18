"""
Test Texture Generator for NanSu Engine
Generates various test textures for texture mapping verification.
"""

import struct
import os

def create_png(filename, width, height, pixels):
    """Create a PNG file from RGBA pixel data (simplified, no compression)"""
    import zlib

    def png_chunk(chunk_type, data):
        chunk = chunk_type + data
        crc = zlib.crc32(chunk) & 0xffffffff
        return struct.pack('>I', len(data)) + chunk + struct.pack('>I', crc)

    # PNG signature
    signature = b'\x89PNG\r\n\x1a\n'

    # IHDR chunk
    ihdr_data = struct.pack('>IIBBBBB', width, height, 8, 6, 0, 0, 0)  # 8-bit RGBA
    ihdr = png_chunk(b'IHDR', ihdr_data)

    # IDAT chunk (image data)
    raw_data = b''
    for y in range(height):
        raw_data += b'\x00'  # Filter type: None
        for x in range(width):
            idx = (y * width + x) * 4
            raw_data += bytes(pixels[idx:idx+4])

    compressed = zlib.compress(raw_data, 9)
    idat = png_chunk(b'IDAT', compressed)

    # IEND chunk
    iend = png_chunk(b'IEND', b'')

    with open(filename, 'wb') as f:
        f.write(signature + ihdr + idat + iend)

    print(f"Created: {filename} ({width}x{height})")

def generate_checkerboard(width, height, tile_size=32):
    """Generate a checkerboard pattern texture"""
    pixels = []
    for y in range(height):
        for x in range(width):
            is_white = ((x // tile_size) + (y // tile_size)) % 2 == 0
            if is_white:
                pixels.extend([255, 255, 255, 255])  # White
            else:
                pixels.extend([128, 128, 128, 255])  # Gray
    return pixels

def generate_gradient(width, height):
    """Generate a gradient texture (red to blue)"""
    pixels = []
    for y in range(height):
        for x in range(width):
            r = int(255 * x / width)
            g = int(255 * y / height)
            b = int(255 * (1 - x / width))
            pixels.extend([r, g, b, 255])
    return pixels

def generate_uv_test(width, height):
    """Generate UV test pattern (red=U, green=V)"""
    pixels = []
    for y in range(height):
        for x in range(width):
            u = int(255 * x / (width - 1))
            v = int(255 * y / (height - 1))
            pixels.extend([u, v, 0, 255])
    return pixels

def generate_grid(width, height, grid_size=32):
    """Generate a grid pattern for alignment testing"""
    pixels = []
    for y in range(height):
        for x in range(width):
            on_grid = (x % grid_size == 0) or (y % grid_size == 0)
            if on_grid:
                pixels.extend([255, 255, 0, 255])  # Yellow grid lines
            else:
                pixels.extend([32, 32, 64, 255])   # Dark blue background
    return pixels

def generate_alpha_test(width, height):
    """Generate a texture with transparency for alpha blending test"""
    import math
    pixels = []
    center_x = width / 2
    center_y = height / 2
    max_radius = min(center_x, center_y) * 0.8

    for y in range(height):
        for x in range(width):
            dx = x - center_x
            dy = y - center_y
            distance = math.sqrt(dx * dx + dy * dy)

            if distance < max_radius:
                # Inner circle - orange with gradient alpha
                alpha = int(255 * (1 - distance / max_radius))
                pixels.extend([255, 165, 0, alpha])  # Orange with varying alpha
            else:
                # Outer area - fully transparent
                pixels.extend([0, 0, 0, 0])
    return pixels

def main():
    # Output directory
    script_dir = os.path.dirname(os.path.abspath(__file__))
    output_dir = os.path.join(script_dir, '..', 'Assets', 'Textures')
    os.makedirs(output_dir, exist_ok=True)

    size = 256  # Texture size

    # Generate test textures
    textures = [
        ('checkerboard.png', generate_checkerboard(size, size, 32)),
        ('gradient.png', generate_gradient(size, size)),
        ('uv_test.png', generate_uv_test(size, size)),
        ('grid.png', generate_grid(size, size, 32)),
        ('alpha_test.png', generate_alpha_test(size, size)),
    ]

    for filename, pixels in textures:
        filepath = os.path.join(output_dir, filename)
        create_png(filepath, size, size, pixels)

    print(f"\nAll textures generated in: {output_dir}")
    print("\nAvailable textures:")
    for filename, _ in textures:
        print(f"  - {filename}")

if __name__ == '__main__':
    main()
