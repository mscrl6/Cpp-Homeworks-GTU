#include "Pieces.h"

Pieces::Pieces() : type('\0'), color('\0'), point(0) {}

Pieces::Pieces(const char &t, const char &c, const double &p) : type(t), color(c), point(p) {}

const char Pieces::getType() const
{
    return type;
}

const char Pieces::getColor() const
{
    return color;
}
const double Pieces::getPoint() const
{
    return point;
}
void Pieces::setType(const char &t)
{
    type = t;
}
void Pieces::setPoint(const char &p)
{
    point = p;
}
void Pieces::setColor(const char &c)
{
    color = c;
}
