#include <iostream>
#include <math.h>

class piksel{
    protected:
        const static int szer = 1920, wys = 1080;
        int x, y;

    public:
        piksel();
        piksel(int, int);
        float calc_dist(int, int);

        int get_x();
        int get_y();

        

        int calc_dist_left();
        int calc_dist_right();
        int calc_dist_up();
        int calc_dist_down();
};


class pikselkolorowy : public piksel {
    private:
        std::string kolor;

    public:
        pikselkolorowy();
        pikselkolorowy(int, int, std::string);
        
        void przesun(int, int);

};


int pixel_distance(piksel &p1, piksel &p2);
int pixel_distance(piksel *p1, piksel *p2);