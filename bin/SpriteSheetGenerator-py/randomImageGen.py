import random, os, click, sys
from PIL import Image

@click.command()
@click.option('-o', '--output', "outputPath", required=True, type=click.Path())
@click.option('-n', "imageCount", default=1024)
@click.option('-w', "minWidth", default=10)
@click.option('-W', "maxWidth", default=100)
@click.option('-h', "minHeight", default=10)
@click.option('-H', "maxHeight", default=100)
def main(outputPath, imageCount, minWidth, maxWidth, minHeight, maxHeight):
    if os.path.exists(outputPath):
        if (not os.path.isdir(outputPath)):
            print("Error : Output Path isn't a folder")
            sys.exit(1)
    else:
        os.mkdir(outputPath)
    
    if len(os.listdir(outputPath)) > 0:
        print("Error : Output Path isn't empty")
        sys.exit(1)
        
    for x in range(imageCount):
        r = random.randrange(0, 255)
        g = random.randrange(0, 255)
        b = random.randrange(0, 255)
        w = random.randrange(minWidth, maxWidth)
        h = random.randrange(minHeight, maxHeight)
        img = Image.new("RGBA", (w - 2, h - 2), (r, g, b))
        base = Image.new(mode="RGBA", size=(w, h), color=(0, 0, 0))
        base.paste(img, (1, 1))
        base.save(outputPath + "\\" + str(x + 1) + ".png")

if __name__ == '__main__':
    if len(sys.argv) == 1:
        main.main(['--help'])
    else:
        main()