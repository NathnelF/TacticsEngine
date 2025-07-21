
# Grid movement.
1. Tab select between entities.
2. Add waypoint system
    - Refactor calculate movement range to take startX and starY as parameters
        - On unit selection, check if there are any waypoints.
            - if there are we calculate the movement range from the last one.
            - if there aren't we calculate the movement range from the unit's current tile.
        - upon adding a waypoint we clear the movemenet range and recalculate it from latest waypoint
        - only allow waypoint addition in the movement range

    - alternate idea. Add a waypoint vector2 to units
        - calculate movement range from the waypoint vector2
        - adding a waypoint will just move the waypoint vector2
        - we would need to store the temp cost somehow. oh wait we just get the starting cost from that tiles cost in movement range.
3. Add blue versus yellow movement ranges
-- I think this wraps up grid movement. A few areas to go to next.

# Turns

# Enemies

# Abilties

# Line of sight

# Cover

# Detection

# Elevation

# AI

# Maps / Map Generation

# Game layers (Tactical / strategy)

# Start screen

# Saving


Nice to haves:
1. Make diagonal movement take longer than perpendicular?
