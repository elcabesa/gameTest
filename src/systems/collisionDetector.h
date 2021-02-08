#ifndef COLLISION_DETECTOR_H
#define COLLISION_DETECTOR_H

#include "SFML/Graphics.hpp"
#include "entt/src/entt/fwd.hpp"

class CollisionDetector {
public:
    CollisionDetector(entt::registry& reg);

    const std::vector<sf::RectangleShape>& getDebugRects() const;
    void calcCollision();
    

private:
    entt::registry& _reg;
    std::vector<sf::RectangleShape> _quadTreeRects;
};

#endif