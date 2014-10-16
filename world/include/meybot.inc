
define meybotsonar sensor
(
    size [0.01 0.05 0.01] 
    # define the range bounds [min max]
    range [0 3.0]
	# define the angular field of view in degrees
	fov 30
	# define the number of samples spread over the fov
    samples 1 
)

define meybot_sonars ranger
(
    # define the pose of each transducer [xpos ypos heading]
    meybotsonar( pose [ 0.3 0 0 0 ] )
    meybotsonar( pose [ 0.2 0.2 0 45] )
    meybotsonar( pose [ 0.2 -0.2 0 -45] )

    #defines color of sensor range in gui
    color_rgba [ 0 1 0 0.2 ]
)

define meybot position
(
    # shape will be scaled to this size
    # width length height
    size [0.6 0.6 0.3]
    # centre of rotation relative to geometric mid
    origin [0 0 0 0]

    # create shape of the robot
    # positions of shape corners
    block
    (
        points 8
        point[0] [1 3]
        point[1] [2 3]
        point[2] [3 2]
        point[3] [3 1]
        point[4] [2 0]
        point[5] [1 0]
        point[6] [0 1]
        point[7] [0 2]
        z [0 0.22]
    )
    
    # weight in kg
    mass 10.0
    drive "diff"
    
    obstacle_return 1           	# Can hit things.
    ranger_return 0.5            	# reflects sonar beams
    blob_return 1               	# Seen by blobfinders  
    fiducial_return 1           	# Seen as "1" fiducial finders
       	  
    localization_origin [0 0 0 0] 	# Start odometry at (0, 0, 0).
    localization "odom"         	
    odom_error [ 0.05 0.05 0.0 0.1 ]	# Odometry error or slip in X, Y and Theta
                                   	# (Uniform random distribution)   

    # four DOF kinematics limits
    # [ xmin xmax ymin ymax zmin zmax amin amax ]				
    velocity_bounds [-0.5 0.5 0 0 0 0 -90.0 90.0 ]					
    acceleration_bounds [-0.5 0.5 0 0 0 0 -90 90.0 ]

    meybot_sonars()
)
