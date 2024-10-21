#include "pch.h"
#include "BouncingBall.h"

BouncingBall::BouncingBall() {
}

BouncingBall::~BouncingBall() {
}

void BouncingBall::BallAppear(int width, int height) {
    // Generate ball, from speed, duration lifetime, shape
    std::vector<float> sizes = { 20.0f, 40.0f, 60.0f, 80.0f, 100.0f };
    static size_t sizeIndex = 0;

    float radius = sizes[sizeIndex];

    std::vector<float> speedsX = { 70.0f, 140.0f, 200.0f, 250.0f, 100.0f };
    std::vector<float> speedsY = { 175.0f, 90.0f, 150.0f, 50.0f, 125.0f };

    static size_t speedIndex = 0;

    float speedX = speedsX[speedIndex];
    float speedY = speedsY[speedIndex];

    std::vector<int> durations = { 2000, 3000, 5000, 4000, 3500 };

    Ball ball;
    ball.x = width / 2.0f;
    ball.y = height / 2.0f;
    ball.radius = radius;
    ball.speedX = speedX;
    ball.speedY = speedY;
    ball.lifetime = durations[speedIndex];

    balls.push_back(ball);

    speedIndex = (speedIndex + 1) % speedsX.size();
    sizeIndex = (sizeIndex + 1) % sizes.size();

    totalBallsGenerated++;
    currentBallsOnScreen++;
}

using namespace std::chrono;

void BouncingBall::BallMoving(int width, int height) {
    static auto lastTime = steady_clock::now();
    auto currentTime = steady_clock::now();
    duration<float> deltaTime = currentTime - lastTime;
    lastTime = currentTime;

    int deltaMilliseconds = static_cast<int>(deltaTime.count() * 1000.0f);

    for (auto& ball : balls) {
        // Calculate future position
        float futureX = ball.x + ball.speedX * deltaTime.count();
        float futureY = ball.y + ball.speedY * deltaTime.count();

        // Check for collisions
        if (futureX <= ball.radius) {
            ball.speedX = -ball.speedX; // Reverse 
            futureX = ball.radius; 
        }
        else if (futureX >= width - ball.radius) {
            ball.speedX = -ball.speedX; 
            futureX = width - ball.radius; 
        }

        if (futureY <= ball.radius) {
            ball.speedY = -ball.speedY; 
            futureY = ball.radius; 
        }
        else if (futureY >= height - ball.radius) {
            ball.speedY = -ball.speedY; 
            futureY = height - ball.radius; 
        }

        ball.x = futureX;
        ball.y = futureY;

        ball.lifetime -= deltaMilliseconds;

        if (ball.lifetime <= 0) {
            continue; // Skip to next ball
        }
    }

    // Erase expired lifetime balls
    for (size_t i = 0; i < balls.size(); ) {
        std::cout << balls[i].lifetime; // Without c_out, it doesn't work, because of time synchronisation
        if (balls[i].lifetime <= 0) {
            balls.erase(balls.begin() + i);
            currentBallsOnScreen--;
        }
        else {
            ++i;
        }
    }
}
