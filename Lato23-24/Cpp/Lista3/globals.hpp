#include <cmath>
#include <exception>
#include <iostream>


double odleglosc(punkt a, punkt b);
double poleTrojkata(punkt a, punkt b, punkt c);
bool czyRownolegle(odcinek c, odcinek d);
bool czyProstopoadle(odcinek c, odcinek d);
int orientation(punkt a, punkt b, punkt c);
bool czyPrzecinaja(odcinek c, odcinek d);
bool czyTrojkatWTrojkacie(trojkat u, trojkat v);
bool czyTrojkatyPrzecinaja(trojkat u, trojkat v);