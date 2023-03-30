#include <iostream>
#include <vector>

class Vector2D {
public:
    float x, y;

    Vector2D(float x = 0.0f, float y = 0.0f) : x(x), y(y) {}

    Vector2D operator+(const Vector2D& v) const {
        return Vector2D(x + v.x, y + v.y);
    }

    Vector2D operator-(const Vector2D& v) const {
        return Vector2D(x - v.x, y - v.y);
    }

    Vector2D operator*(float s) const {
        return Vector2D(x * s, y * s);
    }
};

class Object {
public:
    Vector2D position;
    Vector2D velocity;
    Vector2D acceleration;
    float mass;
    float friction;

    Object(Vector2D position = Vector2D(), Vector2D velocity = Vector2D(), float mass = 1.0f, float friction = 0.0f)
        : position(position), velocity(velocity), mass(mass), friction(friction) {}

    void update(float dt) {
        // Calculate net force
        Vector2D netForce = Vector2D(0.0f, mass * 9.81f); // Gravity
        netForce = netForce - (velocity * friction); // Friction

        // Calculate acceleration
        acceleration = netForce * (1.0f / mass);

        // Calculate velocity
        velocity = velocity + (acceleration * dt);

        // Calculate position
        position = position + (velocity * dt);
    }
};

class Platform : public Object {
public:
    Vector2D size;

    Platform(Vector2D position = Vector2D(), Vector2D velocity = Vector2D(), Vector2D size = Vector2D(1.0f, 1.0f), float friction = 0.5f)
        : Object(position, velocity, std::numeric_limits<float>::infinity(), friction), size(size) {}
};

class Player : public Object {
public:
    Vector2D size;

    Player(Vector2D position = Vector2D(), Vector2D velocity = Vector2D(), Vector2D size = Vector2D(1.0f, 1.0f), float mass = 70.0f, float friction = 0.2f)
        : Object(position, velocity, mass, friction), size(size) {}

    void jump() {
        velocity.y = -10.0f;
    }
};

int main() {
    // Create objects
    Platform platform(Vector2D(0.0f, 1.0f), Vector2D(), Vector2D(5.0f, 0.5f));
    Player player(Vector2D(0.0f, 5.0f), Vector2D(), Vector2D(0.5f, 1.0f));

    // Update game loop
    float dt = 1.0f / 60.0f;
    for (int i = 0; i < 60; i++) {
        player.update(dt);
        platform.update(dt);

        // Check for collision between player and platform
        if (player.position.x + player.size.x >= platform.position.x &&
            player.position.x <= platform.position.x + platform.size.x &&
            player.position.y + player.size.y >= platform.position.y &&
            player.position.y <= platform.position.y + platform.size.y) {
            // Player collided with platform, adjust velocity
            player.velocity.y = 0;
        // Move player to top of platform
        player.position.y = platform.position.y + platform.size.y;
    }

    std::cout << "Player: (" << player.position.x << ", " << player.position.y << ")" << std::endl;
    std::cout << "Platform: (" << platform.position.x << ", " << platform.position.y << ")" << std::endl;
}

return 0;
}