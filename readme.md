# align_faces

This tool provide the following functions:

1. Detect frontal faces in an image.
2. Output face chip rectangles/bounding boxes in JSON format to the console.
3. Align the face chips and save them to the specified destination with the given size, padding and quality.

You can capture the JSON output and read these face chips (JPEG images) to integrate with your application.

## Support platforms

This tool supports multiple platforms the same as Dlib C++ library is supported. Limited testings have performed under the following platforms:

1. Linux/Ubuntu 14.04: Linux_x86_64
2. Mac/OSX: mac_x86_64
3. Windows/Windows 7: windows_x86_64

## How to compile

### Clone the dlib-align-faces project from github.

```
git clone https://github.com/scotthong/dlib-align-faces.git
```

### [Dlib C++ Library](http://dlib.net) dependencies

The align_faces tool is developed based on the [Dlib C++ Library](http://dlib.net). Please refer to the [Dlib website](http://dlib.net) for detailed instructions on how to build [Dlib](http://dlib.net) on your target platform. Please make sure that the required dependencies are installed.

### Additional dependencies

This project utilize a custom [ANT](http://ant.apache.org/) script to build the align_faces tool. Pease make sure you have the following dependencies installed and configured properly for your environment.

1. [Java](https://www.java.com)
2. [ANT](http://ant.apache.org/)

### Compile align_faces

Please execute the following ant target to compile the align_faces tool.

```
ant build-all
```

The "build-all" ant target execute the following tasks:

1. Clone Dlib from github
2. Checkout Dlib branch v19.4
3. Update submodule if there is any
4. Build align_faces using cmake.
5. Copy align_faces to the corresponding target directory for distribution as pre-build binary.

### align_faces command options

align_faces model imageFile imageSize facePathPrefix pyramidUp padding imageQuality

* model: The path to the shape model.
    * models/shape_predictor_68_face_landmarks.dat
* imageFile: The path to the image file.
    * src/test/resources/g7_summit.jpg
* imageSize: The size of the image (Width and Height).
    * 160
* facePathPrefix: The file path prefix of the face chip.
    * target/g7_summit.jpg.align
* pyramidUp: Specify whether the image should be scaled up (2x).
    * false
* padding: The margin or padding to be added to the face chips.
    * 0.4
* imageQuality: The quality value determines how lossy the compression is. Larger quality values result in larger output images but the images will look better.
    * 75

### Run the pre-build align_faces

This dlib-align-faces repository includes three pre-build align_faces binaries under "src/main/bin". These binaries are compiled using the optimization options as configured in the "build.xml" file. The binaries are compiled to support x86_64 processors that support SSE4 instructions. If your processor support AVX or better, please change the configuration and build the binaries yourself to take advantage of these instructions for better performance.

Please execute the following ant target to run the example:
```
ant run
```

Or you can execute the command directly on the console.
```
align_faces models/shape_predictor_68_face_landmarks.dat src/test/resources/g7_summit.jpg 160 target/g7_summit.jpg.align false 0.4 75
```

Here is an example of the bounding boxes/rectangles exported as JSON to the console:
```
{
"scale":1,
"facePathPrefix":"target/g7_summit.jpg.align",
"dim": {"width":3000,"height":1994},
"faces": [
  {"id":0,"rect": {"x":592,"y":707,"width":87,"height":87}},
  {"id":1,"rect": {"x":2224,"y":659,"width":87,"height":87}},
  {"id":2,"rect": {"x":1446,"y":736,"width":88,"height":87}},
  {"id":3,"rect": {"x":1925,"y":677,"width":73,"height":73}},
  {"id":4,"rect": {"x":1715,"y":755,"width":88,"height":87}},
  {"id":5,"rect": {"x":2485,"y":688,"width":104,"height":104}},
  {"id":6,"rect": {"x":1139,"y":688,"width":88,"height":87}},
  {"id":7,"rect": {"x":323,"y":765,"width":88,"height":87}},
  {"id":8,"rect": {"x":861,"y":774,"width":87,"height":88}}
],
"time":3478308,
"code":0
}
```

The face chips are saved as JPEG images under the "target" directory with the file name prefixed as "g7_summit.jpg.align".
```
target/g7_summit.jpg.align.face_0.jpg
target/g7_summit.jpg.align.face_1.jpg
target/g7_summit.jpg.align.face_2.jpg
target/g7_summit.jpg.align.face_3.jpg
target/g7_summit.jpg.align.face_4.jpg
target/g7_summit.jpg.align.face_5.jpg
target/g7_summit.jpg.align.face_6.jpg
target/g7_summit.jpg.align.face_7.jpg
target/g7_summit.jpg.align.face_8.jpg
```

### python integration demo
A simple python script align_faces.py to integrate with align_faces is also provided. Please use the following command run the python script. A plot with the image and bounding boxes of the detected faces will be displayed. A separate plot with aligned face chips will also display as well.
```
python align_faces.py
```

## Credits

The shape model file distributed with this repository is downloaded from the [dlib-models github repository](https://github.com/davisking/dlib-models).

A separate shape model trained using the dataset as detailed [here](https://github.com/davisking/dlib/issues/359) is also provided under the ../models directory. The file size is much smaller. The performance difference as compared to the one provided by Dlib is yet to be validated! Please let me know if you run any comparison using these two shape models.
