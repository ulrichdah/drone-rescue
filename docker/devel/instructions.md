# Acces to GUI
The ARGoS simulation needs access to a graphical user interface to work. Many options are available to have access to the GUI of the apps running in your docker container.
# Build the image
You need to download the ARGoS simulator from https://www.argos-sim.info/core.php, place it in this folder and update the version number in the docker file.

For the first build execute: 
```
docker build . --tag drones-search-and-rescue --network host
```
If you modified the code from this repository or its dependencies (https://github.com/lajoiepy/Buzz) and you want to use the latest version, use the following command: 
```
docker build . --tag drones-search-and-rescue --network host --build-arg CODE_UPDATE=<dummy-arg>
```
Be sure to change the `CODE_UPDATE` value every time to invalidate your build cache. If you modified the ARGoS simulator itself use the arg `CODE_UPDATE_ARGOS` instead.

# To run a container
The ARGoS simulation needs access to a graphical user interface to work. Many options are available to have access to the GUI of the apps running in your docker container. I propose using https://github.com/mviereck/x11docker, but feel free to use another one. 
You can install `x11docker` with the following command:
```
curl -fsSL https://raw.githubusercontent.com/mviereck/x11docker/master/x11docker | sudo bash -s -- --update
```
If you installed `x11docker`, use the following command while specifying a folder on your host machine to store the log files from the simulation.
```
sudo x11docker --hostdisplay --hostnet --user=RETAIN -- --privileged -v <log-folder-on-host-computer>:/home/docker/simulation/argos_simulation/log -- lajoiepy/drones-search-and-rescue:latest
```

# Launching the simulation
If the correct `ENTRYPOINT` is specified in the Dockerfile, the simulation should start with the previous command. \
Otherwise, you can start a bash shell into the container with the following command: 
```
docker exec -it $(docker container ls -q) /bin/bash
```
If you have multiple containers running at the same time, you can replace the `$(docker container ls -q)` with the container ID. The container ID can be found using `docker ps`. 

Then you can start the simulation yourself by going into the folder `/home/docker/simulation/argos_simulation`
and executing :
```
argos3 -c search_example.argos
```

# To debug with Visual Studio Code
Some editing and debugging tools are installed in the image for development purposes (gedit, nano, strace, valgrind, and Visual Studio Code).
To debug the code from this repository:
1. Open Visual Studio Code in the folder `/home/docker/simulation` with the `--user-data-dir /home/docker/code/` argument.
2. Launch the simulation as explained previously.
3. Start the `(gdb) Attach` configuration in the Visual Studio Code debugging tool and select the running `argos3` process.
4. You are ready to place your breakpoints and debug!

# Questions?
For further questions or comments, feel free to contact me at : `lajoie.py@gmail.com`.