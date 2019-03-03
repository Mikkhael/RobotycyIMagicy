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
    
    static Result castOverMap(const Vector2d& from, Vector2d to, Map& map, bool abortOnHit = false)
    {
        
        Vector2i fromCoords = map.positionToCoords(from);
        Vector2i toCoords = map.positionToCoords(to);
        
        
        Vector2i testBoxStart(std::min(fromCoords.x, toCoords.x), std::min(fromCoords.y, toCoords.y));
        Vector2i testBoxEnd(std::max(fromCoords.x, toCoords.x)+1, std::max(fromCoords.y, toCoords.y)+1);
        
        Collision::LineIntersection tempLineIntersection;
        
        unsigned int endY = std::min(map.getSize().y, static_cast<unsigned int>(std::max(testBoxEnd.y, 0)));
        unsigned int endX = std::min(map.getSize().x, static_cast<unsigned int>(std::max(testBoxEnd.x, 0)));
        
        bool hit = true;
        
        for(unsigned int i=std::max(testBoxStart.y, 0); i < endY; i++)
        {
            for (unsigned int j=std::max(testBoxStart.x, 0); j < endX ; j++)
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
                    if(abortOnHit)
					{
						return {hit, to};
					}
                }
            }
        }
        
        return {hit, to};
        
    }
    
};

class Observer
{
	
	static const sf::Color viewColor;
	static constexpr unsigned int vertexCount = 32;
	
	sf::VertexArray viewField;
	
public:
	double closeupRadius;
	double viewDistance;
	double viewAngle;
	double viewAngleCos;
	
	virtual Vector2d getForewardVector() const = 0;
	virtual Vector2d getCastBeginPosition() const = 0;
	
	Observer(double _closeupRadius = 0, double _viewDistance = 0, double _viewAngle = 0)
		: viewField(sf::PrimitiveType::TriangleFan, vertexCount), closeupRadius(_closeupRadius), viewDistance(_viewDistance)
	{
		setViewAngle(_viewAngle);
		for(unsigned int i=0u; i<vertexCount; i++)
		{
			viewField[i].color = viewColor;
		}
	}
	
	void drawViewField(sf::RenderTarget& target, sf::RenderStates states)
	{
		Vector2d position = getCastBeginPosition();
		viewField[0].position = position;
		double angleStep = 2 * viewAngle / (vertexCount-2);
		double tempAngle = -viewAngle;
		Vector2d foreward = getForewardVector() * viewDistance;
		for(unsigned int i = 1u; i<vertexCount; i++)
		{
			viewField[i].position = position + foreward.rotate(tempAngle);
			tempAngle += angleStep;
		}
		target.draw(viewField, states);
	}
	
	void setViewAngle(double angle)
	{
		viewAngle = angle;
		viewAngleCos = std::cos(toRadians(angle));
	}
	
	bool isInView(const Vector2d& target)
	{
		Vector2d toTarget = target - getCastBeginPosition();
		if(toTarget.magnatudeSquared() > viewDistance*viewDistance)
		{
			return false;
		}
		return toTarget.normalize().dot(getForewardVector()) >= viewAngleCos;
	}
	
	bool isInCloseup(const Vector2d& target)
	{
		Vector2d toTarget = target - getCastBeginPosition();
		return toTarget.magnatudeSquared() <= closeupRadius*closeupRadius;
	}
	
	bool canSeeOverRect(const Vector2d& to, const sf::FloatRect& rect)
	{
		LineCast::Result res = LineCast::castWithRect(getCastBeginPosition(), to, rect);
		return res.hit;
		
	}
	bool canSeeOverMap(const Vector2d& to, Map& map)
	{
		LineCast::Result res = LineCast::castOverMap(getCastBeginPosition(), to, map, true);
		return res.hit;
	}
};

const sf::Color Observer::viewColor{255, 0, 0, 30};

#endif // LINECASTER_HPP_INCLUDED
