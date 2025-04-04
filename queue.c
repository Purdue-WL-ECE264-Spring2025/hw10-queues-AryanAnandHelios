
#include "queue.h"
#include "tile_game.h"
#include <stdbool.h>
#include <stdlib.h>

void enqueue(struct queue *puzzleQueue, struct game_state currentState) {
  uint64_t stateHash = serialize(currentState);
  insert_at_tail(&puzzleQueue->data, stateHash);
}

struct game_state dequeue(struct queue *puzzleQueue) {
  uint64_t stateHash = remove_from_head(&puzzleQueue->data);
  return deserialize(stateHash);
}

int number_of_moves(struct game_state startState) {
  struct queue moveQueue = {0};
  struct linked_list seenStates = {0};

  enqueue(&moveQueue, startState);
  uint64_t startStateHash = serialize(startState);
  insert_at_tail(&seenStates, startStateHash);

  while (moveQueue.data.head) {
    struct game_state currentPuzzle = dequeue(&moveQueue);
    
    bool puzzleSolved = true;
    for (int row = 0; row < 4 && puzzleSolved; row++) {
      for (int col = 0; col < 4; col++) {
        int expectedTile = row * 4 + col + 1;
        if (row == 3 && col == 3) {
          expectedTile = 0;
        }
        if (currentPuzzle.tiles[row][col] != expectedTile) {
          puzzleSolved = false;
          break;
        }
      }
    }

    if (puzzleSolved) {
      free_list(seenStates);
      free_list(moveQueue.data);
      return currentPuzzle.num_steps;
    }

    struct game_state possibleMoves[4];
    possibleMoves[0] = currentPuzzle; move_up(&possibleMoves[0]);
    possibleMoves[1] = currentPuzzle; move_down(&possibleMoves[1]);
    possibleMoves[2] = currentPuzzle; move_left(&possibleMoves[2]);
    possibleMoves[3] = currentPuzzle; move_right(&possibleMoves[3]);

    for (int moveIndex = 0; moveIndex < 4; moveIndex++) {
      if (possibleMoves[moveIndex].num_steps > currentPuzzle.num_steps) {
        uint64_t newStateHash = serialize(possibleMoves[moveIndex]);
        
        bool alreadySeen = false;
        struct list_node *currentNode = seenStates.head;
        while (currentNode) {
          if (currentNode->value == newStateHash) {
            alreadySeen = true;
            break;
          }
          currentNode = currentNode->next;
        }

        if (!alreadySeen) {
          enqueue(&moveQueue, possibleMoves[moveIndex]);
          insert_at_tail(&seenStates, newStateHash);
        }
      }
    }
  }

  free_list(seenStates);
    return -1;
  }

