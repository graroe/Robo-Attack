This project consists of two programs that should be compiled seperately.
Program 1 is all files directory "Modeller" (surfaceModeller.cpp, surfaceModeller.h, and subdivcurve.h)
Program 2 is all files directory "Viewer" (MeshViewer.cpp, MeshViewer.h)

The compiled executables should both be executed in the SAME directory in order to faciliate the file 
import/export requirement, because the Modeller exports to its local directory and the Viewer automatically reads from its local directory.

Rather than use a .obj file, I used my own 3D object configuration that is stored as a txt file.
This import/export file is saved under the pre-defined name "savedMesh.txt" upon export by the Modeller.
The Viewer will automatically render the object in this file when exectued.
The Viewer will not render an object unless the is present in its local directory.
If Modeller exports another object, it will overwrite this file, storing the new object.

Mouse Inputs:
-Right mouse: horizontal camera pivot
-Left mouse: vertical camera pivot
-Mousewheel: camera zoom

Key Inputs (NOTE: 3D window must be selected)
-p: toggle display as points
-l: toggle display as lines
-n: toggle normals
-e: export shape to file

The Viewer program uses the same inputs, except for export.
