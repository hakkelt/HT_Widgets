#include "graphics.hpp"
#include "color.h"
#include <sstream>
#include <iostream>

void Color::setValue(unsigned char r0, unsigned char g0, unsigned char b0)
{
    r = r0;
    g = g0;
    b = b0;
}

void Color::setValue(const Color &c)
{
    r = c.r;
    g = c.g;
    b = c.b;
}

bool Color::setValue(std::string str)
{
    // Hibás stringek szûrése
    if ( str[0] != '#' || str.length() != 7 )
    {
        std::cerr << "Incorrect string passed to Color::setValue (it does not start with '#')" << std::endl;
        return false;
    }
    std::string allowedChars = "0123456789abcdef";
    for ( unsigned int i = 1; i < 7; i++ )
        if ( allowedChars.find(tolower(str[i])) == std::string::npos )
        {
            std::cerr << "Incorrect argument passed to Color::setValue (it contains illegal characters)" << std::endl;
            return false;
        }

    // konvertálás
    unsigned int x, y, z;
    std::stringstream ss;
    ss << std::hex << str.substr(1,2);
    ss >> x;
    r = x;
    ss.clear();
    ss << std::hex << str.substr(3,2);
    ss >> y;
    g = y;
    ss.clear();
    ss << std::hex << str.substr(5,2);
    ss >> z;
    b = z;

    return true;
}

Color::Color()
{
    r = g = b = 0;
}

Color::Color(const Color &c)
{
    setValue(c);
}

Color::Color (unsigned char r0, unsigned char g0, unsigned char b0)
{
    setValue(r0, g0, b0);
}

Color::Color(std::string str)
{
    setValue(str);
}

Color& Color::operator= (const Color& c)
{
    setValue(c);
    return *this;
}

bool Color::operator= (const std::string str)
{
    return setValue(str);
}

Color Color::operator+ (const int num) const
{
    Color temp;
    if ( 255 < (signed)r + num || 255 < (signed)g + num || 255 < (signed)b + num )
    {
        std::cerr << "Invalid color (255 < r + num || 255 < g + num || 255 < b + num) ! - Color::operator+" << std::endl;
        return Color(0,0,0);
    }
    temp.r = r + num;
    temp.g = g + num;
    temp.b = b + num;
    return temp;
}

Color Color::operator+ (const Color& c) const
{
    Color temp;
    if ( 255 < (signed)r + c.r || 255 < (signed)g + c.g || 255 < (signed)b + c.b )
    {
        std::cerr << "Invalid color (255 < r + c.r || 255 < g + c.g || 255 < b + c.b) ! - Color::operator+" << std::endl;
        return Color(0,0,0);
    }
    temp.r = r + c.r;
    temp.g = g + c.g;
    temp.b = b + c.b;
    return temp;
}

Color& Color::operator+= (int num)
{
    return *this = *this + num;
}

Color& Color::operator+= (const Color& c)
{
    return *this = *this + c;
}

Color Color::operator- (const int num) const
{
    Color temp;
    if ( (signed)r < num || (signed)g < num || (signed)b < num )
    {
        std::cerr << "Invalid color (r < num || g < num || b < num) ! - Color::operator+" << std::endl;
        return Color(0,0,0);
    }
    temp.r = r - num;
    temp.g = g - num;
    temp.b = b - num;
    return temp;
}

Color Color::operator- (const Color& c) const
{
    Color temp;
    if ( (signed)r < c.r || (signed)g < c.g || (signed)b < c.b )
    {
        std::cerr << "Invalid color (r < c.r || g < c.g || b < c.b) ! - Color::operator+" << std::endl;
        return Color(0,0,0);
    }
    temp.r = r - c.r;
    temp.g = g - c.g;
    temp.b = b - c.b;
    return temp;
}

Color& Color::operator-= (const int num)
{
    return *this = *this - num;
}

Color& Color::operator-= (const Color& c)
{
    return *this = *this - c;
}

Color& Color::operator++ ()
{
    return *this += 1;
}

Color& Color::operator-- ()
{
    return *this -= 1;
}

void Color::getValue(unsigned char &r0, unsigned char &g0, unsigned char &b0) const
{
    r0 = r;
    g0 = g;
    b0 = b;
}

void colorize(const Color &c)
{
    genv::gout << genv::color(c.r, c.g, c.b);
}


/// --------------------------------- Rajzoló függvények -------------------------------------------------------------
void rectange(int x, int y, int width, int height, Color C)
{
    colorize(C);
    genv::gout << genv::move_to(x,y) << genv::line(width, 0) << genv::line(0, height)
        << genv::line(-width,0) << genv::line(0, -height);
}

void rectange3D(int x, int y, int width, int height, bool pressed)
{
    if ( pressed )
    {
        colorize(DarkGray);
        genv::gout << genv::move_to(x,y + height) << genv::line(0, -height - 1) << genv::line(width, 0);
        colorize(White);
        genv::gout << genv::line(0, height + 1) << genv::line(-width,0);
    }
    else
    {
        colorize(White);
        genv::gout << genv::move_to(x,y + height) << genv::line(0, -height - 1) << genv::line(width, 0);
        colorize(DarkGray);
        genv::gout << genv::line(0, height + 1) << genv::line(-width,0);
    }
}

void circle(int x, int y, int diameter, Color C)
{
    colorize(C);
    for ( int i = x; i <= x + diameter; i++ )
    {
        for ( int j = y; j < y + diameter; j++ )
        {
            if ( (i - x - diameter/2)*(i - x - diameter/2) + (j - y - diameter/2)*(j - y - diameter/2) < diameter*diameter/4 )
                genv::gout << genv::move_to (i, j) << genv::dot;
        }
    }
}

#include <math.h>
void circle3D(int x, int y, int diameter)
{
    const double PI =  3.141592;
    Color temp = DarkGray;
    float _x = x, _y = y, r = diameter/2;
    for (float phi = PI/4; phi < PI*5/4; phi += PI/20)
    {
        colorize(temp += 5);
        genv::gout << genv::move_to(_x + r - r*cos(phi), _y  + r - r*sin(phi))
            << genv::line_to(_x + r - r*cos(phi + PI/20), _y  + r - r*sin(phi + PI/20));
    }
    for (float phi = PI*5/4; phi < PI*9/4; phi += PI/20)
    {
        colorize(temp -= 5);
        genv::gout << genv::move_to(_x + r - r*cos(phi), _y  + r - r*sin(phi))
            << genv::line_to(_x + r - r*cos(phi + PI/20), _y  + r - r*sin(phi + PI/20));
    }

}

