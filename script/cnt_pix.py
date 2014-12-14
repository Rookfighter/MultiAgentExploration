import sys
import os
from PIL import Image

BLACK_THRESHOLD = 20

# main
assert(len(sys.argv) > 1)

filename = sys.argv[1]
print "Analyzing file '" + filename + "'"
assert(os.path.exists(filename))

img = Image.open(filename, 'r')
rgb_img = img.convert('RGB')
pixels = rgb_img.load()

blackCount = 0
pixelCount = img.size[0] * img.size[1];

for i in xrange(img.size[0]):
    for j in xrange(img.size[1]):
        rgb = pixels[i,j]
        
        if rgb[0] <= BLACK_THRESHOLD and rgb[1] <= BLACK_THRESHOLD and rgb[2] <= BLACK_THRESHOLD:
            blackCount = blackCount + 1

percent = float(blackCount) / float(pixelCount)
           
print "Black Pixels: " + str(blackCount) + "/" + str(pixelCount) + " (" + repr(percent) + ")"
