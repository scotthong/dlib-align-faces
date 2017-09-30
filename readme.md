# align_faces

This tool provide the following functions:

1. Detect frontal faces in an image.
2. Output the face chips' rectangles/bounding boxes and base64 encoded images in JSON format to the console.

You can capture the JSON output and parse the JSON object to integrate with your application.

![bound boxes](https://github.com/scotthong/dlib-align-faces/blob/master/src/main/resources/bounding_boxes.png)

![bound boxes](https://github.com/scotthong/dlib-align-faces/blob/master/src/main/resources/face_chips.png)

## Supported platforms
This project has been compiled successfully under the following platforms:

1. Linux(Ubuntu/CentOS)
2. Mac OS X
3. Windows

## How to compile

### Clone the dlib-align-faces project from github.

```
git clone https://github.com/scotthong/dlib-align-faces.git
```

### [Dlib C++ Library](http://dlib.net) dependencies

The **align_faces** tool is developed based on the [Dlib C++ Library](http://dlib.net). Please refer to the [Dlib website](http://dlib.net) for detailed instructions on how to build [Dlib](http://dlib.net) on your platform. Please make sure that the required dependencies are installed.

### Additional dependencies

This project utilize a custom [ANT](http://ant.apache.org/) script to build the **align_faces** tool. Pease make sure you have the following dependencies installed and configured properly for your environment.

1. [Java](https://www.java.com)
2. [ANT](http://ant.apache.org/)

### Compile align_faces

Please execute the following ant target to compile the **align_faces** tool.

```
ant build-all
```

The "build-all" ant target execute the following tasks:

1. Clone Dlib from github
2. Checkout Dlib branch v19.7
3. Update submodule if there is any
4. Build **align_faces** using cmake.
5. Copy **align_faces** to the corresponding target directory for distribution as pre-build binary.

### align_faces command options

**align_faces** model imageFile imageSize pyramidUp padding imageQuality

* model: The path to the shape model.
    * models/shape_predictor_5_face_landmarks.dat
* imageFile: The path to the image file.
    * src/test/resources/g7_summit.jpg
* imageSize: The size of the image (Width and Height).
    * 160
* pyramidUp: Specify whether the image should be scaled up (2x).
    * false
* padding: The margin or padding to be added to the face chips.
    * 0.4
* imageQuality: The quality value determines how lossy the compression is. Larger quality values result in larger output images but the images will look better.
    * 75

### Run align_faces using pre-configured ANT target

Please execute the following ant target to run the example:
```
ant run
```

Or after building **align_faces** from source, you can execute the following command:
```
target/align_faces models/shape_predictor_5_face_landmarks.dat src/test/resources/g7_summit.jpg 160 target/g7_summit.jpg.align false 0.4 75
```

Here is an example of the bounding boxes/rectangles exported as JSON to the console:
```
{
"scale":1,
"facePathPrefix":"target/g7_summit.jpg.align",
"dim": {"width":3000,"height":1994},
"faces": [
  {"id":0,"rect": {"x":592,"y":707,"width":87,"height":87},"base64Image": "..."},
  {"id":1,"rect": {"x":2224,"y":659,"width":87,"height":87},"base64Image": "..."},
  {"id":2,"rect": {"x":1446,"y":736,"width":88,"height":87},"base64Image": "..."},
  {"id":3,"rect": {"x":1925,"y":677,"width":73,"height":73},"base64Image": "..."},
  {"id":4,"rect": {"x":1715,"y":755,"width":88,"height":87},"base64Image": "..."},
  {"id":5,"rect": {"x":2485,"y":688,"width":104,"height":104},"base64Image": "..."},
  {"id":6,"rect": {"x":1139,"y":688,"width":88,"height":87},"base64Image": "..."},
  {"id":7,"rect": {"x":323,"y":765,"width":88,"height":87},"base64Image": "..."},
  {"id":8,"rect": {"x":861,"y":774,"width":87,"height":88},"base64Image": "..."}
],
"time":3478308,
"code":0
}
```

### python integration demo
A simple python script **align_faces.py** to integrate with the **align_faces** is also included. Please use the following command to run the python script. A figure of the image with the bounding boxes of the detected faces will be displayed. A separate figure with the extracted face chips will be displayed as well.
```
python align_faces.py
```

## Credits

The shape model file distributed with this repository is downloaded from the [dlib-models github repository](https://github.com/davisking/dlib-models).

