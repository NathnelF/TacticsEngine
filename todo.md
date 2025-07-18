Grid:
    - Draw borders around The last tile in blue / yellow move range
    - because we now have a cost associated with each tile it tells you how far it is from the player
    - Which should let us wrap around tiles easier.

Tile:
    - Make a TileNode struct that has it's cost to move to

Entity:
    - Calculate TileNodes that are in blue / yellow move range on select.
    - 


Optimization:
    If we run into path calculation issues play with calculating paths once and then storing them on selection.
    It might make waypoints a little trickier though.
