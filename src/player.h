#pragma once

#include <iostream>
#include <String>

#include "SDL2/SDL.h"
//#include "SDL2/SDL_mixer.h"

using namespace std;

enum player_control
{
    ePlayer_control_up,
    ePlayer_control_down,
    ePlayer_control_left,
    ePlayer_control_right,
    ePlayer_control_type_minus,
    ePlayer_control_type_plus
};

class Player
{
    private:
        SDL_Rect cropRect;
        SDL_Texture* texture;
        int spritesheet_texture_w, spritesheet_texture_h; //size of spritesheet texture
        bool isActive; //animation timer
        SDL_Scancode keys[6];
        int frame_w, frame_h; //16, 16
        float frameTimerMax;
        float frameTimer; //0..frameTimerMax
        int frame; //0..1
        int frameMax; //2
        int way; //0..3
        int classType; //0..53
        float moveSpeed;
        float cast_cooldown_max;
        //Mix_Chunk* soundEffect;
        int originX, originY;
        int radius;
        Uint32 key_cooldown_timer;
        string name;

    public:
        void Player_new(SDL_Texture* spritesheet_texture, int x, int y);
        Player();
        ~Player();

        void Update(float delta, const Uint8* keyState);
        void Draw(SDL_Renderer* renderTarget, SDL_Rect cameraRect);
        bool IntersectsWith(Player& p);
        int GetOriginX();
        int GetOriginY();
        int GetRadius();

        SDL_Rect positionRect;

};
