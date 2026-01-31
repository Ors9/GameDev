#ifndef GAME_TYPES_H
#define GAME_TYPES_H

typedef enum {
    ENIV_WORD_TERRIAN, // הרצפה העגולה הישנה (אם תרצה להשאיר)
    ENIV_FLOOR_BLOCK,  // המרצפת המרובעת (הבסיס לדרגוניקה)
    ENIV_ROCK_A,       // סלע סוג 1
    ENIV_TREE_PINE,    // עץ אורן
    ENIV_GRASS_DECO,   // קישוט דשא
    ENIVORMENT_COUNT   // תמיד נשאר אחרון - הוא קובע את גודל המערך
} EnivormentResourcesTypes;

typedef enum PlayerAnimationState
{
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

typedef enum CharacterClass
{
    MUTANT_CHAR,
    MONSTER_CHAR,
    CLASS_COUNT
} CharacterClass;

#endif