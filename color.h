#ifndef COLOR_H_INCLUDED
#define COLOR_H_INCLUDED

#include <string>

/// Sz�nek t�rol�s�ra alkalmas rekord

struct Color
{
    unsigned char r,g,b;

    Color();
    Color(const Color &c);
    Color (unsigned char r0, unsigned char g0, unsigned char b0);
    Color(std::string str);

    void setValue(unsigned char r0, unsigned char g0, unsigned char b0);
    void setValue(const Color &c);
    bool setValue(const std::string str);
    void getValue(unsigned char &r0, unsigned char &g0, unsigned char &b0) const;

    Color& operator= (const Color& c);
    bool operator= (const std::string str);
    Color operator+ (const int num) const;
    Color operator+ (const Color& c) const;
    Color& operator+= (const int num);
    Color& operator+= (const Color& c);
    Color& operator++ ();
    Color operator- (const int num) const;
    Color operator- (const Color& c) const;
    Color& operator-= (const int num);
    Color& operator-= (const Color& c);
    Color& operator-- ();
};

const Color Black = Color(0, 0, 0);
const Color White = Color(255, 255, 255);
const Color Gray = Color(200, 200, 200);
const Color DarkGray = Color(100, 100, 100);
const Color BackgroundColor = Color(230, 230, 240);
const Color ActiveColor = Color(200, 215, 200);
const Color ButtonColor = Color(200, 200, 215);
const Color Red = Color(220, 0, 0);
const Color Green = Color(0, 220, 0);
const Color Blue = Color(0, 0, 255);

// Rajzol�s sz�n�nek be�ll�t�sa
void colorize(const Color &c);

// Kit�lt�s n�lk�li keret
void rectange(int x, int y, int width, int height, Color C = Black);
// Kit�lt�s n�lk�li �rny�kolt keret
void rectange3D(int x, int y, int width, int height, bool pressed);
// Kit�lt�tt k�r
void circle(int x, int y, int diameter, Color C);
// Kit�lt�s n�lk�li �rny�kolt k�r
void circle3D(int x, int y, int diameter);

#endif // COLOR_H_INCLUDED
