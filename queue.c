
#include "queue.h"
#include "tile_game.h"
#include <stdbool.h>
#include <stdlib.h>

/* Adds a game state to the queue by serializing it into a 64-bit integer
 * Uses insert_at_tail to maintain FIFO order
 */
void enqueue(struct queue *q, struct game_state state) {
  uint64_t serialized = serialize(state);
  insert_at_tail(&q->data, serialized);
}

/* Removes and returns the first game state from the queue
 * Deserializes the stored integer back into a game state
 */
struct game_state dequeue(struct queue *q) {
  uint64_t serialized = remove_from_head(&q->data);
  return deserialize(serialized);
}

/* Checks if a game state represents the solved puzzle
 * The goal state has tiles arranged in ascending order with 0 in bottom right
 */
/* Finds the minimum number of moves needed to solve the puzzle using BFS
 * Returns -1 if no solution is found
 * Uses a visited list to avoid cycles and prevent infinite loops
 */
int number_of_moves(struct game_state start) {
  struct queue q = {0};
  struct linked_list visited = {0};
  
  enqueue(&q, start);
  uint64_t start_serialized = serialize(start);
  insert_at_tail(&visited, start_serialized);
  
  while (q.data.head) {
    struct game_state current = dequeue(&q);
    
    // Check if current state is the goal state
    bool is_goal = true;
    for (int i = 0; i < 4 && is_goal; i++) {
      for (int j = 0; j < 4; j++) {
        int expected = i * 4 + j + 1;
        if (i == 3 && j == 3) expected = 0;
        if (current.tiles[i][j] != expected) {
          is_goal = false;
          break;
        }
      }
    }
    if (is_goal) 
    {
      free_list(visited);
      free_list(q.data);
      return current.num_steps;
    }
    
    // Try all possible moves: up, down, left, right
    struct game_state moves[4];
    moves[0] = current; move_up(&moves[0]);
    moves[1] = current; move_down(&moves[1]);
    moves[2] = current; move_left(&moves[2]);
    moves[3] = current; move_right(&moves[3]);
    
    for (int i = 0; i < 4; i++) {
      if (moves[i].num_steps > current.num_steps) {
        uint64_t move_serialized = serialize(moves[i]);
        
        // Check if this state was already visited
        bool was_visited = false;
        struct list_node *node = visited.head;
        while (node) {
          if (node->value == move_serialized) {
            was_visited = true;
            break;
          }
          node = node->next;
        }
        
        if (!was_visited) {
          enqueue(&q, moves[i]);
          insert_at_tail(&visited, move_serialized);
        }
      }
    }
  }
  
  free_list(visited);
  return -1; // No solution found
}
