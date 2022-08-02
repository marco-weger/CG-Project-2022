#include <stdio.h>
#include <algorithm>
#include <iostream>
#include <cstdlib>
#include <string>
#include <fstream>

#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEFAULT_ALIGNED_GENTYPES
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

enum status : int {stopped = 0, starting = 1, started = 2};

class Game
{
    private:
    
    status gameStatus = status::stopped;
    float leftFlipperDegree = 0;
    float rightFlipperDegree = 0;
    
    glm::vec3 ballPosition;
    glm::vec3 ballDirection;
    float ballVelocity;
    
    float waitingTime = 0;
    
    float zTopLimit = 9.9;
    float zBottomLimit = 0.65;
    float zBottomRightLimit = -1.25;
    float zBottomLeftLimit = 1.25;
    
    float xRightLimit = -2.28;
    float xLeftLimit = 2.48;
        
    float zLoseLine = -0.11;
    
    float zBumper = 5.993426f;
    float xBumper1 = 1.4819f;
    float xBumper2 = - 1.204f;
    float xBumper3 = 0.179f;
    
    float bumperR = 0.395;
    float ballR = 0.16289;
    
    float zFlipper = 0.5;
    
    float flipperDimension = zBottomLeftLimit - 0.13;
    
    std::string collisionType = "";
    bool incrementScore;
    
    bool leftFlipperMoving = false;
    bool rightFlipperMoving = false;
    
    bool ballInGame;
    
    int record = -1;
    int score = 0;
    
    public:
    
    constexpr static float maxWaitingTime = 3.0f;
    constexpr static float openFlipperTime = 0.2f;
    
    void setStatus(status gameStatus)
    {
        this->gameStatus = gameStatus;
    }
    
    void setWaitingTime(float waitingTime)
    {
        this->waitingTime = waitingTime;
    }
    
    void setLeftFlipperDegree(float degree)
    {
        degree = std::max(0.0f,degree);
        degree = std::min(90.0f,degree);
        leftFlipperMoving = leftFlipperDegree != degree;
        leftFlipperDegree = degree;
    }
    
    void setRightFlipperDegree(float degree)
    {
        degree = std::max(0.0f,degree);
        degree = std::min(90.0f,degree);
        rightFlipperMoving = rightFlipperDegree != degree;
        rightFlipperDegree = degree;
    }
    
    void setBallDirection(glm::vec3 direction)
    {
        this->ballDirection = glm::normalize(direction);
    }
    
    void setBallPosition(glm::vec3 ballPosition)
    {
        //ballPosition[2] = 1.45;
        //ballPosition[0] = -1.05;
        
        // vertical slope
        ballPosition[1] = ballPosition[2] * sin(glm::radians(6.5f)) / cos(glm::radians(6.5f));
        
        this->ballPosition = ballPosition;
    }
    
    void setBallVelocity(float ballVelocity)
    {
        this->ballVelocity = ballVelocity;
    }
    
    status getStatus()
    {
        return gameStatus;
    }
    
    float getWaitingTime()
    {
        return waitingTime;
    }
    
    float getLeftFlipperDegree()
    {
        return leftFlipperDegree;
    }
    
    float getRightFlipperDegree()
    {
        return rightFlipperDegree;
    }
    
    glm::vec3 getBallDirection()
    {
        return ballDirection;
    }
    
    glm::vec3 getBallPosition()
    {
        return ballPosition;
    }
    
    float getBallVelocity()
    {
        return ballVelocity;
    }
    
    std::string getCollisionType()
    {
        return collisionType;
    }
    
    int getRecord()
    {
        return record;
    }
    
    int getScore()
    {
        return score;
    }
    
    void initializeBall()
    {
        float x = static_cast <float> (rand()) / static_cast <float> (RAND_MAX / 0.50f);
        if (rand() % 2 == 0)
        {
            x *= -1;
        }
                
        glm::vec3 direction = glm::vec3(x,0,1);
        
        setBallPosition(glm::vec3(0,0,zLoseLine));
        setBallDirection(glm::normalize(direction));
        setBallVelocity(0.20f * (std::max(1.0f, waitingTime-1)));
        setBallVelocity(ballVelocity * 0.3);
        
        leftFlipperDegree = 90.0;
        rightFlipperDegree = 90.0;
        
        ballInGame = false;
        
        if(record < 0)
        {
            std::ifstream inFile;
            inFile.open("record.cg");
            if(inFile)
            {
                while (inFile >> x)
                {
                    record = x;
                }
            }
        }
        
        score = 0;
        incrementScore = true;
    }
    
    bool isBallInGame()
    {
        if(!ballInGame)
        {
            if(zBottomLimit + flipperDimension + 4 * ballR < ballPosition[2])
            {
                ballInGame = true;
            }
        }
        
        return ballInGame;
    }
    
    void detectCollision()
    {
        float bumper1Distance = std::sqrt(std::pow(ballPosition[0] - xBumper1, 2) + std::pow(ballPosition[2] - zBumper, 2));
        float bumper2Distance = std::sqrt(std::pow(ballPosition[0] - xBumper2, 2) + std::pow(ballPosition[2] - zBumper, 2));
        float bumper3Distance = std::sqrt(std::pow(ballPosition[0] - xBumper3, 2) + std::pow(ballPosition[2] - zBumper, 2));
        
        float flipperH = 2 * ballR;
        float flipperSlope = 0.05;
        
        glm::vec2 p1 = glm::vec2(zBottomLeftLimit, zFlipper);
        glm::vec2 p2 = glm::vec2(zBottomLeftLimit - flipperDimension, zFlipper - flipperSlope);
        p2[0] += flipperDimension * sin(glm::radians(leftFlipperDegree));
        p2[0] -= flipperSlope * sin(glm::radians(leftFlipperDegree));
        p2[1] += flipperDimension * sin(glm::radians(leftFlipperDegree));
        p2[1] -= flipperH * sin(glm::radians(leftFlipperDegree));
        p2[1] += flipperSlope * sin(glm::radians(leftFlipperDegree));
        
        glm::vec2 bp1 = glm::vec2(zBottomLeftLimit, zFlipper - flipperH);
        glm::vec2 bp2 = glm::vec2(zBottomLeftLimit - flipperDimension, zFlipper - flipperH + flipperSlope);
        bp1[0] -= flipperH * sin(glm::radians(leftFlipperDegree));
        bp2[0] += flipperDimension * sin(glm::radians(leftFlipperDegree));
        bp2[0] -= (flipperH - flipperSlope) * sin(glm::radians(leftFlipperDegree));
        bp2[1] += flipperDimension * sin(glm::radians(leftFlipperDegree));
        bp2[1] -= flipperSlope * sin(glm::radians(leftFlipperDegree));
        
        //std::cout << "flipperH: " << flipperH << " -> " << (p2[1] - bp2[1] + flipperSlope + flipperSlope) << "\n";
        //std::cout << "bp1: " << bp1[0] << " " << bp1[1] << "\n";
        //std::cout << "bp2: " << bp2[0] << " " << bp2[1] << "\n";
        //std::cout << "DIST: " << getDistance(bp2, p2) << "\n";
        
        glm::vec2 p3 = glm::vec2(zBottomRightLimit, zFlipper);
        glm::vec2 p4 = glm::vec2(zBottomRightLimit + flipperDimension, zFlipper - flipperSlope);
        p4[0] -= flipperDimension * sin(glm::radians(rightFlipperDegree));
        p4[0] += flipperSlope * sin(glm::radians(rightFlipperDegree));
        p4[1] += flipperDimension * sin(glm::radians(rightFlipperDegree));
        p4[1] -= flipperH * sin(glm::radians(rightFlipperDegree));
        p4[1] += flipperSlope * sin(glm::radians(rightFlipperDegree));
        
        glm::vec2 bp3 = glm::vec2(zBottomRightLimit, zFlipper - flipperH);
        glm::vec2 bp4 = glm::vec2(zBottomRightLimit + flipperDimension, zFlipper - flipperH + flipperSlope);
        bp3[0] += flipperH * sin(glm::radians(rightFlipperDegree));
        bp4[0] -= flipperDimension * sin(glm::radians(rightFlipperDegree));
        bp4[0] += (flipperH - flipperSlope) * sin(glm::radians(rightFlipperDegree));
        bp4[1] += flipperDimension * sin(glm::radians(rightFlipperDegree));
        bp4[1] -= flipperSlope * sin(glm::radians(rightFlipperDegree));
        
        /*
        std::cout << "CHECK1: " << (bp3[0] == -bp1[0]) << "\n";
        std::cout << "CHECK2: " << (bp3[1] == bp1[1]) << "\n";
        std::cout << "CHECK3: " << (bp4[0] == -bp2[0]) << "\n";
        std::cout << "CHECK4: " << (bp4[1] == bp2[1]) << "\n";
        std::cout << "CHECK5: " << (p3[0] == -p1[0]) << "\n";
        std::cout << "CHECK6: " << (p3[1] == p1[1]) << "\n";
        std::cout << "CHECK7: " << (p4[0] == -p2[0]) << "\n";
        std::cout << "CHECK8: " << (p4[1] == p2[1]) << "\n";
        std::cout << "DIST: " << getDistance(bp4, p4) << "\n";
         */
        
        glm::vec2 ballPosition2D = glm::vec2(ballPosition[0], ballPosition[2]);
        
        float cLimit = 1.0f;
        
        bool topCondition = zTopLimit <= ballPosition[2];
        bool leftCondition = xLeftLimit <= ballPosition[0];
        bool rightCondition = xRightLimit >= ballPosition[0];
        bool bottomCondition = zBottomLimit >= ballPosition[2];
        if(bottomCondition)
        {
            if(zBottomLeftLimit >= ballPosition[0] && zBottomRightLimit <= ballPosition[0])
            {
                bottomCondition = false;
            }
        }
        bool bumper1Condition = bumperR + ballR >= bumper1Distance;
        bool bumper2Condition = bumperR + ballR >= bumper2Distance;
        bool bumper3Condition = bumperR + ballR >= bumper3Distance;
        bool leftFlipperCondition = circleLine(ballPosition2D, p1, p2);
        bool rightFlipperCondition = circleLine(ballPosition2D, p4, p3);
        bool bLeftFlipperCondition = circleLine(ballPosition2D, bp1, bp2);
        bool bRightFlipperCondition = circleLine(ballPosition2D, bp4, bp3);
        bool vLeftFlipperCondition = circleLine(ballPosition2D, bp2, p2);
        bool vRightFlipperCondition = circleLine(ballPosition2D, bp4, p4);
        
        if(topCondition || leftCondition || rightCondition || bottomCondition || bumper1Condition || bumper2Condition || bumper3Condition || leftFlipperCondition || rightFlipperCondition || bLeftFlipperCondition || bRightFlipperCondition || vLeftFlipperCondition || vRightFlipperCondition)
        {
            if(topCondition && collisionType != "T")
            {
                glm::vec3 n = glm::vec3(0,0,-1);
                setBallDirection(ballDirection - (1.0f + cLimit) * (ballDirection * n) * n);
                collisionType = "T";
            }
            else if(leftCondition && collisionType != "L")
            {
                glm::vec3 n = glm::vec3(-1,0,0);
                setBallDirection(ballDirection - (1.0f + cLimit) * (ballDirection * n) * n);
                collisionType = "L";
            }
            else if(rightCondition && collisionType != "R")
            {
                glm::vec3 n = glm::vec3(1,0,0);
                setBallDirection(ballDirection - (1.0f + cLimit) * (ballDirection * n) * n);
                collisionType = "R";
            }
            else if(bottomCondition && collisionType != "B")
            {
                glm::vec3 n = glm::vec3(0,0,1);
                setBallDirection(ballDirection - (1.0f + cLimit) * (ballDirection * n) * n);
                collisionType = "B";
            }
            else if(bumper1Condition && collisionType != "BU1")
            {
                glm::vec3 n = glm::vec3(ballPosition[0] - xBumper1, 0, ballPosition[2] - zBumper);
                n = glm::normalize(n);
                setBallDirection(n);
                collisionType = "BU1";
            }
            else if(bumper2Condition && collisionType != "BU2")
            {
                glm::vec3 n = glm::vec3(ballPosition[0] - xBumper2, 0, ballPosition[2] - zBumper);
                n = glm::normalize(n);
                setBallDirection(n);
                collisionType = "BU2";
            }
            else if(bumper3Condition && collisionType != "BU3")
            {
                glm::vec3 n = glm::vec3(ballPosition[0] - xBumper3, 0, ballPosition[2] - zBumper);
                n = glm::normalize(n);
                setBallDirection(n);
                collisionType = "BU3";
            }
            else if(leftFlipperCondition && collisionType.find("_L") == std::string::npos)
            {
                glm::vec2 v2 = p1 - p2;
                glm::vec3 n = glm::vec3(-v2[1], 0, v2[0]);
                if(leftFlipperMoving)
                {
                    setBallDirection(n);
                }
                else
                {
                    setBallDirection(ballDirection - (1.0f + cLimit) * (ballDirection * n) * n);
                }
                std::cout << "COLLISION F_L (" << leftFlipperDegree << ")" << "\n";
                collisionType = "F_L";
            }
            else if(rightFlipperCondition && collisionType.find("_R") == std::string::npos)
            {
                glm::vec2 v2 = p4 - p3;
                glm::vec3 n = glm::vec3(-v2[1], 0, v2[0]);
                if(rightFlipperMoving)
                {
                    setBallDirection(n);
                }
                else
                {
                    setBallDirection(ballDirection - (1.0f + cLimit) * (ballDirection * n) * n);
                }
                std::cout << "COLLISION R_L (" << rightFlipperDegree << ")" << "\n";
                collisionType = "F_R";
            }
            else if(bLeftFlipperCondition && collisionType.find("_L") == std::string::npos)
            {
                glm::vec2 v2 = bp1 - bp2;
                glm::vec3 n = glm::vec3(v2[1], 0, -v2[0]);
                if(leftFlipperMoving)
                {
                    setBallDirection(n);
                }
                else
                {
                    setBallDirection(ballDirection - (1.0f + cLimit) * (ballDirection * n) * n);
                }
                //std::cout << "P2 " << p2[0] << " - " << p2[1] << "\n";
                //std::cout << "BALL " << ballPosition[0] << " - " << ballPosition[2] << "\n";
                std::cout << "COLLISION BF_L (" << leftFlipperDegree << ")" << "\n";
                collisionType = "BF_L";
            }
            else if(bRightFlipperCondition && collisionType.find("_R") == std::string::npos)
            {
                glm::vec2 v2 = bp4 - bp3;
                glm::vec3 n = glm::vec3(v2[1], 0, -v2[0]);
                if(rightFlipperMoving)
                {
                    setBallDirection(n);
                }
                else
                {
                    setBallDirection(ballDirection - (1.0f + cLimit) * (ballDirection * n) * n);
                }
                //std::cout << "P2 " << p2[0] << " - " << p2[1] << "\n";
                //std::cout << "BALL " << ballPosition[0] << " - " << ballPosition[2] << "\n";
                std::cout << "COLLISION BF_R (" << rightFlipperDegree << ")" << "\n";
                collisionType = "BF_R";
            }
            else if(vLeftFlipperCondition && collisionType.find("_L") == std::string::npos)
            {
                glm::vec2 v2 = bp2 - p2;
                glm::vec3 n = glm::vec3(v2[1], 0, -v2[0]);
                if(leftFlipperMoving)
                {
                    setBallDirection(n);
                }
                else
                {
                    setBallDirection(ballDirection - (1.0f + cLimit) * (ballDirection * n) * n);
                }
                std::cout << "COLLISION VF_L (" << leftFlipperDegree << ")" << "\n";
                collisionType = "VF_L";
            }
            else if(vRightFlipperCondition && collisionType.find("_R") == std::string::npos)
            {
                glm::vec2 v2 = bp4 - p4;
                glm::vec3 n = glm::vec3(-v2[1], 0, +v2[0]);
                if(rightFlipperMoving)
                {
                    setBallDirection(n);
                }
                else
                {
                    setBallDirection(ballDirection - (1.0f + cLimit) * (ballDirection * n) * n);
                }
                //std::cout << "bp4 " << bp4[0] << " - " << bp4[1] << "\n";
                //std::cout << "p4 " << p4[0] << " - " << p4[1] << "\n";
                std::cout << "COLLISION VF_R (" << rightFlipperDegree << ")" << "\n";
                collisionType = "VF_R";
            }
            
            if(incrementScore && ((collisionType.find("_L") != std::string::npos && leftFlipperDegree != 0) || (collisionType.find("_R") != std::string::npos && rightFlipperDegree != 0)))
            {
                score++;
                if(score > record)
                {
                    record = score;
                    std::ofstream fw("record.cg", std::ofstream::out);
                    if (fw.is_open())
                    {
                        fw << score;
                    }
                    fw.close();
                    incrementScore = false;
                }
            }
        }
        else
        {
            collisionType = "";
            incrementScore = true;
        }
    }
    
    bool circleLine(glm::vec2 c, glm::vec2 p1, glm::vec2 p2)
    {
        float lineLength = getDistance(p1, p2);
        float dot = (((c[0] - p1[0]) * (p2[0] - p1[0])) + ((c[1] - p1[1]) * (p2[1] - p1[1]))) / (std::pow(lineLength, 2));
        float xProj = p1[0] + (dot * (p2[0] - p1[0]));
        float yProj = p1[1] + (dot * (p2[1] - p1[1]));
        
        float xMax = std::max(p1[0], p2[0]);
        float xMin = std::min(p1[0], p2[0]);
        float yMax = std::max(p1[1], p2[1]);
        float yMin = std::min(p1[1], p2[1]);
        
        if(xProj <= xMax && xProj >= xMin && yProj <= yMax && yProj >= yMin)
        {
            float dist = getDistance(c, glm::vec2(xProj, yProj));
            
            if(dist <= ballR)
            {
                return true;
            }
        }
        return false;
    }
    
    float getDistance(glm::vec2 p1, glm::vec2 p2)
    {
        return std::sqrt(std::pow(p2[0]-p1[0],2) + std::pow(p2[1]-p1[1],2));
    }
    
    void detectEndGame()
    {
        if(ballInGame && ballPosition[2] <= zLoseLine + ballR)
        {
            gameStatus = status::stopped;
            initializeBall();
        }
    }
};
