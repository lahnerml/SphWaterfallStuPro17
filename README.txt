SphWaterfall - README

1. Files/Folders

SphWaterfall	--The executable. Only this executable is needed to run the program
/output			--This folder will contain the rendered images. If it's missing, the program will crash during rendering
/vtk			--This folder will contain the simulated particles per timestep as vtk.
*.cfg			--A config file. It can contain a list of any console command separated by linebreaks. Commands will be executed sequentially. '#' marks a comment. The last command of a config file has to be 'exit' to return to normal input.
*.obj			--A 3d-mesh. The file format is commonly used and documented pretty well. Only vertices and faces are used. Faces must be triangles.

2. Console commands

--This text can be displayed by entering the command 'help' while running the program. Commands are case-insensitive.

Flags:
   -p | file path to the file you want to load
   -t | defines at what time (in sonconds) a command specific event occurs
      | simulate: simulation time
      | moveshutter: time at which the shutter is moved
   -v | followed by 3 numbers x y z, who stand for the coordinates of a point in 3D space
   -h | for addsink which determines the sink height or for render sets the height of the output image
   -w | for render sets the width of the output image

Commands:
	print
		Echo the give input. Mainly for config files.

	loadconfig -p
		Load a configuration from a given file path. The file has to be a .cfg-file.

	loadmesh -p
		Load a mesh from a give file path, the mesh has to be a .obj-file.

	loadshutter -p
		Load a shutter mesh from a give file path, the mesh has to be a .obj-file.

	moveshutter -t
		Set the time at which the shutter will move.

	particlegen
		Generate wall particles on the loaded mesh and shutter.

	addsource -v
		Add a water source at a given point. Multiple sources may be used.

	addsink -h
		Add a sink at a given height.
		
	simulate [-t]
		Start a sph-simulation. Simulated time can be set with '-t' parameter.

	render [-v] [-w -h]
		Start the rendering process. The camera can be set with '-v' parameter. Camera is looking roughly towards (0,0,0)

	help
		Show help

	exit
		Quit the application
		
3. Behavior

- Most console commands will run on their own although it might not be very smart to run the simulation without a terrain or source.
- The initial step of rendering might take very long (about 10 minutes for our test-mesh). Following render steps depend only on the chosen resolution and the number of rendered particles.
- Failing to load a mesh will have the same effect as loading an empty mesh.
- Sources generate an infinite amount of particles.

4. Mesh import

The mesh can be loaded from a file in the .obj format. To create a new mesh use the following settings in your 3D-modelling-software:
- triangulate faces
- no UVs
- no normals
- no materials