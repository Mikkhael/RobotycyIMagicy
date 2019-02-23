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
    
    static Result cast(const Vector2d& from, Vector2d to, Map& map)
    {
        
        Vector2i fromCoords = map.positionToCoords(from);
        Vector2i toCoords = map.positionToCoords(to);
        
        Vector2d lineVector = to - from;
        
        Vector2i testBoxStart(std::min(fromCoords.x, toCoords.x), std::min(fromCoords.y, toCoords.y));
        Vector2i testBoxEnd(std::max(fromCoords.x, toCoords.x)+1, std::max(fromCoords.y, toCoords.y)+1);
        
        Collision::LineIntersection tempLineIntersection;
        
        bool hit = true;
        
        for(int i=std::max(testBoxStart.y, 0); i < testBoxEnd.y; i++)
        {
            for (int j=std::max(testBoxStart.x, 0); j < testBoxEnd.x ; j++)
            {
                if(!map.isTileOpaque({j, i}))
                {
                    continue;
                }
                sf::FloatRect tileRect = map.getTileRect({j, i});
                if(lineVector.x > 0)
                {
                    tempLineIntersection = Collision::intersectLines(from, to, {tileRect.left, tileRect.top}, {tileRect.left, tileRect.top + tileRect.height});
                }
                else
                {
                    tempLineIntersection = Collision::intersectLines(from, to, {tileRect.left + tileRect.width, tileRect.top}, {tileRect.left + tileRect.width, tileRect.top + tileRect.height});
                }
                
                if(tempLineIntersection.areLinesIntersecting)
                {
                    hit = false;
                    to = tempLineIntersection.intersection;
                }
                
                if(lineVector.y > 0)
                {
                    tempLineIntersection = Collision::intersectLines(from, to, {tileRect.left, tileRect.top}, {tileRect.left + tileRect.width, tileRect.top});
                }
                else
                {
                    tempLineIntersection = Collision::intersectLines(from, to, {tileRect.left, tileRect.top + tileRect.height}, {tileRect.left + tileRect.width, tileRect.top + tileRect.height});
                }
                
                
                if(tempLineIntersection.areLinesIntersecting)
                {
                    hit = false;
                    to = tempLineIntersection.intersection;
                }
                
            }
        }
        
        return {hit, to};
        
    }
    
};

#endif // LINECASTER_HPP_INCLUDED
