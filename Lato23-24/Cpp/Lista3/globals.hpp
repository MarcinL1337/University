#include <cmath>
#include <exception>
#include <iostream>

#include "figury.hpp"

double distance(punkt a, punkt b);
double trojkatArea(punkt a, punkt b, punkt c); // for trojkat::isInside function
bool areParallel(odcinek c, odcinek d);
bool arePerpendicular(odcinek c, odcinek d);
int orientation(punkt a, punkt b, punkt c); // for line segment intersection
bool doIntersect(odcinek c, odcinek d);
bool istrojkatInsideAnother(trojkat u, trojkat v);
bool dotrojkatsIntersect(trojkat u, trojkat v);