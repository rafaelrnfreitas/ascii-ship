#ifndef LEADERBOARD_H
#define LEADERBOARD_H

#define LEADERBOARD_SIZE 10

typedef struct {
    char name[4];
    int score;
} score_entry_t;

typedef struct {
    score_entry_t entries[LEADERBOARD_SIZE];
    int count;
} leaderboard_t;

void leaderboard_load(leaderboard_t* lb, const char* path);
int leaderboard_save(const leaderboard_t* lb, const char* path);
int leaderboard_qualifies(const leaderboard_t* lb, int score);
void leaderboard_insert(leaderboard_t* lb, const char* name, int score);

#endif
