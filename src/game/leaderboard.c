#include <stdio.h>
#include <string.h>
#include "game/leaderboard.h"

void leaderboard_load(leaderboard_t* lb, const char* path) {
    lb->count = 0;
    FILE* f = fopen(path, "r");
    if(!f) return;

    char line[64], name[4];
    int score;
    while(fgets(line, sizeof line, f))
        if(sscanf(line, "%3[A-Z] %d", name, &score) == 2 && strlen(name) == 3 && score >= 0)
            leaderboard_insert(lb, name, score);
    fclose(f);
}

int leaderboard_save(const leaderboard_t* lb, const char* path) {
    FILE* f = fopen(path, "w");
    if(!f) {
        perror(path);
        return 1;
    }
    for(int i = 0; i < lb->count; i++)
        fprintf(f, "%s %d\n", lb->entries[i].name, lb->entries[i].score);
    fclose(f);
    return 0;
}

int leaderboard_qualifies(const leaderboard_t* lb, int score) {
    return score > 0 && (lb->count < LEADERBOARD_SIZE || score > lb->entries[lb->count - 1].score);
}

void leaderboard_insert(leaderboard_t* lb, const char* name, int score) {
    int i = 0;
    while(i < lb->count && lb->entries[i].score >= score) i++;
    if(i >= LEADERBOARD_SIZE) return;

    if(lb->count < LEADERBOARD_SIZE) lb->count++;
    memmove(&lb->entries[i + 1], &lb->entries[i], (lb->count - 1 - i) * sizeof(score_entry_t));
    snprintf(lb->entries[i].name, sizeof lb->entries[i].name, "%s", name);
    lb->entries[i].score = score;
}
