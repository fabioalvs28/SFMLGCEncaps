import os, sys, json, click, re, math
from PIL import Image

def sorted_alphanumeric(data):
    convert = lambda text: int(text) if text.isdigit() else text.lower()
    alphanum_key = lambda key: [ convert(c) for c in re.split('([0-9]+)', key) ] 
    return sorted(data, key=alphanum_key)


def getSmallestSheetSize(remainingSize):
    sqrtTotal = math.sqrt(remainingSize)
    if(sqrtTotal >= 1024):
        spriteSheetSize = 2048
    elif(sqrtTotal >= 512):
        spriteSheetSize = 1024
    elif(sqrtTotal >= 256):
        spriteSheetSize = 512
    elif(sqrtTotal >= 128):
        spriteSheetSize = 256
    elif(sqrtTotal >= 64):
        spriteSheetSize = 128
    elif(sqrtTotal >= 32):
        spriteSheetSize = 64
    elif(sqrtTotal >= 16):
        spriteSheetSize = 32
    elif(sqrtTotal >= 8):
        spriteSheetSize = 16
    elif(sqrtTotal >= 4):
        spriteSheetSize = 8
    elif(sqrtTotal >= 2):
        spriteSheetSize = 4
    elif(sqrtTotal >= 1):
        spriteSheetSize = 2
    else:
        spriteSheetSize = 1
    
    return spriteSheetSize


def getSizeOfImageList(imageList, padding):
    total = 0
    for image in imageList:
        _, w, h, _, _ = image
        total += (w + padding * 2) * (h + padding * 2)
    
    return total


@click.command()
@click.option('-i', '--import', "importPath", required=True, type=click.Path(exists=True))
@click.option('-o', '--output', "outputPath", required=True, type=click.Path())
@click.option('-p', '--padding', "padding", default=0)
@click.option('-R', '--rotate', "allowRotate", is_flag=True, default=False)
@click.option('-F', '--fillPrevRow', "tryRefillRow", is_flag=True, default=False)
def main(importPath, outputPath, padding, allowRotate, tryRefillRow):
    importPath += "\\"
    outputPath += "\\"
        
    data = {}
    data["totalImageCount"] = 0
    data["totalTextureCount"] = 0
    data["textures"] = []
    data["spriteIndex"] = {}
    
    if (not os.path.isdir(importPath)):
        print("Error : Import Path isn't a folder")
        sys.exit(1)
    
    if os.path.exists(outputPath):
        if (not os.path.isdir(outputPath)):
            print("Error : Output Path isn't a folder")
            sys.exit(1)
    else:
        os.mkdir(outputPath)
    
    if len(os.listdir(outputPath)) > 0:
        print("Error : Output Path isn't empty")
        sys.exit(1)        
    
    fileList = sorted_alphanumeric(os.listdir(importPath))
    imageList = []
    totalSize = 0
    
    for filename in fileList:
        if os.path.isdir(importPath + filename):
            fileList.remove(filename)
        else:
            with Image.open(importPath + filename) as image:
                data["spriteIndex"][filename] = ""
                image = Image.open(importPath + filename)
                w, h = image.size
                rotated = False
                if w > h and allowRotate:
                    image = image.rotate(90, expand=True)
                    w, h = image.size
                    rotated = True
                imageList.append((image.copy(), w, h, filename, rotated)) #filename.split('.')[0]
                totalSize += (w + padding * 2) * (h + padding * 2)
    
    imageList = sorted(imageList, key=lambda x: x[2], reverse=True)
    
    data["totalImageCount"] = len(imageList)
    
    spriteSheetSize = getSmallestSheetSize(totalSize)
    completed = False
    textureIndex = 0
    startImageIndex = 0
    
    
    while(not completed):
        completed = True
        remainingRows = []
        Ys = [0]
        spritSheet = Image.new(mode="RGBA", size=(spriteSheetSize, spriteSheetSize))
        data["textures"].append({"textureName": "", "textureSize": spriteSheetSize, "imageCount" : 0, "images": []})
        maxX = 0
        maxY = 0
        pos = (0, 0)
        for imageIndex in range(startImageIndex, len(imageList)):
            image, w, h, filename, rotated = imageList[imageIndex]
            if tryRefillRow:
                foundPrevEmplacement = False
                for emplacement in remainingRows:
                    if w <= emplacement[0] and h <= emplacement[1]:
                        spritSheet.paste(image, (emplacement[2][0] + padding, emplacement[2][1] + padding))
                        data["spriteIndex"][filename] = str((textureIndex, imageIndex))
                        data["textures"][textureIndex]["imageCount"] += 1
                        data["textures"][textureIndex]["images"].append({"filename": filename, 
                                                                         "x" : emplacement[2][0] + padding, 
                                                                         "y": emplacement[2][1] + padding, 
                                                                         "w": w, 
                                                                         "h": h,
                                                                         "rotated": rotated
                                                                         })
                        remainingRows.remove(emplacement)
                        if spriteSheetSize - emplacement[2][0] - padding * 2 > 0 and emplacement[2][0] + w + padding * 2 < spriteSheetSize:
                            remainingRows.append((spriteSheetSize - emplacement[2][0] - w - padding * 4, 
                                                  emplacement[1], 
                                                  (emplacement[2][0] + w + padding * 2, 
                                                   emplacement[2][1])))
                        foundPrevEmplacement = True
                        break
                if foundPrevEmplacement:
                    continue
                    
            if pos[0] + w + padding * 2 > spriteSheetSize:
                if spriteSheetSize - pos[0] - padding * 2 > 0:
                    remainingRows.append((spriteSheetSize - pos[0] - padding * 2, max(Ys) - padding * 2, pos))
                pos = (0, pos[1] + max(Ys))
                Ys.clear()
                
            if pos[1] + h + padding * 2 > spriteSheetSize or w > spriteSheetSize:
                completed = False
                spriteSheetSize *= 2
                if spriteSheetSize == 4096:
                    spritSheet.save(outputPath + str(textureIndex) + ".png")
                    data["textures"][textureIndex]["textureName"] = str(textureIndex) + ".png"
                    textureIndex += 1
                    startImageIndex = imageIndex
                    total = getSizeOfImageList(imageList[startImageIndex:], padding)
                    spriteSheetSize = getSmallestSheetSize(total)
                else:
                    data["textures"].pop(-1)
                spritSheet = Image.new(mode="RGBA", size=(spriteSheetSize, spriteSheetSize))
                break;
                
            spritSheet.paste(image, (pos[0] + padding, pos[1] + padding))
            data["spriteIndex"][filename] = str((textureIndex, imageIndex))
            data["textures"][textureIndex]["imageCount"] += 1
            data["textures"][textureIndex]["images"].append({"filename": filename, 
                                                             "x" : pos[0] + padding, 
                                                             "y": pos[1] + padding, 
                                                             "w": w, 
                                                             "h": h,
                                                             "rotated": rotated
                                                             })
            maxX = max(maxX, pos[0] + w + padding * 2)
            maxY = max(maxY, pos[1] + h + padding * 2)
            pos = (pos[0] + w + padding * 2, pos[1])
            Ys.append(h + padding * 2)
        
    data["totalTextureCount"] = textureIndex + 1
    spritSheet.save(outputPath + str(textureIndex) + ".png")
    data["textures"][textureIndex]["textureName"] = str(textureIndex) + ".png"
    
    with open(outputPath + 'spritSheetData.json', 'w') as outfile:
        json.dump(data, outfile, indent=4)

if __name__ == '__main__':
    if len(sys.argv) == 1:
        main.main(['--help'])
    else:
        main()