# How to Add Textures

Your game now supports textures! Here's what you need to do:

## 1. Download stb_image.h

STB Image is a single-header image loading library.

- Download from: https://raw.githubusercontent.com/nothings/stb/master/stb_image.h
- Place it in: `libs/stb/stb_image.h`
- Or create a `libs/stb/` folder and put `stb_image.h` there

## 2. Add Texture Images

Place your texture images in the `assets/textures/` folder:

- `ground.png` - For the floor/ground texture
- `box.png` - For the red platform/box texture

**Supported formats**: PNG, JPG, BMP, TGA

If texture files are not found, the game will use white textures as fallback (so it won't crash).

## 3. Update CMakeLists.txt

Add the stb include directory to your CMakeLists.txt:

```cmake
include_directories(${CMAKE_SOURCE_DIR}/libs/stb)
```

After line 9 where other includes are.

## 4. Rebuild

- Delete `build/` folder
- Run CMake again
- Rebuild the project

## Example Textures

You can:
- Create simple textures in Paint or GIMP
- Download free textures from sites like:
  - OpenGameArt.org
  - Textures.com
  - ambientCG.com
- Use procedurally generated textures

## Tips

- Textures should be power of 2 dimensions (256x256, 512x512, 1024x1024) for best compatibility
- PNG with transparency works great
- The texture will repeat on large surfaces automatically

That's it! Your ground and box will now be textured instead of solid colors.
