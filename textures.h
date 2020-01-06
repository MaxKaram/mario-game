#include <SFML/Graphics.hpp>
#include <vector>
#include <iostream>


enum {ground, wall, lowPipe, mediumPipe, highPipe, BLOCK, oblako, bigKyst, kyst, superBigKyst, doubleKyst,
      tripleKyst, doubleOblako, tripleOblako, questionSymbol, disabled};

enum {mushroom, flower, blockW, wallChunk, fireboll, explosion, blockDmushroom, blockDmoney, enemy1, rotatedEnemy1, bittedEnemy1, gameOverAnimation, heroType};

enum {heroLevel1, heroLevel2, heroLevel3};

struct heroTexture {
    int x, y, width, height, gapX, gapY;
    int upX, upWidth, upHeight;
    int downX, downY, downWidth, downHeight;
};

heroTexture heroTexturesArr[3] = {
    heroTexture{20, 83, 16, 17, 16, 20, 101, 17, 17},
    heroTexture{3, 1, 16, 32, 19, 36, 113, 16, 31},
    heroTexture{3, 397, 16, 32, 19, 36, 113, 16, 31}
};

sf::IntRect blockSizes[16] = {
    sf::IntRect(0, 0, 32, 32), sf::IntRect(34, 0, 32, 32), sf::IntRect(69, 0, 64, 64),
    sf::IntRect(133, 0, 64, 96), sf::IntRect(197, 0, 64, 128), sf::IntRect(263, 0, 32, 32),
    sf::IntRect(297, 0, 64, 49), sf::IntRect(362, 0, 96, 38), sf::IntRect(459, 0, 64, 32),
    sf::IntRect(0, 141, 160, 70), sf::IntRect(160, 141, 96, 32), sf::IntRect(160, 173, 128, 32),
    sf::IntRect(289, 141, 96, 48), sf::IntRect(289, 189, 128, 48), sf::IntRect(435, 141, 32, 32),
    sf::IntRect(435, 237, 32, 32)
};

std::vector<std::vector<sf::IntRect>> textureArrs = {
    {sf::IntRect(488, 141, 32 , 32)},
    {sf::IntRect(0, 226, 32, 32), sf::IntRect(32, 226, 32, 32), sf::IntRect(64, 226, 32, 32), sf::IntRect(96, 226, 32, 32)},
    {sf::IntRect(34, 0, 32, 32)},
    {sf::IntRect(34, 0, 14, 14)},
    {sf::IntRect(166, 226, 16, 16)},
    {sf::IntRect(199, 218, 32, 32), sf::IntRect(240, 218, 32, 32), sf::IntRect(240, 218, 32, 32), sf::IntRect(199, 218, 32, 32)},
    {sf::IntRect(435, 237, 32, 32)},
    {sf::IntRect(435, 237, 32, 32)},
    {sf::IntRect(103, 137, 16 , 16), sf::IntRect(122, 137, 16 , 16)},
    {sf::IntRect(148, 137, 16 , 16)},
    {sf::IntRect(84, 145, 16 , 8)},
    {sf::IntRect(233, 85, 14 , 14)},

};