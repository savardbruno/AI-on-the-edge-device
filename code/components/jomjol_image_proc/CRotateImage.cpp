#include "CRotateImage.h"


CRotateImage::CRotateImage(CImageBasis *_org, CImageBasis *_temp)
{
    rgb_image = _org->rgb_image;
    channels = _org->channels;
    width = _org->width;
    height = _org->height;
    bpp = _org->bpp;
    externalImage = true;   
    ImageTMP = _temp;    
}

void CRotateImage::Mirror(){
    int memsize = width * height * channels;
    uint8_t* odata;
    if (ImageTMP)
    {
        odata = ImageTMP->rgb_image;
    }
    else
    {
        odata = (unsigned char*)GET_MEMORY(memsize);
    }


    int x_source, y_source;
    stbi_uc* p_target;
    stbi_uc* p_source;

    for (int x = 0; x < width; ++x)
        for (int y = 0; y < height; ++y)
        {
            p_target = odata + (channels * (y * width + x));

            x_source = width - x;
            y_source = y;

            p_source = rgb_image + (channels * (y_source * width + x_source));
            for (int _channels = 0; _channels < channels; ++_channels)
                p_target[_channels] = p_source[_channels];
        }

    //    memcpy(rgb_image, odata, memsize);
    memCopy(odata, rgb_image, memsize);
    if (!ImageTMP)
    {
        stbi_image_free(odata);
    }
}

void CRotateImage::Rotate(float _angle, int _centerx, int _centery)
{
    float m[2][3];

    float x_center = _centerx;
    float y_center = _centery;
    _angle = _angle / 180 * M_PI;

    m[0][0] = cos(_angle);
    m[0][1] = sin(_angle);
    m[0][2] = (1 - m[0][0]) * x_center - m[0][1] * y_center;

    m[1][0] = -m[0][1];
    m[1][1] = m[0][0];
    m[1][2] = m[0][1] * x_center + (1 - m[0][0]) * y_center;

    int memsize = width * height * channels;
    uint8_t* odata;
    if (ImageTMP)
    {
        odata = ImageTMP->rgb_image;
    }
    else
    {
        odata = (unsigned char*)GET_MEMORY(memsize);
    }
    

    int x_source, y_source;
    stbi_uc* p_target;
    stbi_uc* p_source;

    for (int x = 0; x < width; ++x)
        for (int y = 0; y < height; ++y)
        {
            p_target = odata + (channels * (y * width + x));

            x_source = int(m[0][0] * x + m[0][1] * y);
            y_source = int(m[1][0] * x + m[1][1] * y);

            x_source += int(m[0][2]);
            y_source += int(m[1][2]);

            if ((x_source >= 0) && (x_source < width) && (y_source >= 0) && (y_source < height))
            {
                p_source = rgb_image + (channels * (y_source * width + x_source));
                for (int _channels = 0; _channels < channels; ++_channels)
                    p_target[_channels] = p_source[_channels];
            }
            else
            {
                for (int _channels = 0; _channels < channels; ++_channels)
                    p_target[_channels] = 255;
            }
        }

    //    memcpy(rgb_image, odata, memsize);
    memCopy(odata, rgb_image, memsize);

    if (!ImageTMP)
    {
        stbi_image_free(odata);
    }
}

void CRotateImage::Rotate(float _angle)
{
//    printf("width %d, height %d\n", width, height);
    Rotate(_angle, width / 2, height / 2);
}

void CRotateImage::Translate(int _dx, int _dy)
{
    int memsize = width * height * channels;
    uint8_t* odata;
    if (ImageTMP)
    {
        odata = ImageTMP->rgb_image;
    }
    else
    {
        odata = (unsigned char*)GET_MEMORY(memsize);
    }



    int x_source, y_source;
    stbi_uc* p_target;
    stbi_uc* p_source;

    for (int x = 0; x < width; ++x)
        for (int y = 0; y < height; ++y)
        {
            p_target = odata + (channels * (y * width + x));

            x_source = x - _dx;
            y_source = y - _dy;

            if ((x_source >= 0) && (x_source < width) && (y_source >= 0) && (y_source < height))
            {
                p_source = rgb_image + (channels * (y_source * width + x_source));
                for (int _channels = 0; _channels < channels; ++_channels)
                    p_target[_channels] = p_source[_channels];
            }
            else
            {
                for (int _channels = 0; _channels < channels; ++_channels)
                    p_target[_channels] = 255;
            }
        }

    //    memcpy(rgb_image, odata, memsize);
    memCopy(odata, rgb_image, memsize);
    if (!ImageTMP)
    {
        stbi_image_free(odata);
    }
}

