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

#endif
