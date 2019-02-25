#include "../Headers/Actor.hpp"

bool moveOutOfWalls()
{
    sf::FloatRect actorRect(getPosition() - size*0.5, size);
    Vector2u actorCoords = scene->map.positionToCoords(getPosition());
    
    if(scene->map.areCoordsGood(actorCoords))
    {
        sf::FloatRect tileRect = scene->map.getTileRect(actorCoords);
        
        if(!scene->map.isTileWalkable(actorCoords))
        {
            Vector2d toMove = Collision::getRectsOverlapMTV(actorRect, tileRect);
            
            Vector2u newCoords = actorCoords + toMove.normalize();
            if(!scene->map.isTileWalkable(newCoords))
            {
                return false;
            }
            move(toMove);
            actorRect = {getPosition() - size*0.5, size};
            actorCoords = newCoords;
            
        }
        
    }
    
    for(int i=-1; i<=1; i++)
    {
        for(int j=-1; j<=1; j++)
        {
            if(i == 0 && j == 0)
            {
                continue;
            }
            Vector2u newCoords = actorCoords + Vector2u(j, i);
            if(scene->map.areCoordsGood(newCoords) && !scene->map.isTileWalkable(newCoords) && Collision::checkIfRectsOverlap(actorRect, scene->map.getTileRect(newCoords)))
            {
                Vector2f toMove = Collision::getRectsOverlapMTV(actorRect, scene->map.getTileRect(newCoords));
                actorRect = {{actorRect.left + toMove.x, actorRect.top + toMove.y}, size};
            }
        }
    }
    
    Vector2d newPosition(actorRect.left + size.x*0.5, actorRect.top + size.y*0.5);
    if(scene->map.isTileWalkable(scene->map.positionToCoords(newPosition)))
    {
        setPosition(newPosition);
        return true;
    }
    
    return false;
}
