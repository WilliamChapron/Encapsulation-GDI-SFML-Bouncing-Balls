#pragma once

// Ball data
struct Ball {
    float x, y;
    float radius;
    float speedX, speedY;
    int lifetime;
};

class BouncingBall {
public:
    BouncingBall();
    ~BouncingBall();

    void BallAppear(int width, int height);
    void BallMoving(int width, int height);


    int GetTotalBallsGenerated() const { return totalBallsGenerated; }
    int GetCurrentBallsOnScreen() const { return currentBallsOnScreen; }
    std::vector<Ball> GetAllBalls() const { return balls; }

private:
    std::vector<Ball> balls;
    int totalBallsGenerated = 0; 
    int currentBallsOnScreen = 0; 
};
