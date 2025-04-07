#include "queue.h"
#include "tile_game.h"

#include <stdlib.h>
#include <stdbool.h>

#define VISITED_SIZE (1 << 24) // 16M bits

struct visited_set {
    bool *visited;
};

void visited_set_init(struct visited_set *vs) {
    vs->visited = calloc(VISITED_SIZE, sizeof(bool));
}

void visited_set_free(struct visited_set *vs) {
    free(vs->visited);
}

// Returns true if already visited; false otherwise and marks as visited
bool visited_set_check_and_mark(struct visited_set *vs, uint64_t serialized) {
    size_t index = serialized % VISITED_SIZE;
    if (vs->visited[index]) {
        return true;
    }
    vs->visited[index] = true;
    return false;
}
int is_goal(struct game_state state) {
    int expected = 1;
    for (int row = 0; row < 4; row++) {
        for (int col = 0; col < 4; col++) {
            if (row == 3 && col == 3)
                return state.tiles[row][col] == 0;
            if (state.tiles[row][col] != expected++)
                return 0;
        }
    }
    return 1;
}
void enqueue(struct queue *q, struct game_state state) {
    uint64_t nums = serialize(state);
    insert_at_tail(&(q->data), nums);
}

struct game_state dequeue(struct queue *q) { 
    size_t value = remove_from_head(&(q->data));
    return (deserialize(value)); 
}

int number_of_moves(struct game_state start) { 
    struct visited_set vs;
    visited_set_init(&vs);

    struct queue q = {.data.head = NULL};
    enqueue(&q, start);

    uint64_t start_serial = serialize(start);
    visited_set_check_and_mark(&vs, start_serial);

    while (q.data.head != NULL) {
        struct game_state current = dequeue(&q);

        if (is_goal(current)) {
            visited_set_free(&vs);
            free_list(q.data);
            return current.num_steps;
        }

        for (int dir = 0; dir < 4; dir++) {
            struct game_state next = current;

            switch (dir) {
                case 0: move_up(&next); break;
                case 1: move_down(&next); break;
                case 2: move_left(&next); break;
                case 3: move_right(&next); break;
            }

            // skip if move didn't happen
            if (next.num_steps == current.num_steps)
                continue;

            uint64_t next_serial = serialize(next);
            if (!visited_set_check_and_mark(&vs, next_serial)) {
                enqueue(&q, next);
            }
        }
    }

    visited_set_free(&vs);
    free_list(q.data);
    return -1;
}


