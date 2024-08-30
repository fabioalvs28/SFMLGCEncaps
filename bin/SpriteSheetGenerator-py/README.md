## Spritesheet packer

Tools that pack multiples sprites into 1 larger texture for more efficient draw call
## Installation for Python script

Clone the repo and install requirements:

```shell
pip install -r requirements.txt
```

## Usage

#### randomImageGen.py
Tool for generating lot of random images (1 random RGB color with a 1px black outline) for testing spriteSheetGenerator.py:

Options:
* -o, --output: path where to images will be generated [Required] (The folder need to be empty)
* -n: Number of images to be generated [Optional] (Default: 1024)
* -w: minimum width for each images [Optional] (Default: 10)
* -W: maximum width for each images [Optional] (default: 100)
* -h: minimum height for each images [Optional] (default: 10)
* -H: maximum height for each images [Optional] (default: 100)

Exemple Usage:
```py
python randomImageGen.py -o randomImage -n 1024 -w 10 -W 100 -h 10 -H 100
```
Generate 1024 images in the `randomImage` folder of size between 10x10 and 100x100

#### spriteSheetGenerator
Tool for packing lot of small images into 1 or more larger textures, the tool also generate a json file to know where each sprites is placed:

Options:
* -i, --input: path where to images to be packed are located [Required]
* -o, --output: path where to textures will be generated [Required] CAUTION : The folder will be deleted and regenerated
* -p, --padding: padding around each images [Optional] (default: 0)
* -R: allow 90° rotations of images [Optional] (default: false)
* -F: try to fills previous row with new images [Optional] (default: false)
* -T: Trim images to removed additional transparent pixels

Exemple Usage:
```cmd
spriteSheetGenerator.exe -i randomImage -o output -p 1 -R -F -T
```
Pack the 1024 images from the `randomImage` folder and generate Textures in the `output` folder with a padding of 1px, while allowing rotations, previous row filling and image trimming.

#### spritSheetData.json
This file contain informations about each textures and sprites:

* totalImageCount: contain the number of sprites packed in all the textures
* totalTextureCount: contain the number of textures created
* textures: array that contain all the textures data, each texture is an element of the array
  * textureName: name of this textures file
  * textureSize: size of the textures, always a power of 2, maximum 2048x2048
  * imageCount: number of images in this specific texture
  * images: array that contain all the sprites data of this textures, each sprite is an element of the array
    * filename: origin name of the sprite (without extension)
    * x: x position of the upper left corner on the texture
    * y: y position of the upper left corner on the texture
    * w: width of the sprite
    * h: height of the sprite
    * rotated: if the sprite was rotated 90° clockwise before being packed
    * cropped: if the sprite was cropped before being packed
    * crop_x: if cropped, the x position at which the crop was done (upper left corner), else 0
    * crop_y: if cropped, the x position at which the crop was done (upper left corner), else 0
    * original_w: if cropped, the original width of the image before the crop and rotation was done (upper left corner), else 0
    * original_h: if cropped, the original height of the image before the crop and rotation was done (upper left corner), else 0
* spriteIndex: index to find on which textures a specific sprite is located, the sprite original name return a tuple (t, i), T is the textures index, i is the image index in this textures, a if a sprite name return (3, 46), then this sprite is located on the 4th textures and is the 47th placed on this texture, to get this sprite data jsonQuery textures[3][46]

#### spritSheetData.ssdg
This Binary file contain informations about each textures and sprites:

A ImHex pattern is available for debuging (metadata.hexpat)

The saved data is the same as the .json, but it doesn't have the spriteIndex