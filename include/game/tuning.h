#ifndef TUNING_H
#define TUNING_H

// 0 = seed from the clock; any other value replays the same run
#define GAME_SEED 0

// Enemy lifecycle (world units; camera sits at y=-10 looking toward +y)
#define SPAWN_Y 85.0f            // enemies appear far away, near the far plane
#define SPAWN_SAFE_DIST 30.0f    // never spawn closer than this to the player
#define ENTER_Y 40.0f            // ENTERING ends once closer than this
#define DESPAWN_Y -12.0f         // behind the camera
#define BOUNDS_X 60.0f
#define BOUNDS_Z 40.0f
#define ENEMY_MAX_LIFE 15.0f     // safety net: force LEAVING after this long
#define DYING_TIME 0.25f
#define ENEMY_BLINK 0.05f

// Drifter: straight run with a gentle side-to-side sway
#define DRIFTER_SPEED 18.0f
#define DRIFTER_SWAY 2.0f        // peak sideways speed
#define DRIFTER_SWAY_FREQ 1.5f

// Chaser: steers toward the player with a capped turn rate
#define CHASER_SPEED 16.0f
#define CHASER_TURN 1.2f         // rad/s
#define CHASER_COMMIT_DIST 6.0f  // stops steering this close in depth, so it always flies past

// Shooter: parks, fires telegraphed bursts, then leaves
#define SHOOTER_SPEED 20.0f
#define SHOOTER_HOLD_Y 30.0f
#define SHOOTER_BURSTS 2
#define SHOOTER_BURST 3
#define SHOOTER_COOLDOWN 1.8f    // between bursts
#define SHOOTER_SHOT_GAP 0.25f   // between shots in a burst
#define TELEGRAPH_TIME 0.4f      // blink before each burst

// Enemy shots
#define ENEMY_SHOT_CAP 24
#define ENEMY_SHOT_SPEED 25.0f
#define ENEMY_SHOT_RADIUS 0.6f

// Waves: {type, count, seconds between spawns}, cycled forever
#define WAVE_TABLE { \
    { ENEMY_DRIFTER, 4, 0.6f }, \
    { ENEMY_DRIFTER, 5, 0.4f }, \
    { ENEMY_CHASER,  3, 1.0f }, \
    { ENEMY_SHOOTER, 2, 1.5f }, \
    { ENEMY_DRIFTER, 6, 0.5f }, \
    { ENEMY_CHASER,  4, 0.8f }, \
    { ENEMY_SHOOTER, 3, 1.2f }, \
}
#define WAVE_GAP 1.5f            // pause after the screen is clear
// Spawn lanes: where an enemy sits on screen, measured on the player's plane (visible: about +-5.4 x +-3.0)
#define SPAWN_CENTER_X 4.0f      // lanes stay in the central ~70% of the screen
#define SPAWN_CENTER_Z 2.2f
#define LANE_MIN_SEP 2.5f        // keep lanes this far apart so one burst can't sweep a group
#define SPAWN_TRIES 8            // candidates per spawn; the one farthest from other enemies wins

// Difficulty rises by one level per cleared wave
#define DIFF_SPEED_STEP 0.08f
#define DIFF_SPEED_MAX 2.0f
#define DIFF_COOLDOWN_STEP 0.1f
#define DIFF_COOLDOWN_MIN 0.4f   // cooldown multiplier floor
#define DIFF_GROW_EVERY 2        // +1 enemy per wave every N levels
#define DIFF_GROW_MAX 3          // at most this many extra enemies per wave, so waves never become swarms

#endif
