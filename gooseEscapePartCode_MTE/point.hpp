#ifndef POINT_HPP
#define POINT_HPP

class Point
{
    private:
        int x, y;
    public:
        Point(int x, int y)
        {
            this->x = x;
            this->y = y;
        }

        int getX() const
        {
            return this->x;
        }

        int getY() const
        {
            return this->y;
        }

        bool operator==(Point const &rhs)
        {
            return (this->x == rhs.x && this->y == rhs.y);
        }
};
#endif