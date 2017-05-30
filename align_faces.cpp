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
#include <setjmp.h>
#include <jpeglib.h>
#include <jerror.h>

using namespace dlib;
using namespace std;

// ----------------------------------------------------------------------------------------
bool string2bool (const std::string & v)
{
    return !v.empty () && 
           (strcasecmp (v.c_str (), "true") == 0 ||
           atoi (v.c_str ()) != 0);
}

// http://renenyffenegger.ch/notes/development/Base64/Encoding-and-decoding-base-64-with-cpp
static const std::string base64_chars = 
             "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
             "abcdefghijklmnopqrstuvwxyz"
             "0123456789+/";

std::string base64_encode(unsigned char const* bytes_to_encode, unsigned long in_len) {
    std::string ret;
    int i = 0;
    int j = 0;
    unsigned char char_array_3[3];
    unsigned char char_array_4[4];
    while (in_len--) {
        char_array_3[i++] = *(bytes_to_encode++);
        if (i == 3) {
            char_array_4[0] = (char_array_3[0] & 0xfc) >> 2;
            char_array_4[1] = ((char_array_3[0] & 0x03) << 4) + ((char_array_3[1] & 0xf0) >> 4);
            char_array_4[2] = ((char_array_3[1] & 0x0f) << 2) + ((char_array_3[2] & 0xc0) >> 6);
            char_array_4[3] = char_array_3[2] & 0x3f;
            for(i = 0; (i <4) ; i++) {
                ret += base64_chars[char_array_4[i]];
            }
            i = 0;
        }
    }

    if (i) {
        for(j = i; j < 3; j++) {
            char_array_3[j] = '\0';
        }
        char_array_4[0] = ( char_array_3[0] & 0xfc) >> 2;
        char_array_4[1] = ((char_array_3[0] & 0x03) << 4) + ((char_array_3[1] & 0xf0) >> 4);
        char_array_4[2] = ((char_array_3[1] & 0x0f) << 2) + ((char_array_3[2] & 0xc0) >> 6);
        char_array_4[3] =   char_array_3[2] & 0x3f;

        for (j = 0; (j < i + 1); j++) {
            ret += base64_chars[char_array_4[j]];
        }

        while((i++ < 3)) {
            ret += '=';
        }
    }
    return ret;
}

//////////////////////////////////////////////////////////////////////////////////////////////////
// https://stackoverflow.com/questions/1443390/compressing-iplimage-to-jpeg-using-libjpeg-in-opencv

/* choose an efficiently fwrite’able size (16k) */
#define OUTPUT_BUF_SIZE 16384

struct jpeg_saver_error_mgr 
{
    jpeg_error_mgr pub;    /* "public" fields */
    jmp_buf setjmp_buffer;  /* for return to caller */
};

void jpeg_saver_error_exit (j_common_ptr cinfo)
{
    /* cinfo->err really points to a jpeg_saver_error_mgr struct, so coerce pointer */
    jpeg_saver_error_mgr* myerr = (jpeg_saver_error_mgr*) cinfo->err;

    /* Return control to the setjmp point */
    longjmp(myerr->setjmp_buffer, 1);
}

/* Expanded data destination object for memory output */
typedef struct {
    struct jpeg_destination_mgr pub;    /* public fields */
    unsigned char ** outbuffer;         /* target buffer */
    unsigned long * outsize;
    unsigned char * newbuffer;          /* newly allocated buffer */
    JOCTET * buffer;                    /* start of buffer */
    size_t bufsize;
} my_mem_destination_mgr;

typedef my_mem_destination_mgr * my_mem_dest_ptr;

void init_mem_destination(j_compress_ptr cinfo) {
    /* no work necessary here */
}

boolean empty_mem_output_buffer(j_compress_ptr cinfo) {
    size_t nextsize;
    JOCTET * nextbuffer;
    my_mem_dest_ptr dest = (my_mem_dest_ptr) cinfo->dest;

    /* Try to allocate new buffer with double size */
    nextsize = dest->bufsize * 2;
    nextbuffer = (JOCTET *)malloc(nextsize);
    if (nextbuffer == NULL) {
        ERREXIT1(cinfo, JERR_OUT_OF_MEMORY, 10);
    }

    memcpy(nextbuffer, dest->buffer, dest->bufsize);

    if (dest->newbuffer != NULL) {
        free(dest->newbuffer);
    }

    dest->newbuffer = nextbuffer;
    dest->pub.next_output_byte = nextbuffer + dest->bufsize;
    dest->pub.free_in_buffer = dest->bufsize;
    dest->buffer = nextbuffer;
    dest->bufsize = nextsize;
    return true;
}

void term_mem_destination(j_compress_ptr cinfo) {
    my_mem_dest_ptr dest = (my_mem_dest_ptr) cinfo->dest;
    *dest->outbuffer = dest->buffer;
    *dest->outsize = dest->bufsize - dest->pub.free_in_buffer;
}

void jpeg_mem_dest(j_compress_ptr cinfo, unsigned char ** outbuffer, unsigned long * outsize) {
    my_mem_dest_ptr dest;

    /* sanity check */
    if (outbuffer == NULL || outsize == NULL) {
        ERREXIT(cinfo, JERR_BUFFER_SIZE);
    }

    /* The destination object is made permanent so that multiple JPEG images
    * can be written to the same buffer without re-executing jpeg_mem_dest.
    */
    /* first time for this JPEG object? */        
    if (cinfo->dest == NULL) {
        cinfo->dest = (struct jpeg_destination_mgr *)
        (*cinfo->mem->alloc_small) ((j_common_ptr) cinfo, JPOOL_PERMANENT, sizeof(my_mem_destination_mgr));
    }

    dest = (my_mem_dest_ptr) cinfo->dest;
    dest->pub.init_destination = init_mem_destination;
    dest->pub.empty_output_buffer = empty_mem_output_buffer;
    dest->pub.term_destination = term_mem_destination;
    dest->outbuffer = outbuffer;
    dest->outsize = outsize;
    dest->newbuffer = NULL;

    if (*outbuffer == NULL || *outsize == 0) {
        /* Allocate initial buffer */
        dest->newbuffer = *outbuffer = (unsigned char*)malloc(OUTPUT_BUF_SIZE);
        if (dest->newbuffer == NULL) {
            ERREXIT1(cinfo, JERR_OUT_OF_MEMORY, 10);
        }
        *outsize = OUTPUT_BUF_SIZE;
    }
    dest->pub.next_output_byte = dest->buffer = *outbuffer;
    dest->pub.free_in_buffer = dest->bufsize = *outsize;
}

void save_jpeg_as_base64(const array2d<rgb_pixel>& img, int quality, std::ostringstream &oss) {
    unsigned char *mem = NULL;
    unsigned long mem_size = 0;
    struct jpeg_compress_struct cinfo;
    struct jpeg_error_mgr jerr;
    cinfo.err = jpeg_std_error(&jerr);
    jpeg_create_compress(&cinfo);
    jpeg_mem_dest(&cinfo, &mem, &mem_size);
     
    cinfo.image_width      = img.nc();
    cinfo.image_height     = img.nr();
    cinfo.input_components = 3;
    cinfo.in_color_space   = JCS_RGB;
    jpeg_set_defaults(&cinfo);
    jpeg_set_quality (&cinfo, quality, TRUE);
    jpeg_start_compress(&cinfo, TRUE);
     
    // now write out the rows one at a time
    while (cinfo.next_scanline < cinfo.image_height) {
        JSAMPROW row_pointer = (JSAMPROW) &img[cinfo.next_scanline][0];
        jpeg_write_scanlines(&cinfo, &row_pointer, 1);
    }
    jpeg_finish_compress(&cinfo);
    jpeg_destroy_compress(&cinfo);

    std:string base64EncodedJpeg = base64_encode(mem, mem_size);
    oss << base64EncodedJpeg;
    // freeup the memory
    free(mem);
}
//////////////////////////////////////////////////////////////////////////////////////////////////

int main(int argc, char** argv) {
    try {
        clock_t t1, t2;
        t1 = clock();        
        std::ostringstream json;

        if (argc < 7) {
            json << "{\n\"code\":1," << endl 
                 << "\"message\":\"";
            json << "Usage: align_faces model imageFile imageSize pyramidUp padding imageQuality"
                 << "\"" << endl << "}" << endl;
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

        float scale = 1.0f;
        bool pyramidUp = string2bool(argv[4]);
        if(pyramidUp) {
            dlib::pyramid_up(img);
            scale = 2.0f;
        }
        // cout << "pyramidUp : " << pyramidUp << endl;

        float padding = 0.05f;
        padding = std::atof(argv[5]);
        // cout << "padding : " << padding << endl;

        int imageQuality = std::stoi(argv[6]);
        // cout << "imageQuality : " << imageQuality << endl;

        // Now tell the face detector to give us a list of bounding boxes
        // around all the faces in the image.
        std::vector<rectangle> dets = detector(img);

        json << "{\"scale\":" << scale << "," << endl
             << "\"dim\": {\"width\":" 
             << img.nc() << ",\"height\":" << img.nr() 
             << "}";

        if(dets.size() > 0) {
            std::vector<full_object_detection> shapes;
            //
            for(unsigned int j = 0; j < dets.size(); ++j) {
                full_object_detection shape = sp(img, dets[j]);
                shapes.push_back(shape);
            }

            dlib::array<array2d<rgb_pixel>> face_chips;
            extract_image_chips(
                img, 
                get_face_chip_details(shapes, imageSize, padding),
                face_chips
            );

            json << "," << endl <<"\"faces\": [" << endl;
            for(unsigned int j = 0; j < shapes.size(); ++j) {
                full_object_detection shape = shapes[j];
                rectangle rect = shape.get_rect();
                // id
                json << "  {\"id\":" << j << ",";
                // rect
                json << " \"rect\": {\"x\":" 
                     << rect.left() << ",\"y\":" << rect.top()
                     << ",\"width\":" << rect.width() << ",\"height\":" 
                     << rect.height() << "}," << endl;

                // base64Image
                json << "    \"base64Image\": \"";
                save_jpeg_as_base64(face_chips[j], imageQuality, json);
                json << "\"}";

                if(j < (dets.size() - 1)) {
                    json << "," << endl;
                }
            }

            t2 = clock();
            long diff = (t2 - t1);

            // json << endl << "]," << endl << "\"code\":0}";
            json << endl << "]," 
                 << endl << "\"time\":" << diff << ","
                 << endl << "\"code\":0}" << endl;

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
