define marker model
(
    boundary 1

    size [0.05 0.05 0.01]
    color [1 0 0 0.2]

    gui_nose 0
    gui_grid 0
    gui_move 1
    gui_outline 0

    # insensible to collision and sensors
    gripper_return 0
    fiducial_return 0
    ranger_return 0

    obstacle_return 0 
    
    #marker_range()
)
