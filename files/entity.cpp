#include "entity.h"

Entity::Entity(std::string path){
    texture.loadFromFile(path);
    sprite.setTexture(texture);
}
