#ifndef GRID_GRAPH_H
#define GRID_GRAPH_H

#include <Arduino.h>

#define GRID_ROWS 6
#define GRID_COLS 6
#define NUM_NODES (GRID_ROWS * GRID_COLS)
#define MAX_NEIGHBOURS 4
#define NO_NODE 255
#define NO_EDGE 255

typedef struct {
    uint8_t to;
    uint8_t weight;
} Edge;

uint8_t graphNodeCount(void);
bool graphIsValidNode(uint8_t node);
uint8_t graphRow(uint8_t node);
uint8_t graphCol(uint8_t node);

/**
 * Returns the weight of the edge from -> to.
 * Returns NO_EDGE if the two nodes are not directly adjacent.
 */
uint8_t graphGetWeight(uint8_t from, uint8_t to);

/**
 * Fills neighbours[] with up to MAX_NEIGHBOURS valid outgoing edges.
 * Returns the number of neighbours written.
 */
uint8_t graphGetNeighbours(uint8_t node, Edge neighbours[MAX_NEIGHBOURS]);

/**
 * Optional helper for debugging.
 */
void graphPrintNeighbours(Stream &out, uint8_t node);

#endif