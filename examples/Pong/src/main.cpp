#include "Platform.h"

struct Rectangle {
    glm::vec2 position;
    glm::vec2 size;
};

static constexpr int WINDOW_WIDTH  = 640;
static constexpr int WINDOW_HEIGHT = 360;

static Rectangle playerPaddle = {
    .position = { 32, WINDOW_HEIGHT / 2 },
    .size     = { 32, 128 },
};

static f32 playerMoveSpeed = 150.0f;

static Rectangle aiPaddle = {
    .position = { WINDOW_WIDTH - 32, WINDOW_HEIGHT / 2 },
    .size     = { 32, 128 },
};

static Rectangle ball = {
    .position = { WINDOW_WIDTH / 2, WINDOW_HEIGHT / 2 },
    .size     = { 16, 16 }
};

static f32 ballMoveSpeed         = 250.0f;
static glm::vec2 ballMovementDir = { 1.0f, 0.0f };

static void Update(const Input& input, f32 deltaTime) {
    // player movement
    
    if (playerPaddle.position.y - (playerPaddle.size.y / 2.0f) >= 0 && input.KeyDown('w')) {
        playerPaddle.position.y -= playerMoveSpeed * deltaTime;
    }

    if (playerPaddle.position.y + (playerPaddle.size.y / 2.0f) <= WINDOW_HEIGHT && input.KeyDown('s')) {
        playerPaddle.position.y += playerMoveSpeed * deltaTime;
    }

    // ball movement
    if (ball.position.x < playerPaddle.position.x + (playerPaddle.size.x / 2.0f) || ball.position.x > aiPaddle.position.x - (aiPaddle.size.x / 2.0f)) {
        ballMovementDir.x *= -1;
    }

    if (ball.position.x - (ball.size.x / 2.0f) <= 0 || ball.position.x + (ball.size.x / 2.0f) >= WINDOW_WIDTH) {
        ballMovementDir.x *= -1.0f;
    }

    if (ball.position.y - (ball.size.y / 2.0f) <= 0 || ball.position.y + (ball.size.y / 2.0f) >= WINDOW_HEIGHT) {
        ballMovementDir.y *= -1.0f;
    }

    ball.position += ballMovementDir * ballMoveSpeed * deltaTime;

    // draw everything
    Renderer2D::DrawRect(playerPaddle.position, playerPaddle.size, { 1, 1, 1, 1 }); 
    Renderer2D::DrawRect(aiPaddle.position, aiPaddle.size, { 1, 1, 1, 1 });
    Renderer2D::DrawRect(ball.position, ball.size, { 1, 1, 1, 1 });
}

int main(int argc, char** argv) {
    Settings settings = {
        .windowWidth  = WINDOW_WIDTH,
        .windowHeight = WINDOW_HEIGHT,
        .windowTitle  = "PONG",
        .updateFunc   = Update,
    };

    Platform::Init(settings);
    Platform::Run();
    Platform::Shutdown();

    return 0;
}
