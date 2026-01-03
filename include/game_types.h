#ifndef GAME_TYPES_H
#define GAME_TYPES_H



typedef enum PlayerAnimationState {
    PLAYER_IDLE,
    PLAYER_WALK,
    PLAYER_RUN,
    PLAYER_PUNCH,
    PLAYER_SWIPE,
    PLAYER_JUMP,
    PLAYER_JUMP_ATTACK,
    PLAYER_ROAR,
    PLAYER_FLEX,
    PLAYER_DIE,
    ANIM_COUNT
} PlayerAnimationState;

typedef enum CharacterClass {
    MUTANT_CHAR,
    MONSTER_CHAR,
    CLASS_COUNT
} CharacterClass;


#endif