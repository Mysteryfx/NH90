
#ifndef _Color_h_
#define _Color_h_

__inline int f2i( float f )
{
    int i = (int)f;
 //   __asm fld   f;
 //   __asm fistp i;
    return i;
}

__inline float i2f( int i )
{
    float f = (float)i;
//    __asm fild i;
//    __asm fstp f;
    return f;
}


namespace Graphics
{

class Color
{
public:
    float r,g,b,a;

    Color() { r = g = b = a = 0.0f; }

    Color( float R, float G, float B, float A = 1 )
    { r = R; g = G; b = B; a = A; }

    Color( unsigned uc )
    {
        a = i2f((uc>>24)&0xFF) * (1.0f/255.0f);
        r = i2f((uc>>16)&0xFF) * (1.0f/255.0f);
        g = i2f((uc>>8)&0xFF ) * (1.0f/255.0f);
        b = i2f( uc&0xFF     ) * (1.0f/255.0f);
    }

    Color & operator = ( unsigned uc )
    {
        a = i2f((uc>>24)&0xFF) * (1.0f/255.0f);
        r = i2f((uc>>16)&0xFF) * (1.0f/255.0f);
        g = i2f((uc>>8)&0xFF ) * (1.0f/255.0f);
        b = i2f( uc&0xFF     ) * (1.0f/255.0f);
        return *this;
    }

    Color   operator + ( const Color &c )
    { return Color( r+c.r, g+c.g, b+c.b, a+c.a ); }

    Color   operator - ( const Color &c )
    { return Color(  r-c.r, g-c.g, b-c.b, a-c.a); }

    Color   operator * ( const Color &c )
    { return Color(  r*c.r, g*c.g, b*c.b, a*c.a); }

    Color   operator * ( const float f )
    { return Color( r*f, g*f, b*f, a*f); }

    Color   operator / ( const float f )
    { float f1 = 1.0f / f; return Color( r*f1, g*f1, b*f1, a*f1); }

    Color & operator += ( const Color &c )
    { a += c.a, r += c.r, g += c.g, b += c.b; return *this; }

    Color & operator -= ( const Color &c )
    { a -= c.a, r -= c.r, g -= c.g, b -= c.b; return *this; }

    Color & operator *= ( const Color &c )
    { a *= c.a, r *= c.r, g *= c.g, b *= c.b; return *this; }

    Color & operator *= ( const float f )
    { a *= f, r *= f, g *= f, b *= f; return *this; }

    Color & operator /= ( const float f )
    { float f1 = 1.0f / f; a *= f1, r *= f1, g *= f1, b *= f1; return *this; }

    bool    operator == ( const Color &c )
    { return a == c.a && r == c.r && g == c.g && b == c.b; }

    bool    operator != ( const Color &c )
    { return a != c.a || r != c.r || g != c.g || b != c.b; }

    operator unsigned () const
    {
        unsigned uc = (f2i(a*255.0f)<<24) | (f2i(r*255.0f)<<16) | (f2i(g*255.0f)<<8) | f2i(b*255.0f);
        return uc;
    }

    Color & Sat()
    {
        if ( a < 0.0f ) a = 0.0f; else if ( a > 1.0f ) a = 1.0f;
        if ( r < 0.0f ) r = 0.0f; else if ( r > 1.0f ) r = 1.0f;
        if ( g < 0.0f ) g = 0.0f; else if ( g > 1.0f ) g = 1.0f;
        if ( b < 0.0f ) b = 0.0f; else if ( b > 1.0f ) b = 1.0f;
        return *this;
    }

    Color GetSat()
    {
        Color c( *this );
        return c.Sat();
    }

};

}

#endif _Color_h_
