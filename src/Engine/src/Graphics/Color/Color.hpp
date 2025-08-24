#pragma once

namespace smpl
{
    struct Color
    {
        float r = 0.0f;
        float g = 0.0f;
        float b = 0.0f;
        float a = 0.0f;

        Color(float r, float g, float b, float a = 255.0f)
            : r{ r / 255.0f }, g{ g / 255.0f }, b{ b / 255.0f }, a{ a / 255.0f }
        {

        }

        float* data() 
        { 
            return &r; 
        }

        const float* data() const 
        { 
            return &r; 
        }
    };
}