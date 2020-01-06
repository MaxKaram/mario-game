#include "map.h"
#include "textures.h"
#include <cmath>

enum {left = 71, right};

class animation {
    protected:
        sf::Sprite person;
        std::vector<sf::IntRect> framesArr = {};

        int frame = 0, countFrames = 0, type = heroType, time = 0, rotation = 0, rotationSpeed = 0;

        float speedX = 0, boostX = 0, speedY = 0, boostY = 0, x = 100, y = 296, maxSpeedX = 3, maxSpeedY = 15;
        float endX = -1, endY = -1;

        bool leftIntersect = false, rightIntersect = false, upIntersect = false, downIntersect = false;
        bool endAnimation = false, isEmpty = false;
    public:
        animation(sf::Texture* texture ,float speedX, float speedY, float x, float y, int type, int time,
            float endX = -1, float endY = -1, float boostX = 0, float boostY = 0, int rotationSpeed = 0) {
                
            this->speedX = speedX;
            this->speedY = speedY;
            this->x = x;
            this->y = y;
            this->framesArr = textureArrs[type];
            this->countFrames = framesArr.size();
            this->endX = endX;
            this->endY = endY;
            this->type = type;
            this->boostX = boostX;
            this->boostY = boostY;
            this->time = time;
            this->rotationSpeed = rotationSpeed;

            person.setTexture(*texture);
            person.setTextureRect(framesArr[0]);
            person.setPosition(x, y);

            if(type == enemy1 || type == gameOverAnimation || type == bittedEnemy1 || type == rotatedEnemy1) person.setScale(2, 2); 
            if(type == fireboll) maxSpeedX = 12;
        }
        animation(bool isEmpty) {
            this->isEmpty = isEmpty;
        }

        animation() {}

        void draw(sf::RenderWindow* window) {
            window->draw(person);
        }

        float getEndY() {
            return endY;
        }

        bool animationIsEmpty() {return isEmpty;}

        void invertSpeedX() {
            this->speedX = -this->speedX;
            this->boostX = -this->boostX;
            this->x += this->speedX;
        }

        virtual void changeStates(sf::FloatRect* blockCoords, bool changeCoords, char& blockType) {
            if(((leftIntersect && speedX > 0) || (rightIntersect && speedX < 0)) && type != wallChunk && changeCoords) {
                if(type == fireboll) endAnimation = true;
                boostX = -boostX;
                speedX = -speedX;
            }

            if(downIntersect && changeCoords && endY == -1 && type != fireboll) {
                y = blockCoords->top + 30;
                speedY = 1;
            }
            if(upIntersect && changeCoords && endY == -1) {
                boostY = 0;
                sf::FloatRect coords = person.getGlobalBounds();
                if(type == fireboll) {
                    speedY = -10;
                } 
                y = blockCoords->top - textureArrs[type][0].height - 1;
                if(type == enemy1) y -= textureArrs[type][0].height;
            }
        }

        void getSideX(sf::FloatRect* block, sf::FloatRect* hero, char& sym, bool changeCoords) {
            if(hero->top + hero->height <= block->top || block->top + block->height - 20 <= hero->top) return;

            if(hero->left < block->left && hero->left + hero->width > block->left && !rightIntersect) {
                leftIntersect = true;
                changeStates(block, changeCoords, sym);
            }
            else if(block->left < hero->left && block->left + block->width > hero->left && !leftIntersect) {
                rightIntersect = true;
                changeStates(block, changeCoords, sym);
            }
        }

        void getSideY(sf::FloatRect* block, sf::FloatRect* hero, char& sym , bool changeCoords) {
            if(hero->left + hero->width <= block->left || hero->left >= block->left + block->width) return;

            if(hero->top + hero->height > block->top && hero->top < block->top) {
                upIntersect = true;
                if(changeCoords) changeStates(block, changeCoords, sym);
            } else if(hero->top >= block->top && hero->top <= block->top + block->height && !downIntersect) {
                downIntersect = true;
                if(changeCoords) changeStates(block, changeCoords, sym);
            }
        }

        bool isEnd() {
            return endAnimation;
        }

        void close() {
            endAnimation = true;
        }

        int getType() {
            return type;
        }

        sf::FloatRect getBounds() {
            sf::FloatRect bounds = person.getGlobalBounds();
            if(rotationSpeed != 0) {
                bounds.top -= bounds.height / 2;
                bounds.left -= bounds.width / 2;
            }
            return bounds;
        }

        void checkMapItersects(sf::Sprite *hero, bool changeCoords = true) {
            sf::FloatRect heroCoords = hero->getGlobalBounds();
            heroCoords.top += speedY;
            heroCoords.left += speedX;

            if(rotationSpeed != 0) {
                heroCoords.top -= heroCoords.height / 2;
                heroCoords.left -= heroCoords.width / 2;
            }

            if(heroCoords.top > 530) endAnimation = true;
            if(heroCoords.top + heroCoords.height > 450) return;

            if(heroCoords.left == 0) return;

            sf::FloatRect blockCoords;
            
            int fromX = heroCoords.left / 32 - 2;
            int toX = (heroCoords.left + heroCoords.width) / 32 + 2;

            int fromY = heroCoords.top / 30 - 5;
            int toY = (heroCoords.top + heroCoords.width) / 30 + 1;

            leftIntersect = false, rightIntersect = false, upIntersect = false, downIntersect = false;

            for(int i = fromY; i < toY; ++i) {
                for(int j = fromX; j < toX; j++) {
                    if(map[i][j] != ' ' && map[i][j] != 'k' && map[i][j] != 'K' && map[i][j] != 'o') {

                        int blockWidth = map[i][j] == 'g' || map[i][j] == 'w' || 
                                         map[i][j] == 'b' || map[i][j] == 'q' || 
                                         map[i][j] == 'd' ? 32 : 64;
                        
                        int blockHeight = map[i][j] == 'L' ? 64 : 
                                          map[i][j] == 'M' ? 96 : 
                                          map[i][j] == 'H' ? 128 : 32;


                        blockCoords = sf::FloatRect(j * 32, i * 30 + 50, blockWidth, blockHeight);
                        // if(map[i][j + 1] == 'q' || map[i][j + 1] == 'w') blockCoords.width -= 20;

                        if(heroCoords.intersects(blockCoords)) {
                            getSideY(&blockCoords, &heroCoords, map[i][j] , changeCoords);
                            heroCoords.top -= speedY;
                            if(type == fireboll && upIntersect) heroCoords.top += speedY - 15;

                            getSideX(&blockCoords, &heroCoords, map[i][j], changeCoords);
                        }
                    }

                }
            }
        }

        void checkEnd() {
            if(endX == -1 || endY == -1) return;
            bool checkY = false;
            bool checkX = false;

            if(boostY > 0) {
                checkY = y >= endY;
            } else if(boostY < 0) {
                checkY = y <= endY;
            } else {
                checkY = speedY <= 0 && y <= endY || speedY >= 0 && y >= endY; 
            }
            
            if(boostX > 0) {
                checkX = x >= endX;
            } else if(boostX < 0) {
                checkX = x <= endX;
            } else {
                checkX = speedX <= 0 && x <= endX || speedX >= 0 && x >= endX; 
            }

            endAnimation = checkY && checkX;
            if(endAnimation) {
                x = endX, y = endY;
            }
        }

        virtual void move(int milliseconds) {
            if(milliseconds % time == 0) {
                frame += frame == countFrames - 1 ? -(countFrames - 1) : 1;
            }
            if(endAnimation) return;

            if(speedX > 0) {
                x += speedX;
                speedX = speedX >= maxSpeedX ? maxSpeedX : speedX + boostX;
            }
            if(speedX < 0) {
                x += speedX;
                speedX = speedX <= -maxSpeedX ? -maxSpeedX : speedX + boostX;
            }

            if(rotationSpeed != 0) {
                sf::FloatRect coords = person.getGlobalBounds();
                person.setOrigin(coords.width / 2, coords.height / 2);
                person.setRotation(rotation);
                rotation += rotationSpeed;
            }

            checkMapItersects(&person);

            if(upIntersect && type == fireboll && std::fabs(423 - y) < 0.001) {
                y = 393;
            }

            speedY += 3;
            if(!upIntersect && endY == -1) boostY = 5;
            speedY -= 3;

            if((boostY != 0 && !upIntersect) || endY != -1) {
                y += speedY;
                speedY = speedY >= maxSpeedY ? maxSpeedY : speedY + boostY; 
            } else if(boostY != 0) {
                boostY = 0;
                speedY = 0;
            }

            checkEnd();

            person.setTextureRect(framesArr[frame]);
            person.setPosition(x, y);
        }
};