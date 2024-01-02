#ifndef PIECES_H
#define PIECES_H
#include <iostream>
#include <vector>
#include <string>
using namespace std;
class Pieces
{
public:
    Pieces();
    Pieces(const char &t, const char &c, const double &p);
    const char getType() const;
    const char getColor() const;
    const double getPoint() const;
    void setType(const char &t);
    void setColor(const char &c);
    void setPoint(const char &p);

private:
    char type; // p, k, q, r, n, b
    char color; // w, b, e
    double point;
};

#endif // PIECES_H
