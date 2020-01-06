#include "animation.h"

class Game : animation {
    private:
        sf::Sprite hero;
        sf::Sprite block;
        sf::Texture blockTexture;
        sf::Texture heroTexture;
        std::vector<animation> animationsArr = {};

        int vectorX = 0, vectorY = 0, start = 83, questionFrame = 0, countAnimations = 0, countEmptyAnimations = 0;
        int marioLevel = 0, timer = 0, reload = 0;
        float viewX = 0, maxSpeedX = 6, maxSpeedY = 12;
        bool changeFrame = false, boostMode = false, invulnerability = false, gameOver = false;
    public: 
        Game(sf::Texture* blockTex, sf::Texture* heroTex) {
            block.setTexture(*blockTex);
            hero.setTexture(*heroTex);
            hero.setScale(2, 2);

            blockTexture = *blockTex;
            heroTexture = *heroTex;
        }

        void renderMap(sf::RenderWindow* window) {
            int from = viewX / 32 - 4;
            from = from < 0 ? 0 : from;
            int to = from + 21;

            to == to > 200 ? 200 : to;

            for(int i = 0; i < 13; ++i) {
                for(int j = from; j < to; ++j) {
                    if(map[i][j] != ' ' && (map[i][j] != 'k' && map[i][j] != 'o' && map[i][j] != 'K' && map[i][j])) {
                        sf::IntRect textureRect;
                        int index = map[i][j] == 'g' ? ground         : map[i][j] == 'w' ? wall :
                                    map[i][j] == 'L' ? lowPipe        : map[i][j] == 'M' ? mediumPipe :
                                    map[i][j] == 'H' ? highPipe       : map[i][j] == 'b' ? BLOCK :
                                    map[i][j] == 'd' ? disabled : questionSymbol;

                        if(map[i][j] == 'e') {
                            map[i][j] = ' ';
                            countAnimations++;
                            animationsArr.push_back(animation(&heroTexture, -3, 0, j * 32, i * 30 + 48, enemy1, 140));
                        }

                        textureRect = blockSizes[index];

                        if(index == questionSymbol) {
                            int CurrentFrame = questionFrame > 2 ? 2 - questionFrame % 3 - 1 : questionFrame;
                            textureRect.top += 32 * CurrentFrame;
                        }

                        block.setTextureRect(textureRect);
                        block.setPosition(j * 32, i * 30 + 50);
                        window->draw(block);
                    }
                }
            }
        }

        void renderDecoration(sf::RenderWindow* window) {
            int from = viewX / 32 - 4;
            from = from < 0 ? 0 : from;
            int to = from + 21;

            to == to > 200 ? 200 : to;

            for(int i = 0; i < 13; ++i) {
                for(int j = from; j < to; ++j) {
                    if(map[i][j] != ' ' && (map[i][j] == 'o' || map[i][j] == 'k' || map[i][j] == 'K')) {
                        sf::IntRect textureRect;
                        if(map[i][j - 1] == map[i][j] && (map[i][j] == 'k' || map[i][j] == 'K' || map[i][j] == 'o')) continue;

                        int index = map[i][j] == 'o' ? oblako : map[i][j] == 'K' ? bigKyst : kyst;
                        
                        if(map[i][j] == 'k' && map[i][j + 1] == 'k' && map[i][j + 2] == 'k') index = tripleKyst;
                   else if(map[i][j] == 'k' && map[i][j + 1] == 'k')  index = doubleKyst;

                        if(map[i][j] == 'o' && map[i][j + 1] == 'o' && map[i][j + 2] == 'o') index = tripleOblako;      
                   else if(map[i][j] == 'o' && map[i][j + 1] == 'o')  index = doubleOblako;   

                        if(map[i][j] == 'K' && map[i][j + 1] == 'K')  index = superBigKyst;

                        int yCoord = map[i][j] == 'o' ? i * 30 - 19 : map[i][j] == 'K' ? i * 30 - 8 : i * 30; 
                        textureRect = blockSizes[index];

                        block.setTextureRect(textureRect);
                        block.setPosition(j * 32, yCoord + 50);
                        window->draw(block);
                    }
                }
            }
        }

        void renderAnimations(sf::RenderWindow* window) {
            for(int i = 0; i < countAnimations; i++) {
                sf::FloatRect personCoords = animationsArr[i].getBounds();
                if(personCoords.left + personCoords.width < viewX || personCoords.left > viewX + 512) continue;
                animationsArr[i].draw(window);
            }
        }

        bool isGameOver() {return gameOver;}

        void changeQuestionFrame(int time) {
            if(time % 350 == 0) questionFrame += questionFrame == 4 ? -3 : 1;
        }

        void drawHero(sf::RenderWindow* window) {
            if(invulnerability) hero.setColor(sf::Color(210, 210, 210, 255));
            else hero.setColor(sf::Color(255, 255, 255, 255));
            window->draw(hero);
        }

        void setView(sf::View* view, sf::RenderWindow* window) {
            view->reset(sf::FloatRect(viewX, 50, 512, 550));
            window->setView(*view);
        }

        void changeStates(sf::FloatRect* blockCoords, bool changeCoords, char& blockType) override {
            if(leftIntersect && changeCoords) {
                speedX = 0.25;
                x = blockCoords->left - 34;
            }
            if(rightIntersect && changeCoords) {
                speedX = -0.25;
            }
            if(downIntersect && changeCoords) {
                y = blockCoords->top + 30;
                speedY = 1;

                if(blockType != 'd' && (blockType == 'q' || blockType == 'G' || blockType == 'm')) {
                    countAnimations++;
                    int x = blockCoords->left;
                    int y = blockCoords->top;
                    int type = blockType == 'q' ? blockDmoney : blockDmushroom;

                    blockType = ' ';
                    
                    animation block(&blockTexture, 0, -3, x, y - 1, type, 280, x, y, 0, 1);
                    animationsArr.push_back(block);
                } else if(blockType == 'w') {
                    int x = blockCoords->left;
                    int y = blockCoords->top;
                    int type;
                    if(marioLevel > 0) {
                        countAnimations += 4;
                        animationsArr.push_back(animation(&blockTexture, -1, -20, x, y, wallChunk, 700, x - 20, y + 100, -0.2, 3, -30)); // левый верхний кусок стены
                        animationsArr.push_back(animation(&blockTexture, 1, -20, x + 18, y, wallChunk, 700, x + 20, y + 100, 0.2, 3, 30)); // правый верхний кусок стены
                        animationsArr.push_back(animation(&blockTexture, -1, -10, x, y + 18, wallChunk, 700, x - 20, y + 100, -0.2, 3, -30)); // левый нижний кусок стены
                        animationsArr.push_back(animation(&blockTexture, 1, -10, x + 18, y + 18, wallChunk, 700, x + 20, y + 100, 0.2, 3, 30)); // правы нижний кусок стены
                    } else {
                        countAnimations++;
                        animationsArr.push_back(animation(&blockTexture, 0, -3, x, y - 1, blockW, 280, x, y, 0, 1));
                    }
                    blockType = ' ';
                }
            }
            if(upIntersect && changeCoords) {
                boostY = 0;
                y = blockCoords->top - heroTexturesArr[marioLevel].height * 2;
            }
        }

        void checkAnimationItersects() {
            sf::FloatRect heroCoords = hero.getGlobalBounds();
            sf::FloatRect personCoords;
            int currentType = heroType;


            for(int j = -1; j < countAnimations; ++j) {
                leftIntersect = false, rightIntersect = false, upIntersect = false, downIntersect = false;
                if(j != -1) {
                    heroCoords = animationsArr[j].getBounds();
                    currentType = animationsArr[j].getType();
                }
                for(int i = 0; i < countAnimations; i++) {
                    if(animationsArr[i].animationIsEmpty() || i == j) continue;
                    personCoords = animationsArr[i].getBounds();


                    if(heroCoords.intersects(personCoords)) {
                        int type = animationsArr[i].getType();


                        if((type == mushroom || type == flower) && currentType == heroType) {
                            marioLevel += marioLevel < 2 ? 1 : 0;
                            start = heroTexturesArr[marioLevel].y;
                            animationsArr[i].close();
                        }

                        if(type == enemy1 && !gameOver) {
                            heroCoords.left += speedX;
                            heroCoords.top += speedY;

                            sf::FloatRect blockCoords = animationsArr[i].getBounds();
                            char sym = 'e';
                            if(heroCoords.height > personCoords.height) {
                                heroCoords.top += (heroCoords.height - personCoords.height) + 1;
                                heroCoords.height -= heroCoords.height - personCoords.height + 1;
                            }

                            if(currentType == heroType) getSideY(&blockCoords, &heroCoords, sym, true);
                            else getSideY(&blockCoords, &heroCoords, sym, false);
                            heroCoords.left -= speedX;
                            if(upIntersect && currentType == heroType) {
                                int x = personCoords.left;
                                int y = personCoords.top;
                                animationsArr[i] = animation(&heroTexture, 0, -0.05, x, y + 18, bittedEnemy1, 140, x, y + 17);
                                speedY = -14;
                            };
                            heroCoords.left += speedX;
                            heroCoords.top -= speedY;
                            if(!upIntersect || currentType != heroType) getSideX(&blockCoords, &heroCoords, sym, false);

                            if(currentType == fireboll || (downIntersect && (currentType == blockW || currentType == blockDmushroom || currentType == blockDmoney || currentType == wallChunk))) {
                                if(currentType == fireboll) {
                                    animationsArr[j].close();
                                }
                                int xCoord = personCoords.left;
                                int yCoord = personCoords.top;
                                animationsArr[i] = animation(&heroTexture, 0, -12, xCoord, yCoord, rotatedEnemy1, 1050, xCoord, yCoord + 100, 0, 2);
                            }

                            if((leftIntersect || rightIntersect) && !invulnerability) {
                                if(marioLevel == 0 && currentType == heroType) {
                                    float x = heroCoords.left;
                                    float y = heroCoords.top;
                                    gameOver = true;
                                    countAnimations++;
                                    animationsArr.push_back(animation(&heroTexture, 0, -20, x, y - 2, gameOverAnimation, 1050, x, y + 300, 0, 3));
                                }
                                else if(currentType == heroType)  {
                                    invulnerability = true;
                                    timer = 3150;
                                    marioLevel = 0;
                                } else if(currentType == enemy1) animationsArr[j].invertSpeedX();
                            }
                        }
                    }
                }
            }
        }

        void performAnimations(int time) {
            sf::FloatRect heroCoords = hero.getGlobalBounds();
            sf::FloatRect personCoords;
            sf::FloatRect animationCoords;

            for(int i = 0; i < countAnimations; i++) {
                if(animationsArr[i].animationIsEmpty()) continue;
                if(gameOver && (animationsArr[i].getType()) != gameOverAnimation) continue;
                personCoords = animationsArr[i].getBounds();
                int animationType = 0;
                if(personCoords.left < viewX - 100 || personCoords.left > viewX + 612) continue;
                
                animationsArr[i].move(time);
                if(animationsArr[i].isEnd()) {
                    animationType = animationsArr[i].getType();
                    animationCoords = animationsArr[i].getBounds();

                    if(animationType == mushroom && animationsArr[i].getEndY() != -1) {
                        animationsArr[i] = animation(&blockTexture, 4, 0, animationCoords.left, animationCoords.top, mushroom, 280);
                    } else if(animationType == blockW || animationType == blockDmoney || animationType == blockDmushroom) {
                        int xIndex = animationCoords.left / 32;
                        int yIndex = animationCoords.top / 30 - 1;

                        map[yIndex][xIndex] = animationType == blockW ? 'w' : 'd';
                        if(animationType == blockDmushroom) {
                            int x = personCoords.left;
                            int y = personCoords.top;
                            int type = marioLevel == 0 ? mushroom : flower;
                            animationsArr[i] = animation(&blockTexture , 0, -1, x, y, type, 35, x, y - 32);
                        }
                        else animationsArr[i] = animation(true);
                    } else if(animationType == flower && animationsArr[i].getEndY() != -1) {
                        animationsArr[i] = animation(&blockTexture, 0, 0, animationCoords.left, animationCoords.top, flower, 35);
                    } else if(animationType == fireboll) {
                        int x = personCoords.left;
                        int y = personCoords.top;
                        animationsArr[i] = animation(&blockTexture, 0.01, 0, x, y, explosion, 245, x + 0.10, y);
                    }
                    else animationsArr[i] = animation(true);
                }
            }
        }

        void move(int milliseconds) override {
            changeFrame = boostMode ? true : !changeFrame;
            if(vectorX != 0 && changeFrame) frame += frame == 3 ? -3 : 1;
            if(y > 530) gameOver = true;
            reload -= reload ? 35 : 0;

            int marioX = heroTexturesArr[marioLevel].x;
            int marioY = heroTexturesArr[marioLevel].y;
            int marioWidth = heroTexturesArr[marioLevel].width;
            int marioHeight = heroTexturesArr[marioLevel].height;
            int marioGapX = heroTexturesArr[marioLevel].gapX;
            int marioGapY = heroTexturesArr[marioLevel].gapY;

            speedX += speedX > 0 ? boostX + 2 : boostX;

            timer -= timer == 0 ? 0 : 35;
            invulnerability = timer == 0 ? false : true;
            checkAnimationItersects();
            checkMapItersects(&hero, false); 
            speedX -= speedX > 0 ? (boostX + 2) : boostX;

            if((speedX < 0 || (speedX == 0 && boostX < 0 && vectorX == left)) && !rightIntersect) {
                start = marioY + marioGapY;
                x += speedX;


                if(boostX > 0) {
                    speedX = (speedX + boostX) >= 0 ? 0 : speedX + boostX;
                }
                else speedX = speedX <= -maxSpeedX ? -maxSpeedX : speedX + boostX;

                boostX = speedX == 0 ? 0 : boostX;
                
            } else if((speedX > 0 ||(speedX == 0 && boostX > 0 && vectorX == 72)) && !leftIntersect) {
                start = marioY;
                x += speedX;

                if(boostX < 0) speedX = (speedX + boostX) <= 0 ? 0 : speedX + boostX;
                else speedX = speedX >= maxSpeedX ? maxSpeedX : speedX + boostX;

                boostX = speedX == 0 ? 0 : boostX;

            } else if(vectorX == 0) frame = 0;

            hero.setTextureRect(sf::IntRect(marioX + marioGapX * frame, start, marioWidth, marioHeight));
            checkMapItersects(&hero);

            speedY += 3;
            float copySpeedX = speedX;
            speedX = 0;
            checkMapItersects(&hero);
            if(!upIntersect && boostY == 0) boostY = 5;
            speedY -= 3;
            speedX = copySpeedX;

            if(boostY != 0 && !upIntersect) {
                y += speedY;
                speedY += speedY >= maxSpeedY ? 0 : boostY;
                boostY = speedY >= 0 ? 5 : boostY;

                marioX = heroTexturesArr[marioLevel].upX;
                marioWidth = heroTexturesArr[marioLevel].upWidth;
                marioHeight = heroTexturesArr[marioLevel].upHeight;

                hero.setTextureRect(sf::IntRect(marioX, start, marioWidth, marioHeight));
            } else if(boostY != 0) {
                boostY = 0;
                speedY = 0;
            }
            
            hero.setPosition(x, y);
            if(x > 224 && x < 6112) viewX = x - 224;
        }

        void setVector(int vector) {
            if(vectorX == vector || vectorY == vector) return;
            vectorX = vector == 71 || vector == 72 || vector == 0 ? vector : vectorX;
            if(vector == 25 && boostY == 0) {
                vectorY = vector;
                speedY = -20;
                boostY = 1.5;
            }
            if(vector == 23) {
                if(marioLevel >= 2 && !boostMode && !reload) {
                    reload = 315;
                    countAnimations++;

                    float animationsSpeedX = std::fabs(start - heroTexturesArr[marioLevel].y) < 0.001 ? 12 : -12;
                    float xCoord = std::fabs(start - heroTexturesArr[marioLevel].y) < 0.001 ? x + 35 : x - 15;
                    float yCoord = y + 15;
                    animationsArr.push_back(animation(&blockTexture, animationsSpeedX, 0, xCoord, yCoord, fireboll, 35, -1, -1, 0, 0, 90));
                }
                boostMode = true;
                maxSpeedX = 10;
            }

            speedX = vector == left ? -1 : vector == right ? 1 : speedX;
            boostX = vector == left ? -0.25 : vector == right ? 0.25 : boostX;
        }

        void unsetVector(int vector) {
            if(vector == vectorX) {
                vectorX = 0;
                boostX = -(boostX * 2);
            }
            if(vector == 25) {
                boostY = 5;
                vectorY = 0;
            };
            
            if(vector == 23) {
                boostMode = false;
                maxSpeedX = 5;
            }
        }
};