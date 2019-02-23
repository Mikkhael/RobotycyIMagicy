#ifndef LINECASTER_HPP_INCLUDED
#define LINECASTER_HPP_INCLUDED

#include "Vectors.hpp"
#include "Map.hpp"

class LineCast
{
    
public:
    
    struct Result
    {
        bool hit;
        Vector2d contactPoint;
    };
    
    static Result castWithRect(const Vector2d& from, Vector2d to, sf::FloatRect rect)
    {
        Collision::LineIntersection tempLineIntersection;
        bool hit = true;
        if(to.x > from.x)
        {
            tempLineIntersection = Collision::intersectLines(from, to, {rect.left, rect.top}, {rect.left, rect.top + rect.height});
        }
        else
        {
            tempLineIntersection = Collision::intersectLines(from, to, {rect.left + rect.width, rect.top}, {rect.left + rect.width, rect.top + rect.height});
        }
        
        if(tempLineIntersection.areLinesIntersecting)
        {
            hit = false;
            to = tempLineIntersection.intersection;
        }
        
        if(to.y > from.y)
        {
            tempLineIntersection = Collision::intersectLines(from, to, {rect.left, rect.top}, {rect.left + rect.width, rect.top});
        }
        else
        {
            tempLineIntersection = Collision::intersectLines(from, to, {rect.left, rect.top + rect.height}, {rect.left + rect.width, rect.top + rect.height});
        }
        
        if(tempLineIntersection.areLinesIntersecting)
        {
            hit = false;
            to = tempLineIntersection.intersection;
        }
        
        return {hit, to};
    }
    
    static Result castOverMap(const Vector2d& from, Vector2d to, Map& map)
    {
        
        Vector2i fromCoords = map.positionToCoords(from);
        Vector2i toCoords = map.positionToCoords(to);
        
        
        Vector2i testBoxStart(std::min(fromCoords.x, toCoords.x), std::min(fromCoords.y, toCoords.y));
        Vector2i testBoxEnd(std::max(fromCoords.x, toCoords.x)+1, std::max(fromCoords.y, toCoords.y)+1);
        
        Collision::LineIntersection tempLineIntersection;
        
        bool hit = false;
        
        for(int i=std::max(testBoxStart.y, 0); i < testBoxEnd.y; i++)
        {
            for (int j=std::max(testBoxStart.x, 0); j < testBoxEnd.x ; j++)
            {
                if(!map.isTileOpaque({j, i}))
                {
                    continue;
                }
                sf::FloatRect tileRect = map.getTileRect({j, i});
                Result res = castWithRect(from, to, tileRect);
                
                if(!res.hit)
                {
                    hit = false;
                    to = res.contactPoint;
                }
            }
        }
        
        return {hit, to};
        
    }
    
};

#endif // LINECASTER_HPP_INCLUDED
