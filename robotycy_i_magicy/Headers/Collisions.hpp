#ifndef COLLISIONS_HPP_INCLUDED
#define COLLISIONS_HPP_INCLUDED

namespace Collision
{
    template<typename T>
    bool checkIfRangesOverlap(T minA, T maxA, T minB, T maxB)
    {
        return minB < maxA && minA < maxB;
    }
    template<typename T>
    bool checkIfRangeIsEnclosed(T minA, T maxA, T minB, T maxB)
    {
        return maxA < maxB && minA > maxB;
    }
    template<typename T>
    bool checkIfRangesOverlapClosed(T minA, T maxA, T minB, T maxB)
    {
        return minB <= maxA && minA <= maxB;
    }
    
    template<typename T>
    T checkHowRangesOverlap(T minA, T maxA, T minB, T maxB)
    {
        T toRight = maxB - minA;
        T toLeft  = maxA - minB;
        return (toRight < toLeft) ? toRight : -toLeft;
    }
    
    template<typename T>
    bool checkIfRectsOverlap(sf::Rect<T> a, sf::Rect<T> b)
    {
        return  checkIfRangesOverlap(a.left, a.left + a.width, b.left, b.left + b.width) &&
                checkIfRangesOverlap(a.top,  a.top + a.height, b.top,  b.top + b.height);
    }
    
    template<typename T>
    bool checkIfRectIsEnclosed(sf::Rect<T> a, sf::Rect<T> b)
    {
        return  checkIfRangeIsEnclosed(a.left, a.left + a.width, b.left, b.left + b.width) &&
                checkIfRangeIsEnclosed(a.top,  a.top + a.height, b.top,  b.top + b.height); 
    }
    
    template<typename T>
    Vector2<T> getRectsOverlaps(sf::Rect<T> a, sf::Rect<T> b)
    {
        return Vector2<T>(checkHowRangesOverlap(a.left, a.left + a.width, b.left, b.left + b.width),
                          checkHowRangesOverlap(a.top,  a.top + a.height, b.top,  b.top + b.height));
    }
    
    template<typename T>
    Vector2<T> getRectsOverlapMTV(sf::Rect<T> a, sf::Rect<T> b)
    {
        Vector2<T> temp = getRectsOverlaps(a, b);
        if(std::abs(temp.x) < std::abs(temp.y))
        {
            return Vector2<T>(temp.x, 0);
        }
        return Vector2<T>(0, temp.y);
    }
    
    
};


#endif // COLLISIONS_HPP_INCLUDED
