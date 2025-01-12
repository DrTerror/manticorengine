int main(int argc, char* argv[]) {
    SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO);
    SDL_Window* window = SDL_CreateWindow("Platformer", 800, 600, SDL_WINDOW_OPENGL);
    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

    EntityManager entityManager;

    // Create player entity
    auto player = entityManager.createEntity();
    entityManager.addComponent<TransformComponent>(player, 100.0f, 100.0f, 32.0f, 32.0f);
    entityManager.addComponent<VelocityComponent>(player, 0.0f, 0.0f);
    entityManager.addComponent<SpriteComponent>(player, /*... load SDL_Texture ...*/);

    // Systems
    MovementSystem movementSystem;
    CollisionSystem collisionSystem;
    RenderSystem renderSystem(renderer);

    bool running = true;
    float deltaTime = 0.016f; // Assume ~60 FPS

    while (running) {
        // Process input
        SDL_Event event;
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_EVENT_QUIT) running = false;
        }

        // Update systems
        movementSystem.update(entityManager, deltaTime);
        collisionSystem.update(entityManager, deltaTime);

        // Render
        SDL_RenderClear(renderer);
        renderSystem.update(entityManager, deltaTime);
        SDL_RenderPresent(renderer);
    }

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    re
turn 0;
}
