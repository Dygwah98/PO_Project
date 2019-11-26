#ifndef LINE_HPP
#define LINE_HPP

#include"gameobject.h"

class Line {

    private:
        LineImage* img;
        std::vector<GameObject*> objects;
        float position;
        float speed;

    protected:
    
    public:
        Line();
        Line(ALLEGRO_BITMAP*, int);
        ~Line();

        void update();
        void redraw();
        void reset();

        Collision collides(const GameObject&) const;

        float get_speed() const;
};

#endif