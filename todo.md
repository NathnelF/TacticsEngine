
# Grid movement <- complete for now!
    1. Updates meshes / shader effects for position previews.

# Turns
    1. Player turns
        c. end turn (manually right now, needs to be automatic when all units are out of actions);
    2. Enemy turns <- requires enemies!
        a. Need enemy ai to decide on actions.
            b. For now they will just end their turn most likely.
        
# Enemies


# Abilties
    1. System scaffolding complete
        a. Ability id + registry
        b. To add an abillity you give it an ID and then write the execute logic for that ability
    
    2. Todo
        a. Add lots and lots of abilities.
        b. Figure out how to handle abilities that affect other abilities. (i.e: free grenade throw, next shot ignores cover ...etc);
        c. area of affect abilities.
        d. target unit abilities.
        e. target environment abilities.

# Line of sight
    1. LOS calculations complete for now need testing
    2. Remove target icon or change color if an enemy is not in LOS

# Cover
    1. Directional cover based on terrain. (done)
    2. Update meshes / add shader effects for cover

            

# Detection

# Fog of war

# Elevation

# AI

# Maps / Map Generation

# Game layers (Tactical / strategy)

# Equipment

# Start screen

# Saving

# Animation

# Real meshes, models, and textures


Nice to haves:
1. Make diagonal movement take longer than perpendicular?
2. Show line preview when hovering over last waypoint tile rather than only other tiles in range from last waypoint.
