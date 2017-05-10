# align_faces

The purpose of this project is to provide a tool with the following functions:

1. Detect frontal faces in an image.
2. Output face chip rectangles/bounding boxes in JSON format to the console.
3. Align the face chips and save them to the specified destination.

You can capture the JSON output and read the exported face chips (JPEG images) to integrate with your application.

## Support platforms

This tool can be compiled under multiple platforms the same way Dlib C++ library is supported. However, only limited testings are performed under the following platforms:

1. Linux/Ubuntu 14.04: Linux_x86_64
2. Mac/OSX: mac_x86_64
3. Windows/Windows 7: windows_x86_64

## How to compile

### Clone the dlib-align-faces project from github.

```
git clone https://github.com/scotthong/dlib-align-faces.git
```

### Install Dlib dependencies

The align_faces tool is developed using the [Dlib C++ Library](http://dlib.net). Please refer to the [Dlib website](http://dlib.net) for detailed instructions on how to build [Dlib](http://dlib.net) on your target platform. Please make sure that the required dependencies are installed.

### Additional dependencies

This project utilize a custom [ANT](http://ant.apache.org/) script to build the align_faces tool. Pease make sure you have the following dependencies installed and configured properly.

1. Java
2. Ant

### Compile align_faces

Please execute the default ant target to compile the align_faces tool. The "build-all" ant target will perform the following three operations:
1. Clone Dlib from github
2. Checkout Dlib branch v19.4
3. Update submodule if there is any
4. Build align_faces using cmake.
5. Copy align_faces to the corresponding target directory for distribution as pre-compiled binary.

```
ant build-all
```


### Run the pre-build align_faces tool

This dlib-align-faces repository includes three pre-build align_faces binaries under src/main/bin. These binaries are compiled using the optimization options as configured in the "build.xml" file. The binaries are compiled to support x86_64 processors that support SSE4 instructions. If your processor support AVX or better, please change the configuration and build the binaries yourself to take advantage these instructions for better performance.

Please execute the following ant target to run the example:
```
ant run
```

Here is JSON output:
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

The face chips are saved as jpeg images under the "target" directory with the file name prefixed as "g7_summit.jpg.align"
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

### Syntax

## Credits
The model files contained in this directory are downloaded and extracted from the dlib-models github repository:
* [https://github.com/davisking/dlib-models](https://github.com/davisking/dlib-models)

Please refer to the following link for the LICENSE information for the shape model:
* [https://github.com/davisking/dlib-models/blob/master/LICENSE](https://github.com/davisking/dlib-models/blob/master/LICENSE)

* [dlib C++ library](https://github.com/davisking/dlib)
