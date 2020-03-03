#ifndef _IMAGE_H_
#define _IMAGE_H_

class Image {
public:
    int width, height;
    unsigned char *data;
    Image(const char *fname);
    ~Image();
    
};

#endif