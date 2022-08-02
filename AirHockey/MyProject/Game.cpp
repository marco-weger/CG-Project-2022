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

//#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

//enum status : int {stopped = 0, starting = 1, started = 2};
enum CamType {player1, player2, upPlayer1, upPlayer2, center, upCenter};

class Game
{
    private:
    
    CamType camType = CamType::player1;
    
    glm::vec3 aim = glm::vec3(0.00f, 0.0f, 0.0f);
    glm::vec3 pos = glm::vec3(0.000f, 2.00f, -0.001f);
    
    const float CAM_SPEED = 1.25f;
    
    bool borderLoaded = false;
    int tableWidth, tableHeight;
    stbi_uc* tableMap;
    
    const float tableHalfHeight = 0.88;
    const float tableHalfWidth = 0.45;
    
    const float paddleRadius = 0.0705;
    const float diskRadius = 0.029f * 1.75f;
    const int checkSteps = 12;
    
    glm::vec2 player1Position;
    glm::vec2 player2Position;
    
    glm::vec2 diskPosition;
    glm::vec2 diskDirection;
    const float diskSpeed = 0.8f;
    const float paddleSpedd = 0.50f;
    
    const float mouseMultiplier = 3.5f;
    
    bool collisionDetected;
    
    float accuracy = 0.001;
    float halfGoalLine = 0.22;
    
    const float goalYLevel = 0.918f;
    int player1Score;
    int player2Score;
    const int winScore = 5;
    
    bool cpuDirection;
    const float cpuSpeed = 0.35f;
    
    void loadBorder()
    {
        if(!borderLoaded)
        {
            tableMap = stbi_load("textures/border.png", &tableWidth, &tableHeight, NULL, 1);
            
            if (!tableMap) {
                std::cout << "textures/border.png" << "\n";
                throw std::runtime_error("failed to load map image!");
            }
            std::cout << "Table map -> size: " << tableWidth << "x" << tableHeight <<"\n";
            
            borderLoaded = true;
        }
    }
    
    bool getStepPointPaddle(float x, float y) {
        x += tableHalfWidth;
        if(x < 0.0f)
        {
            return false;
        }
        else if(x > 2 * tableHalfWidth)
        {
            return false;
        }
        
        y += tableHalfHeight;
        if(y < 0.0f)
        {
            return false;
        }
        else if(y > 2 * tableHalfHeight)
        {
            return false;
        }
        
        int pixX = round(x * tableWidth / (2 * tableHalfWidth));
        int pixY = round(y * tableHeight / (2 * tableHalfHeight));
        
        int pix = (int)tableMap[tableWidth * pixY + pixX];
        if(pix > 100)
        {
            return false;
        }
        
        return true;
    }
    
    glm::vec2 getStepPointDisk(float x, float y) {
        y += tableHalfHeight;
        if(y < 0.0f && (x > halfGoalLine || x < -halfGoalLine))
        {
            //std::cout << "[COLLISION] " << x << " " << y << "\n";
            return glm::vec2(0,1);
        }
        else if(y > 2 * tableHalfHeight && (x > halfGoalLine || x < -halfGoalLine))
        {
            return glm::vec2(0,-1);
        }
        
        x += tableHalfWidth;
        if(x < 0.0f)
        {
            return glm::vec2(1,0);
        }
        else if(x > 2 * tableHalfWidth)
        {
            return glm::vec2(-1,0);
        }
        
        int pixX = round(x * tableWidth / (2 * tableHalfWidth));
        int pixY = round(y * tableHeight / (2 * tableHalfHeight));
        
        int pix = (int)tableMap[tableWidth * pixY + pixX];
        //std::cout << pixX << " " << pixY << " " << x << " " << y << " \t P = " << pix << "\n";
        if(pix < 220)
        {
            return glm::vec2(0,0);
        }
        else
        {
            glm::vec impact = glm::vec2(x-tableHalfWidth,y-tableHalfHeight);
            glm::vec2 center = glm::vec2(0.0f);
            
            center[0] = tableHalfWidth-halfGoalLine;
            center[0] *= x > tableHalfWidth ? 1 : -1;
            center[1] = tableHalfHeight-halfGoalLine;
            center[1] *= y > tableHalfHeight ? 1 : -1;
            
            return center - impact;
        }
    }
    
    bool getPaddleStep(float x, float y) {
        for(int i = 0; i < checkSteps; i++) {
            if(!getStepPointPaddle(x + cos(6.2832 * i / (float)checkSteps) * paddleRadius,
                                  y + sin(6.2832 * i / (float)checkSteps) * paddleRadius)) {
                return false;
            }
        }
        return true;
    }
    
    glm::vec2 getDiskStep(float x, float y) {
        //std::cout << "************************************\n";
        glm::vec2 tmp;
        for(int i = 0; i < checkSteps; i++) {
            tmp = getStepPointDisk(x + cos(6.2832 * i / (float)checkSteps) * diskRadius,
                               y + sin(6.2832 * i / (float)checkSteps) * diskRadius);
            if(tmp[0] != 0 || tmp[1] != 0) {
                return tmp;
            }
        }
        return glm::vec2(0,0);
    }
    
    bool sameSign(float x, float y)
    {
        return (x >= 0) ^ (y < 0);
    }
    
    float getDistance(glm::vec2 p1, glm::vec2 p2)
    {
        return std::sqrt(std::pow(p2[0]-p1[0],2) + std::pow(p2[1]-p1[1],2));
    }
    
    public:
    
    Game()
    {
        loadBorder();
        
        player1Position = glm::vec2(0.0,-0.68);
        player2Position = glm::vec2(0.0,0.68);
        
        diskDirection = glm::vec2(0.0f,0.0f);
        diskPosition = glm::vec2(0.0f,0.0f);
        
        player1Score = 0;
        player2Score = 0;
        
        cpuDirection = true;
    }
    
    CamType getCamType()
    {
        return camType;
    }
    
    void changeView()
    {
        switch (camType) {
            case CamType::player1:
                camType = CamType::upPlayer1;
                break;
            case CamType::upPlayer1:
                camType = CamType::player2;
                break;
            case CamType::player2:
                camType = CamType::upPlayer2;
                break;
            case CamType::upPlayer2:
                camType = CamType::center;
                break;
            case CamType::center:
                camType = CamType::upCenter;
                break;
            case CamType::upCenter:
                camType = CamType::player1;
                break;
        }
    }
    
    void incrementPlayer1Position(int axis, float deltaT)
    {
        glm::vec2 newPosition = player1Position;
        newPosition[axis] += deltaT * paddleSpedd;
        if(getPaddleStep(newPosition[0], newPosition[1]) && sameSign(player1Position[1], newPosition[1] + paddleRadius))
        {
            if(getDistance(newPosition, diskPosition) > diskRadius + paddleRadius - accuracy)
            {
                player1Position = newPosition;
            }
        }
    }
    
    void decrementPlayer1Position(int axis, float deltaT)
    {
        glm::vec2 newPosition = player1Position;
        newPosition[axis] -= deltaT * paddleSpedd;
        if(getPaddleStep(newPosition[0], newPosition[1]))
        {
            if(getDistance(newPosition, diskPosition) > diskRadius + paddleRadius - accuracy)
            {
                player1Position = newPosition;
            }
        }
    }
    
    void updatePlayer1Position(glm::vec2 mouse, float deltaT)
    {
        mouse[1] *= -1;
        
        mouse[0] = tableHalfWidth * mouse[0] / 0.30832;
        mouse[1] = tableHalfHeight * mouse[1] / 0.808919;
        
        glm::vec2 newPosition = player1Position;
        
        //std::cout << mouse[0] << " " << mouse[1] << "\n";
        
        if(abs(player1Position[0] - mouse[0]) > 0.001)
        {
            newPosition[0] += std::min(abs(player1Position[0] - mouse[0]), mouseMultiplier * deltaT * paddleSpedd) * (player1Position[0] > mouse[0] ? -1 : 1);
        }
        
        if(abs(player1Position[1] - mouse[1]) > 0.001)
        {
            newPosition[1] += std::min(abs(player1Position[1] - mouse[1]), mouseMultiplier * deltaT * paddleSpedd) * (player1Position[1] > mouse[1] ? -1 : 1);
        }
        
        if(getPaddleStep(newPosition[0], newPosition[1]) && newPosition[1] + paddleRadius < 0)
        {
            if(getDistance(newPosition, diskPosition) > diskRadius + paddleRadius - accuracy)
            {
                player1Position = newPosition;
            }
        }
    }
    
    void updatePlayer2Position(glm::vec2 mouse, float deltaT)
    {
        mouse[0] *= -1;
        
        mouse[0] = tableHalfWidth * mouse[0] / 0.30832;
        mouse[1] = tableHalfHeight * mouse[1] / 0.808919;
        
        glm::vec2 newPosition = player2Position;
        
        if(abs(player2Position[0] - mouse[0]) > 0.001)
        {
            newPosition[0] += std::min(abs(player2Position[0] - mouse[0]), mouseMultiplier * deltaT * paddleSpedd) * (player2Position[0] > mouse[0] ? -1 : 1);
        }
        
        if(abs(player2Position[1] - mouse[1]) > 0.001)
        {
            newPosition[1] += std::min(abs(player2Position[1] - mouse[1]), mouseMultiplier * deltaT * paddleSpedd) * (player2Position[1] > mouse[1] ? -1 : 1);
        }
        
        if(getPaddleStep(newPosition[0], newPosition[1]) && newPosition[1] - paddleRadius > 0)
        {
            if(getDistance(newPosition, diskPosition) > diskRadius + paddleRadius - accuracy)
            {
                player2Position = newPosition;
            }
        }
    }
    
    void incrementPlayer2Position(int axis, float deltaT)
    {
        glm::vec2 newPosition = player2Position;
        newPosition[axis] += deltaT * paddleSpedd;
        if(getPaddleStep(newPosition[0], newPosition[1]))
        {
            if(getDistance(newPosition, diskPosition) > diskRadius + paddleRadius - accuracy)
            {
                player2Position = newPosition;
            }
        }
    }
    
    void decrementPlayer2Position(int axis, float deltaT)
    {
        glm::vec2 newPosition = player2Position;
        newPosition[axis] -= deltaT * paddleSpedd;
        if(getPaddleStep(newPosition[0], newPosition[1]) && sameSign(player2Position[1], newPosition[1] - paddleRadius))
        {
            if(getDistance(newPosition, diskPosition) > diskRadius + paddleRadius - accuracy)
            {
                player2Position = newPosition;
            }
        }
    }
    
    void updatePlayer1Cpu(float deltaT)
    {
        if(diskDirection[0] != 0 || diskDirection[1] != 0)
        {
            player1Position[1] = -0.68;
            player1Position[0] += deltaT * cpuSpeed * (cpuDirection ? 1 : -1);
            if(abs(player1Position[0]) > halfGoalLine)
            {
                cpuDirection = !cpuDirection;
                player1Position[0] = halfGoalLine * (player1Position[0] > 0 ? 1 : -1);
            }
        }
    }
    
    void updatePlayer2Cpu(float deltaT)
    {
        if(diskDirection[0] != 0 || diskDirection[1] != 0)
        {
            player2Position[1] = 0.68;
            player2Position[0] += deltaT * cpuSpeed * (cpuDirection ? 1 : -1);
            if(abs(player2Position[0]) > halfGoalLine)
            {
                cpuDirection = !cpuDirection;
                player2Position[0] = halfGoalLine * (player2Position[0] > 0 ? 1 : -1);
            }
        }
    }

    void incrementDiskPosition(int axis, float deltaT)
    {
        diskDirection = glm::vec2(0);
        diskDirection[axis] = +1;
        
        glm::vec2 newPosition = diskPosition;
        newPosition[axis] += deltaT * diskSpeed;
        glm::vec2 check = getDiskStep(newPosition[0], newPosition[1]);
        if(check[0] == 0 && check[1] == 0)
        {
            diskPosition = newPosition;
        }
    }
    
    void decrementDiskPosition(int axis, float deltaT)
    {
        diskDirection = glm::vec2(0);
        diskDirection[axis] = -1;
        
        glm::vec2 newPosition = diskPosition;
        newPosition[axis] -= deltaT * diskSpeed;
        glm::vec2 check = getDiskStep(newPosition[0], newPosition[1]);
        if(check[0] == 0 && check[1] == 0)
        {
            diskPosition = newPosition;
        }
    }

    void updateDiskPosition(float deltaT)
    {
        glm::vec2 newPosition = diskPosition;
        newPosition += diskDirection * deltaT * diskSpeed;
        
        glm::vec2 n = getDiskStep(newPosition[0], newPosition[1]);
        
        if(n[0] == 0 && n[1] == 0)
        {
            diskPosition = newPosition;
        }
        else
        {
            //std::cout << "COLLISION!\n";
            //std::cout << "[n] " << n[0] << " " << n[1] << "\n";
            
            if(n[0] == 0 || n[1] == 0)
            {
                diskDirection = diskDirection - 2.0f * (diskDirection * n) * n;
                diskDirection = glm::normalize(diskDirection);
            }
            else
            {
                diskDirection = glm::normalize(n);
            }
            
            if(!collisionDetected)
            {
                diskPosition = diskPosition + diskDirection * deltaT * diskSpeed;
            }
        }
    }
    
    glm::vec2 getPlayer1Position()
    {
        return player1Position;
    }
    
    glm::vec2 getPlayer2Position()
    {
        return player2Position;
    }
    
    glm::vec2 getDiskDirection()
    {
        return diskDirection;
    }
    
    glm::vec2 getDiskPosition()
    {
        return diskPosition;
    }

    glm::vec3 getAimPos()
    {
        return aim;
    }
    
    glm::vec3 getCamPos(float deltaT)
    {
        glm::vec3 target = glm::vec3(-1.8f, 0.75f, 0.0f);
        switch (camType) {
            case CamType::player1:
                target = glm::vec3(-1.8f, 0.75f, 0.0f);
                break;
            case CamType::player2:
                target = glm::vec3(1.8f, 0.75f, 0.0f);
                break;
            case CamType::upPlayer1:
                target = glm::vec3(-0.001f, 2.65f, 0.0f);
                break;
            case CamType::upPlayer2:
                target = glm::vec3(0.001f, 2.65f, 0.0f);
                break;
            case CamType::center:
                target = glm::vec3(0.000f, 1.80f, 1.30f);
                break;
            case CamType::upCenter:
                target = glm::vec3(0.000f, 2.00f, 0.001f);
                break;
            default:
                std::cout << "Pos: " << pos[0] << " " << pos[1] << " " << pos[2] << "\n";
                break;
        }
        for(int i = 0; i < 3; i++)
        {
            if(pos[i] != target[i])
            {
                pos[i] += std::min(CAM_SPEED * deltaT, abs(pos[i] - target[i])) * (pos[i] > target[i] ? -1 : 1);
            }
        }
        
        return pos;
    }
    
    int getPlayer1Score()
    {
        return player1Score;
    }
    
    int getPlayer2Score()
    {
        return player2Score;
    }
    
    void detectCollision()
    {
        collisionDetected = true;
        if(getDistance(diskPosition, player1Position) <= paddleRadius + diskRadius + accuracy)
        {
            diskDirection = glm::normalize(diskPosition - player1Position);
        }
        else if(getDistance(diskPosition, player2Position) <= paddleRadius + diskRadius + accuracy)
        {
            diskDirection = glm::normalize(diskPosition - player2Position);
        }
        else
        {
            collisionDetected = false;
        }
    }
    
    void detectGoal()
    {
        if(diskPosition[0] < halfGoalLine && diskPosition[0] > -halfGoalLine && goalYLevel < abs(diskPosition[1]))
        {
 
            if(diskPosition[1] > 0)
            {
                player1Score++;
            }
            else if(diskPosition[1] < 0)
            {
                player2Score++;
            }
            
            diskPosition = glm::vec2(0.0f);
            diskDirection = glm::vec2(0.0f);
            player1Position = glm::vec2(0.0,-0.68);
            player2Position = glm::vec2(0.0,0.68);
            
            //printScore();
        }
            
        if(player1Score == winScore || player2Score == winScore)
        {
            newGame();
        }
    }

    void printScore()
    {
        if(player1Score < winScore && player2Score < winScore)
        {
            std::cout << "\n\n\n\n\n\n\n\n\n";
            std::cout << "*X*X*X*X*X*X*X*X*X*X*X*X*\n";
            std::cout << "*                       *\n";
            std::cout << "*     GOOOOOAL!!!!!     *\n";
            std::cout << "*     PLAYER ONE: " << player1Score << "     *\n";
            std::cout << "*     PLAYER TWO: " << player2Score << "     *\n";
            std::cout << "*                       *\n";
            std::cout << "*X*X*X*X*X*X*X*X*X*X*X*X*\n";
        }
        else if(player1Score == winScore)
        {
            std::cout << "\n\n\n\n\n\n\n\n\n";
            std::cout << "*X*X*X*X*X*X*X*X*X*X*X*X*\n";
            std::cout << "*                       *\n";
            std::cout << "*     THE WINNER IS     *\n";
            std::cout << "*      PLAYER ONE       *\n";
            std::cout << "*                       *\n";
            std::cout << "*X*X*X*X*X*X*X*X*X*X*X*X*\n";
        }
        else if(player2Score == winScore)
        {
            std::cout << "\n\n\n\n\n\n\n\n\n";
            std::cout << "*X*X*X*X*X*X*X*X*X*X*X*X*\n";
            std::cout << "*                       *\n";
            std::cout << "*     THE WINNER IS     *\n";
            std::cout << "*      PLAYER ONE       *\n";
            std::cout << "*                       *\n";
            std::cout << "*X*X*X*X*X*X*X*X*X*X*X*X*\n";
        }
    }
    
    void newGame()
    {
        diskPosition = glm::vec2(0.0f);
        diskDirection = glm::vec2(0.0f);
        
        player1Position = glm::vec2(0.0,-0.68);
        player2Position = glm::vec2(0.0,0.68);
        
        player1Score = 0;
        player2Score = 0;
    }
};
