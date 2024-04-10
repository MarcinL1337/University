#include "figury.hpp"
#include "globals.hpp"

double odleglosc(punkt a, punkt b)
{
    double bracket1 = b.getX() - a.getX();
    double bracket2 = b.getY() - a.getY();

    return sqrt(pow(bracket1, 2) + pow(bracket2, 2));
}

double poleTrojkata(punkt a, punkt b, punkt c)
{
    double A = odleglosc(a, b);
    double B = odleglosc(b, c);
    double C = odleglosc(a, c);
    double x = (A + B + C) / 2;

    return sqrt(x * (x - A) * (x - B) * (x - C));
}

bool czyRownolegle(odcinek c, odcinek d)
{
    double m1 = (c.getP1().getY() - c.getP2().getY()) / (c.getP1().getX() - c.getP2().getX());
    double m2 = (d.getP1().getY() - d.getP2().getY()) / (d.getP1().getX() - d.getP2().getX());

    if (m1 == m2)
        return true;
    else
        return false;
}

bool czyProstopoadle(odcinek c, odcinek d)
{
    double m1 = (c.getP1().getY() - c.getP2().getY()) / (c.getP1().getX() - c.getP2().getX());
    double m2 = (d.getP1().getY() - d.getP2().getY()) / (d.getP1().getX() - d.getP2().getX());

    if (m1 == (-1 / m2))
        return true;
    else
        return false;
}

int orientation(punkt a, punkt b, punkt c)
{
    int value = (b.getY() - a.getY()) * (c.getX() - b.getX()) - (b.getX() - a.getX()) * (c.getY() - b.getY());

    if (value == 0)
        return 0; // colinear
    else if (value > 0)
        return 1; // clockwise
    else
        return 2; // counter-clockwise
}

bool czyPrzecinaja(odcinek c, odcinek d)
{
    punkt p1 = c.getP1();
    punkt q1 = c.getP2();
    punkt p2 = d.getP1();
    punkt q2 = d.getP2();
    
    int o1 = orientation(p1, q1, p2);
    int o2 = orientation(p1, q1, q2);
    int o3 = orientation(p2, q2, p1);
    int o4 = orientation(p2, q2, q1);

    
    if (o1 != o2 && o3 != o4)
        return true;
    
    if (o1 == 0 && c.czyNalezy(p2))
        return true;
        
    if (o2 == 0 && c.czyNalezy(q2))
        return true;
        
    if (o3 == 0 && d.czyNalezy(p1))
        return true;
        
    if (o4 == 0 && d.czyNalezy(q1))
        return true;

    return false; // if it does not meet any condition above
}

bool czyTrojkatWTrojkacie(trojkat u, trojkat v)
{
    punkt a1 = u.getP1();
    punkt b1 = u.getP2();
    punkt c1 = u.getP3();
    punkt a2 = v.getP1();
    punkt b2 = v.getP2();
    punkt c2 = v.getP3();

    if (u.czyPunktNalezy(a2) && u.czyPunktNalezy(b2) && u.czyPunktNalezy(c2))
        return true;
    else if (v.czyPunktNalezy(a1) && v.czyPunktNalezy(b1) && v.czyPunktNalezy(c1))
        return true;
    else
        return false;
}

bool czyTrojkatyPrzecinaja(trojkat u, trojkat v)
{
    if (czyTrojkatWTrojkacie(u, v))
        return false;

    punkt a1 = u.getP1();
    punkt b1 = u.getP2();
    punkt c1 = u.getP3();
    punkt a2 = v.getP1();
    punkt b2 = v.getP2();
    punkt c2 = v.getP3();

    odcinek o1(a1, b1);
    odcinek o2(b1, c1);
    odcinek o3(a1, c1);
    odcinek o4(a2, b2);
    odcinek o5(b2, c2);
    odcinek o6(a2, c2);

    if (czyPrzecinaja(o1, o4) || czyPrzecinaja(o1, o5) || czyPrzecinaja(o1, o6))
        return true;
    if (czyPrzecinaja(o2, o4) || czyPrzecinaja(o2, o5) || czyPrzecinaja(o2, o6))
        return true;
    if (czyPrzecinaja(o3, o4) || czyPrzecinaja(o3, o5) || czyPrzecinaja(o3, o6))
        return true;

    return false;

}