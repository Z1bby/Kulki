#include <SFML/Graphics.hpp>

#ifndef __InstallingSFML__entity__
#define __InstallingSFML__entity__

class Entity{
public:
    sf::Texture texture;
    sf::Sprite sprite;
    Entity(std::string path);
};
#endif
