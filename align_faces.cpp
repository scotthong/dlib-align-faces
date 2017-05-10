/*
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
*/

// These three directives DLIB_JPEG_SUPPORT, DLIB_PNG_SUPPORT and _MSC_VER need to be defined here
// to workaround the compiling issues under Windows.
#ifndef DLIB_JPEG_SUPPORT      
    #define DLIB_JPEG_SUPPORT     
#endif        

#ifndef DLIB_PNG_SUPPORT      
    #define DLIB_PNG_SUPPORT      
#endif

#ifdef _MSC_VER 
    #define strncasecmp _strnicmp
    #define strcasecmp _stricmp
#endif

#include <dlib/image_processing/frontal_face_detector.h>
#include <dlib/image_processing.h>
#include <dlib/image_io.h>
#include <iostream>
#include <time.h>   

using namespace dlib;
using namespace std;

// ----------------------------------------------------------------------------------------
bool string2bool (const std::string & v)
{
    return !v.empty () && 
           (strcasecmp (v.c_str (), "true") == 0 ||
           atoi (v.c_str ()) != 0);
}

int main(int argc, char** argv)
{
    try
    {
        clock_t t1, t2;
        t1 = clock();        
        std::ostringstream json;

        if (argc < 8)
        {
            json << "{\n\"code\":1," << endl 
                 << "\"message\":\"";
            json << "Usage: align_faces model imageFile imageSize facePathPrefix pyramidUp padding imageQuality"
                 << "\"" << endl << "}";
            cout << json.str();
            return 0;
        }

        frontal_face_detector detector = get_frontal_face_detector();
        shape_predictor sp;
        deserialize(argv[1]) >> sp;

        // cout << "processing image " << argv[i] << endl;
        array2d<rgb_pixel> img;

        std::string imageFile = argv[2];
        // cout << "imageFile : " << imageFile << endl;
        load_image(img, imageFile);

        int imageSize = std::stoi(argv[3]);
        // cout << "imageSize : " << imageSize << endl;

        std::string facePathPrefix = argv[4];
        // cout << "facePathPrefix : " << facePathPrefix << endl;

        float scale = 1.0f;
        bool pyramidUp = string2bool(argv[5]);
        if(pyramidUp) {
            dlib::pyramid_up(img);
            scale = 2.0f;
        }
        // cout << "pyramidUp : " << pyramidUp << endl;

        float padding = 0.05f;
        padding = std::atof(argv[6]);
        // cout << "padding : " << padding << endl;

        int imageQuality = std::stoi(argv[7]);
        // cout << "imageQuality : " << imageQuality << endl;

        // Now tell the face detector to give us a list of bounding boxes
        // around all the faces in the image.
        std::vector<rectangle> dets = detector(img);

        json << "{\"scale\":" << scale << "," << endl
             << "\"facePathPrefix\":\"" << facePathPrefix << "\"," << endl
             << "\"dim\": {\"width\":" 
             << img.nc() << ",\"height\":" << img.nr() 
             << "}";

        if(dets.size() > 0) {
            std::vector<full_object_detection> shapes;

            json << "," << endl <<"\"faces\": [" << endl;
            for(unsigned int j = 0; j < dets.size(); ++j) {
                full_object_detection shape = sp(img, dets[j]);
                rectangle rect = shape.get_rect();

                json << "  {\"id\":" << j
                     << ",\"rect\": {\"x\":" 
                     << rect.left() << ",\"y\":" << rect.top()
                     << ",\"width\":" << rect.width() << ",\"height\":" 
                     << rect.height() << "}}";

                if(j < (dets.size() - 1)) {
                    json << "," << endl;
                }
                shapes.push_back(shape);
            }
            // json << endl << "]," << endl << "\"code\":0}";
            t2 = clock();
            long diff = (t2 - t1);
            json << endl << "]," 
                 << endl << "\"time\":" << diff << ","
                 << endl << "\"code\":0}";            

            dlib::array<array2d<rgb_pixel>> face_chips;
            extract_image_chips(
                img, 
                get_face_chip_details(shapes, imageSize, padding),
                face_chips
            );

            for (unsigned long j = 0; j < face_chips.size(); ++j) {
                std::ostringstream stringStream;
                stringStream << facePathPrefix << ".face_" << j << ".jpg";
                std::string filename = stringStream.str();
                save_jpeg(face_chips[j], filename, imageQuality);
            }
        }
        else {
            json << ",\"code\":0}" << endl;
        }

        cout << json.str();
    }
    catch (exception& e) {
        std::ostringstream json;
        json << "{\"code\":1," << endl 
             << "\"message\":\"" << e.what() << "\"}" << endl;
        cout << json.str();
    }
}

// ----------------------------------------------------------------------------------------
