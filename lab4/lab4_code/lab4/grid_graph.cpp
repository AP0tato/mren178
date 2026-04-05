#include "grid_graph.h"

/*
 * Horizontal edge weights.
 * hWeight[r][c] is the weight between:
 *   node(r, c) and node(r, c+1)
 *
 * There are GRID_ROWS rows and GRID_COLS - 1 horizontal edges per row.
 */
static const uint8_t hWeight[GRID_ROWS][GRID_COLS - 1] = {
    {4, 6, 3, 8, 2},
    {3, 4, 7, 6, 3},
    {4, 6, 2, 4, 6},
    {4, 5, 3, 6, 5},
    {4, 5, 4, 3, 5},
    {5, 4, 6, 5, 3}
};

/*
 * Vertical edge weights.
 * vWeight[r][c] is the weight between:
 *   node(r, c) and node(r+1, c)
 *
 * There are GRID_ROWS - 1 rows of vertical edges and GRID_COLS columns.
 */
static const uint8_t vWeight[GRID_ROWS - 1][GRID_COLS] = {
    {7, 5, 4, 5, 7, 6},
    {5, 6, 5, 4, 7, 5},
    {7, 5, 6, 3, 4, 5},
    {6, 7, 4, 7, 6, 3},
    {7, 6, 6, 4, 6, 4}
};

static uint8_t nodeIndex(uint8_t row, uint8_t col) {
    return (uint8_t)(row * GRID_COLS + col);
}

uint8_t graphNodeCount(void) {
    return NUM_NODES;
}

bool graphIsValidNode(uint8_t node) {
    return node < NUM_NODES;
}

uint8_t graphRow(uint8_t node) {
    return (uint8_t)(node / GRID_COLS);
}

uint8_t graphCol(uint8_t node) {
    return (uint8_t)(node % GRID_COLS);
}

uint8_t graphGetWeight(uint8_t from, uint8_t to) {
    if (!graphIsValidNode(from) || !graphIsValidNode(to)) {
        return NO_EDGE;
    }

    uint8_t r1 = graphRow(from);
    uint8_t c1 = graphCol(from);
    uint8_t r2 = graphRow(to);
    uint8_t c2 = graphCol(to);

    // Same row -> horizontal neighbour?
    if (r1 == r2) {
        if ((uint8_t)(c1 + 1) == c2) {
            return hWeight[r1][c1];
        }
        if ((uint8_t)(c2 + 1) == c1) {
            return hWeight[r1][c2];
        }
    }

    // Same column -> vertical neighbour?
    if (c1 == c2) {
        if ((uint8_t)(r1 + 1) == r2) {
            return vWeight[r1][c1];
        }
        if ((uint8_t)(r2 + 1) == r1) {
            return vWeight[r2][c1];
        }
    }

    return NO_EDGE;
}

uint8_t graphGetNeighbours(uint8_t node, Edge neighbours[MAX_NEIGHBOURS]) {
    if (!graphIsValidNode(node) || neighbours == NULL) {
        return 0;
    }

    uint8_t count = 0;
    uint8_t row = graphRow(node);
    uint8_t col = graphCol(node);

    // Up
    if (row > 0 && count < MAX_NEIGHBOURS) {
        uint8_t other = nodeIndex((uint8_t)(row - 1), col);
        neighbours[count].to = other;
        neighbours[count].weight = graphGetWeight(node, other);
        count++;
    }

    // Down
    if ((uint8_t)(row + 1) < GRID_ROWS && count < MAX_NEIGHBOURS) {
        uint8_t other = nodeIndex((uint8_t)(row + 1), col);
        neighbours[count].to = other;
        neighbours[count].weight = graphGetWeight(node, other);
        count++;
    }

    // Left
    if (col > 0 && count < MAX_NEIGHBOURS) {
        uint8_t other = nodeIndex(row, (uint8_t)(col - 1));
        neighbours[count].to = other;
        neighbours[count].weight = graphGetWeight(node, other);
        count++;
    }

    // Right
    if ((uint8_t)(col + 1) < GRID_COLS && count < MAX_NEIGHBOURS) {
        uint8_t other = nodeIndex(row, (uint8_t)(col + 1));
        neighbours[count].to = other;
        neighbours[count].weight = graphGetWeight(node, other);
        count++;
    }

    return count;
}

void graphPrintNeighbours(Stream &out, uint8_t node) {
    Edge neighbours[MAX_NEIGHBOURS];
    uint8_t count = graphGetNeighbours(node, neighbours);

    out.print("Node ");
    out.print(node);
    out.print(" neighbours: ");

    for (uint8_t i = 0; i < count; i++) {
        out.print("[");
        out.print(neighbours[i].to);
        out.print(", w=");
        out.print(neighbours[i].weight);
        out.print("]");
        if (i + 1 < count) {
            out.print(" ");
        }
    }
    out.println();
}