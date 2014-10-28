define marker_range model
(
    pose [0 0 0 0]
    #bitmap "bitmaps/circle.png"
    
    color "light grey"
    size [4 4 0]
    
    # regular 12 sided polygon
    block(
        points 12
        point[0] [26 -97]
        point[1] [-26 -97]
        point[2] [-71 -71]
        point[3] [-97 -26]
        point[4] [-97 26]
        point[5] [-71 71]
        point[6] [-26 97]
        point[7] [26 97]
        point[8] [71 71]
        point[9] [97 26]
        point[10] [97 -26]
        point[11] [71 -71]
        z [0 0.2]
    )

    gui_nose 0
    gui_grid 0
    gui_move 1
    gui_outline 0

    # insensible to collision and sensors
    gripper_return 0
    fiducial_return 0
    ranger_return 0

    obstacle_return 0 
)

define marker model
(
    boundary 1

    size [0.05 0.05 0]
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
