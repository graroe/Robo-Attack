
This project is a game prototype made entirely in c++, using the openGL API
the project is comprised of the following files:
	Scene.cpp (comtains main function)
	Robot3D.h (defines an object class)
	Robot3D.cpp (methods for same class)
	MeshLoader.cpp (loads object from savedMesh.txt)
	MeshLoader.h (header file for above)
	QuadMesh.cpp (ground mesh, reused from project 1)
	QuadMesh.h (header for above)
	VECTOR3D.h (defines class, reused from project 2)
	savedMesh.txt (object file produced using Mesh Modeller program, also available in this repo)

All files above should be stored/compiled in the same directory.
Robots' speed and firing rate may vary slightly, no guarantee they will hit player every game
One hit is enough to kill a robot or the player cannon
destroyed robots simply disappear (no special animation). Destoryed cannon has a retreat animation.

The game can end in one of 3 ways:
All robots are destroyed
All living robots walk behind the player
player is hit with a projectile (triggers "death" animation, in which cannon recedes)

If the game ends, the cannon will no longer operate until you restart the game

On the first playthrough after loading up the game, the robots will start in pre-defined positions
However, each time the games ends and the player chooses to "restart", they will be presented
with a new level, with the robot starting positions and angles randomized.

Player inputs:
AIM: Mouse movement
SHOOT: Left mouse button OR spacebar
RESTART/NEW LEVEL: 'r' (only available when game over)
QUIT GAME: 'q'.