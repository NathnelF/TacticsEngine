
1. Implement turns
2. Once per entity per turn, compute movement cost to all tiles (or all tiles in a max range)
3. Cache the computed tiles and their cost to reach.
4. Generate movement ranges based on all tiles with a cost less than speed.
5. Reconstruct a minimum cost path for each tile within movement range.
6. Outline the movement range
    a. Search tiles within range to see if they border a higher cost tile
    b. 
