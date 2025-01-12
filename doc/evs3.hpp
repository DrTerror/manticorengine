//Setting Up Components

struct TransformComponent : public Component {
    float x, y;
    float width, height;
};

struct VelocityComponent : public Component {
    float dx, dy;
};

struct SpriteComponent : public Component {
    SDL_Texture* texture;
    SDL_Rect srcRect, destRect;
};

struct CollisionComponent : public Component {
    bool isCollidable = true;
};

struct SoundComponent : public Component {
    std::string soundFile
;
};

// Setting Up Systems

class MovementSystem : public System {
public:
    void update(EntityManager& entityManager, float deltaTime) override {
        for (auto& [entityID, mask] : entityManager.entityComponentMask) {
            if (mask.test(getComponentTypeID<TransformComponent>()) &&
                mask.test(getComponentTypeID<VelocityComponent>())) {

                auto* transform = entityManager.getComponent<TransformComponent>(entityID);
                auto* velocity = entityManager.getComponent<VelocityComponent>(entityID);

                transform->x += velocity->dx * deltaTime;
                transform->y += velocity->dy * deltaTime;
            }
        }
    }

};

//
class CollisionSystem : public System {
public:
    void update(EntityManager& entityManager, float deltaTime) override {
        for (auto& [entityA, maskA] : entityManager.entityComponentMask) {
            if (!maskA.test(getComponentTypeID<CollisionComponent>())) continue;

            auto* transformA = entityManager.getComponent<TransformComponent>(entityA);
            if (!transformA) continue;

            for (auto& [entityB, maskB] : entityManager.entityComponentMask) {
                if (entityA == entityB || !maskB.test(getComponentTypeID<CollisionComponent>())) continue;

                auto* transformB = entityManager.getComponent<TransformComponent>(entityB);
                if (!transformB) continue;

                // Check for overlap
                if (transformA->x < transformB->x + transformB->width &&
                    transformA->x + transformA->width > transformB->x &&
                    transformA->y < transformB->y + transformB->height &&
                    transformA->y + transformA->height > transformB->y) {
                    std::cout << "Collision detected between Entity " << entityA
                              << " and Entity " << entityB << std::endl;
                }
            }
        }
    
    }
};


//
class RenderSystem : public System {
public:
    void update(EntityManager& entityManager, float deltaTime) override {
        for (auto& [entityID, mask] : entityManager.entityComponentMask) {
            if (mask.test(getComponentTypeID<SpriteComponent>()) &&
                mask.test(getComponentTypeID<TransformComponent>())) {

                auto* sprite = entityManager.getComponent<SpriteComponent>(entityID);
                auto* transform = entityManager.getComponent<TransformComponent>(entityID);

                sprite->destRect.x = static_cast<int>(transform->x);
                sprite->destRect.y = static_cast<int>(transform->y);
                SDL_RenderCopy(renderer, sprite->texture, &sprite->srcRect, &sprite->destRect);
            }
        }
    }

private:
    SDL_Renderer* renderer;

public:
    explicit RenderSystem(SDL_Renderer* renderer) : renderer(renderer) {
      
    }
};


