'''
MIT License

Copyright (c) 2017 Scott Hong

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
'''

import commands
import json
import matplotlib.pyplot as plt
import matplotlib.patches as patches
from PIL import Image
import numpy as np
from scipy import misc

# align_faces model image_file image_size face_path_prefix pyramid_up padding image_quality

align_faces = 'target/align_faces'
model = 'models/shape_predictor_68_face_landmarks.dat'
image_file = 'src/test/resources/g7_summit.jpg'
image_size = 160
face_path_prefix = 'target/g7_summit.jpg'
pyramid_up = 'false'
padding = '0.4'
image_quality = '75'

command = align_faces + ' ' + model + ' ' + image_file + ' ' + str(image_size) + ' ' + face_path_prefix + ' ' + pyramid_up + ' ' + padding + ' ' + image_quality

results = commands.getstatusoutput(command)

resultCode = results[0]
facesJson = results[1]

# parse the json string
faceChips = json.loads(facesJson)

faces = faceChips['faces']
face_path_prefix = faceChips['facePathPrefix']
scale = faceChips['scale']
dim = faceChips['dim']

im = np.array(Image.open(image_file), dtype=np.uint8)
# Create figure and axes
fig, ax = plt.subplots(1)
fig.set_size_inches(10.5, 6)
# Display the image
ax.imshow(im)

image_paths = []
# iterate through the face chips
for face in faces:
    rect = face['rect'] 
    # print face['id'], rect['x'], rect['y'], rect['width'], rect['height']
    image_path = face_path_prefix + '.face_' + str(face['id']) + '.jpg'
    image_paths.append(image_path)
    # Create a Rectangle patch
    rect = patches.Rectangle(
        (rect['x'], rect['y']),
        rect['width'], rect['height'],
        linewidth=1, edgecolor='r',facecolor='none')
    # Add the patch to the Axes
    ax.add_patch(rect)

print '# faces detected :', len(image_paths)
plt.draw()

# Create figure with 3x3 sub-plots.
img_shape = (image_size, image_size)
fig, axes = plt.subplots(1, len(image_paths))
fig.set_size_inches(10.5, 1.25)
fig.subplots_adjust(left=0.01, bottom=0.01, right=0.99, top=0.99, hspace=0.1, wspace=0.1)
for i, ax in enumerate(axes.flat):
    im = np.array(Image.open(image_paths[i]), dtype=np.uint8)
    ax.imshow(im)
    # Remove ticks from the plot.
    ax.set_xticks([])
    ax.set_yticks([])

plt.show()
