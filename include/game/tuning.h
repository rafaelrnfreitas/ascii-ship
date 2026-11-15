#ifndef TUNING_H
#define TUNING_H

// 0 = seed from the clock; any other value replays the same run
#define GAME_SEED 0

#define SPAWN_Y 85.0f
#define SPAWN_SAFE_DIST 30.0f
#define DESPAWN_Y -12.0f
#define BOUNDS_X 60.0f
#define BOUNDS_Z 40.0f
#define ENEMY_MAX_LIFE 15.0f
#define DYING_TIME 0.25f
#define ENEMY_BLINK 0.05f

#define DRIFTER_SPEED 18.0f
#define CHASER_SPEED 16.0f
#define SHOOTER_SPEED 20.0f
#define HOLD_Y 30.0f
#define BURSTS 2
#define BURST_SHOTS 3
#define BURST_COOLDOWN 1.8f
#define SHOT_GAP 0.25f
#define TELEGRAPH_TIME 0.4f

#define DRIFTER_SWAY 3.0f
#define DRIFTER_SWAY_FREQ 1.5f
#define CHASER_LOOP 3.0f
#define CHASER_LOOP_FREQ 1.2f
#define CHASER_FAN 0.2f

#define ENEMY_SHOT_CAP 48
#define ENEMY_SHOT_SPEED 25.0f
#define ENEMY_SHOT_RADIUS 0.6f

// Points for a laser kill, by type: drifter, chaser, shooter
#define SCORE_TABLE { 100, 150, 250 }

// Waves: {type, count, seconds between spawns}, cycled forever
#define WAVE_TABLE { \
    { ENEMY_DRIFTER, 3, 0.8f }, \
    { ENEMY_DRIFTER, 4, 0.8f }, \
    { ENEMY_CHASER,  2, 2.0f }, \
    { ENEMY_SHOOTER, 2, 1.5f }, \
    { ENEMY_DRIFTER, 4, 0.8f }, \
    { ENEMY_CHASER,  2, 2.0f }, \
    { ENEMY_SHOOTER, 3, 1.2f }, \
}
#define WAVE_GAP 1.5f
#define SPAWN_CENTER_X 4.0f
#define SPAWN_CENTER_Z 2.2f
#define LANE_MIN_SEP 2.5f
#define SPAWN_TRIES 8

#define DIFF_SPEED_STEP 0.08f
#define DIFF_SPEED_MAX 2.0f
#define DIFF_COOLDOWN_STEP 0.1f
#define DIFF_COOLDOWN_MIN 0.4f
#define DIFF_GROW_EVERY 2
#define DIFF_GROW_MAX 1

#endif
