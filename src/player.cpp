#pragma once

#include <iostream>
#include <String>
#include <cmath>

#include "SDL2/SDL_image.h"
//#include "SDL2/SDL_mixer.h"

#include "player.h"

using namespace std;

Player::Player()
{
    cropRect = { 0, 0, 16, 16 };
    texture = nullptr;
    spritesheet_texture_w = spritesheet_texture_h = 0; //size of spritesheet texture
    isActive = false; //animation timer
    keys[6];
    frame_w = frame_h = 16; //16, 16
    frameTimerMax = 0.5f;
    frameTimer = 0; //0..frameTimerMax
    frame = 0; //0..1
    frameMax = 2; //2
    way = 0; //0..3
    classType = 0; //0..53
    moveSpeed = 200.0f;
    radius = frame_w / 2;
    //soundEffect = nullptr;
    key_cooldown_timer = 0;
    cast_cooldown_max = 190.0f;

    static int playerNumber = 0;
    playerNumber++;

    name = "Player:" + to_string(playerNumber);

    // Setup keys for player 1
    // ** I see the enum didn't exist when this was written **
    if (playerNumber == 1)
    {
        keys[0] = SDL_SCANCODE_W;
        keys[1] = SDL_SCANCODE_S;
        keys[2] = SDL_SCANCODE_A;
        keys[3] = SDL_SCANCODE_D;
        keys[4] = SDL_SCANCODE_KP_MINUS;
        keys[5] = SDL_SCANCODE_KP_PLUS;
    }
    else
    {
        // Setup keys all other players (not 1)
        keys[0] = SDL_SCANCODE_UP;
        keys[1] = SDL_SCANCODE_DOWN;
        keys[2] = SDL_SCANCODE_LEFT;
        keys[3] = SDL_SCANCODE_RIGHT;
        keys[4] = SDL_SCANCODE_KP_DIVIDE;
        keys[5] = SDL_SCANCODE_KP_MULTIPLY;
    }

}    //Player::Player()

void Player::Player_new(SDL_Texture* spritesheet_texture, int x, int y)
{
    // This is new player
    isActive = false;

    // Read size of spritesheet texture
    SDL_QueryTexture(spritesheet_texture, NULL, NULL, &spritesheet_texture_w, &spritesheet_texture_h);

    texture = spritesheet_texture;

    // spritesheet rect
    cropRect = { 0, 0, 16, 16 };

    // player screen position
    positionRect = { x, y, 16, 16 };

    // Defaults
    moveSpeed = 200.0f;
    radius = frame_w / 2;
    frameTimer = 0;
    frame = 0;
    frameMax = 2;
    way = 0;
    classType = 0;
    originX = frame_w / 2;
    originY = frame_h / 2;

    //soundEffect = Mix_LoadWAV("../../Sounds/whipcrack1.wav");

    cout << "Name: " << name << endl;

}    //Player::Player_new()

void Player::Update(float delta, const Uint8* keyState)
{
    isActive = true;
    if (keyState[keys[ePlayer_control_up]])
    {
        way = 0;
        positionRect.y -= moveSpeed * delta;
        cropRect.x = (way * cropRect.w * frameMax) + (frame * cropRect.w);
    }
    if (keyState[keys[ePlayer_control_down]])
    {
        way = 1;
        positionRect.y += moveSpeed * delta;
        cropRect.x = (way * cropRect.w * frameMax) + (frame * cropRect.w);
    }
    if (keyState[keys[ePlayer_control_left]])
    {
        way = 3;
        positionRect.x -= moveSpeed * delta;
        cropRect.x = (way * cropRect.w * frameMax) + (frame * cropRect.w);
    }
    if (keyState[keys[ePlayer_control_right]])
    {
        way = 2;
        positionRect.x += moveSpeed * delta;
        cropRect.x = (way * cropRect.w * frameMax) + (frame * cropRect.w);
    }
    if (keyState[keys[ePlayer_control_type_minus]])
    {
        if (SDL_GetTicks() - key_cooldown_timer > cast_cooldown_max)
        {
            key_cooldown_timer = SDL_GetTicks();
            //Mix_PlayChannel(-1, soundEffect, 0);
            classType--;
            if (classType < 0)
                classType = spritesheet_texture_h / cropRect.h - 1;
            cropRect.y = classType * cropRect.h;
        }
    }
    if (keyState[keys[ePlayer_control_type_plus]])
    {
        if (SDL_GetTicks() - key_cooldown_timer > cast_cooldown_max)
        {
            key_cooldown_timer = SDL_GetTicks();
            //Mix_PlayChannel(-1, soundEffect, 0);
            classType++;
            if (classType >= spritesheet_texture_h / cropRect.h)
                classType = 0;
            cropRect.y = classType * cropRect.h;
        }
    }

    if (isActive)
    {
        frameTimer += delta;

        if (frameTimer >= frameTimerMax)
        {
            frameTimer = 0;
            cropRect.x = (way * cropRect.w * frameMax) + (frame * cropRect.w);
            if (frame >= frameMax - 1)
                frame = 0;
            else
                frame++;
        }
    }
    else
    {
        frameTimer = 0;
    }
}    //Player::Update()

void Player::Draw(SDL_Renderer* renderTarget, SDL_Rect cameraRect)
{
    SDL_Rect drawingRect = { positionRect.x - cameraRect.x, positionRect.y - cameraRect.y, positionRect.w, positionRect.h };
    SDL_RenderCopy(renderTarget, texture, &cropRect, &drawingRect);
}

bool Player::IntersectsWith(Player& p)
{
    if (sqrt(pow(GetOriginX() - p.GetOriginX(), 2) + pow(GetOriginY() - p.GetOriginY(), 2)) >= radius + p.GetRadius())
    {
        SDL_SetTextureColorMod(texture, 255, 255, 255);
        return false;
    }
    SDL_SetTextureColorMod(texture, 255, 0, 0);
    return true;
}

Player::~Player()
{
    SDL_DestroyTexture(texture);
    //Mix_FreeChunk (soundEffect);
}

int Player::GetOriginX()
{
    return positionRect.x + originX;
}

int Player::GetOriginY()
{
    return positionRect.y + originY;
}

int Player::GetRadius()
{
    return radius;
}
