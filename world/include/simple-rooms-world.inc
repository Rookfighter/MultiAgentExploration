include "include/map.inc"
include "include/marker.inc"
include "include/pioneer.inc"

window
(
    size [800 600]
    
    # GUI options
    show_data 0
    show_flags 1
    show_blocks 1
    show_clock 1
    show_footprints 0
    show_grid 1
    show_trailarrows 0
    show_trailrise 0
    show_trailfast 0
    show_occupancy 0
    pcam_on 0
    screenshots 0
)

floorplan
(
    name "floorplan1"
    bitmap "bitmaps/simple_rooms.png"
    size [15 15 1]
)
