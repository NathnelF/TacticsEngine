
# Grid movement <- complete for now!

# Turns
    1. Player turns
        a. Check if can perform action (done)
        b. Perform action (done) (more to do on specific actions)
        c. end turn (manually right now, needs to be automatic when all units are out of actions);
    2. Enemy turns <- requires enemies!
        a. Need enemy ai to decide on actions.
            b. For now they will just end their turn most likely.
        
# Enemies
    #Here is the issue. A playerUnit contains a gridUnit, and an enemyUnit contains a gridUnit. But my unitGrid is just a list of GridUnits. Which don't know if they are a player or
    an enemy. I could either have, a separate grid for each type and then redo all my functions for each. Or match gridUnit id to player / enemy unit ID so I can easily look them
    up? That way I can look up a unit's id, and then look up if that is a player or enemy unit from there? I could also add a flag to a grid unit that marks if it player or enemy
    and just update that along with the speed since we have precedent for that already.

    
    1. Add a tactical Unit that will represent a player unit.
        a. Contains a gridUnit struct (composition)
        b. will pull from a list of strategy units to get abilities, model, texture, etc..
    2. Add enemy unit that will represent an enmy unit
        a. contains a gridUnit struct (composition)
        b. Will pull from a list of predefined enemy units to get abilities, model, texture etc..


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

# Cover
    1. Directional cover based on terrain. (done)

            

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
