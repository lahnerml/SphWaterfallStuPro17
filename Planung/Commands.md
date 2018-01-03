# Commands
The Project comes with a console interface for controlling the simulation. Below are the commands of this console interface.

- loadMesh [-p path]

        Loads a mesh fom an .obj file.

        -p: The path of the obj file

- particleGen [-s|-f|-e]

        Generates initial particle positions.
        
        -s: Static Particles
        -f: Fluid Particles
        -e: End particle generation

- moveShutter [-t stattime|-u|-d|-l]

        Makes the shutter move at a specified point in time. Repeated moevements in the same direction will be ignored. Movements a the same timestamp will override the previous ones.

        -t: The time when the shutter starts moving
        -u: Up
        -d: Down
        -l: Shows a list of all set shutter movements

- simulate [-s|-e|-r depth|-g rate position|-m|-t time]

        Starts/Ends the simulation. Waits for the particleGen to finish if one is currently running.

        -s: Starts a new simulation
        -e: Ends a running simulation
        -r: Removes particles below a certain depth
        -g: Generates new particles at a certain rate at a given position
        -m: Uses a loaded mesh
        -t: Terminates the simulation after a given time

- render [-o path|-d|-e]

        Renders a finished simulation. Waits for the simulation to finish if one is currently running.

        -o: The Folder to write the output to
        -d: Enable debug rendering
        -e: Stops the rendering