FROM ubuntu:18.04
LABEL Maintainer="Pierre-Yves Lajoie <lajoie.py@gmail.com>"
LABEL drone-rescue-simulation.version="0.1"

# Install common dependencies
RUN apt-get update && apt-get install -y \
    build-essential \
    dpkg \ 
    git \
    pkg-config \
    python \
    python-dev \
    python-numpy \
    software-properties-common \
    && rm -rf /var/lib/apt/lists/*

# Install ARGoS
RUN apt-get update && apt-get install -y \
    wget \
    freeglut3-dev \
    qt5-default \
    libxi-dev \
    libxmu-dev \
    libfreeimage-dev \
    libfreeimageplus-dev \
    liblua5.2-dev \
    lua5.2 \
    liblua5.3-dev \
    lua5.3 \
    libboost-filesystem-dev \
    cmake \
    && rm -rf /var/lib/apt/lists/*
    
RUN apt-get update && \
    apt-get -y install sudo
RUN adduser --disabled-password --gecos '' docker
RUN adduser docker sudo
RUN echo '%sudo ALL=(ALL) NOPASSWD:ALL' >> /etc/sudoers
USER docker
#RUN sudo dpkg -i argos3_simulator-3.0.0-x86_64-beta56.deb
RUN sudo chmod -R ugo+rw /home/docker
USER root

# Install and setup some editing and debugging tools
RUN sudo apt-get install -y gedit nano strace valgrind

RUN sudo wget -O vscode.deb https://go.microsoft.com/fwlink/?LinkID=760868 &&\
    sudo apt install -y ./vscode.deb &&\
    sudo apt install -y libasound2

RUN mkdir /home/docker/code &&\
    code --install-extension ms-vscode.cpptools --user-data-dir /home/docker/code

# Settings for buzz editing
RUN touch /home/docker/code/User/settings.json &&\
    echo "{ \"files.associations\": { \"*.bzz\":\"python\" } }" > /home/docker/code/User/settings.json
    
RUN apt-get update && DEBIAN_FRONTEND="noninteractive" apt-get -y install tzdata
RUN apt-get update && apt-get install -y \
    graphviz \
    graphviz-dev \
    asciidoc \
    && rm -rf /var/lib/apt/lists/*

# Add dummy argument to force rebuild starting from that point
ARG CODE_UPDATE_ARGOS=unknown

# Install Argos
RUN cd /home/docker/ &&\
    git clone https://github.com/lajoiepy/argos3.git &&\
    cd argos3 &&\
    git checkout spiri_downward_facing_camera &&\
    mkdir build_simulator &&\
    cd build_simulator &&\
    cmake ../src -DCMAKE_BUILD_TYPE=Debug \
     -DARGOS_BUILD_FOR=simulator \
     -DARGOS_THREADSAFE_LOG=ON \
     -DARGOS_DYNAMIC_LOADING=ON &&\
    make

RUN touch /home/docker/argos3/build_simulator/argos3.1.gz &&\
    touch /home/docker/argos3/build_simulator/README.html &&\
    cd /home/docker/argos3/build_simulator &&\
    make install

RUN chmod +x /home/docker/argos3/build_simulator/argos_post_install.sh &&\
    ./home/docker/argos3/build_simulator/argos_post_install.sh &&\
    echo "\nsource /home/docker/argos3/build_simulator/setup_env.sh\n" >> /.bashrc


# Add dummy argument to force rebuild starting from that point
ARG CODE_UPDATE=unknown

# Install Buzz
RUN git clone https://github.com/lajoiepy/Buzz.git /home/docker/buzz
RUN cd /home/docker/buzz &&\
    mkdir build &&\
    cd build &&\
    cmake -DCMAKE_BUILD_TYPE=Debug ../src &&\
    make &&\
    make install &&\
    ldconfig

# Clone simulation
RUN cd /home/docker/ &&\
    git clone https://github.com/ulrichdah/drone-rescue.git

# Setup debug config for visual studio code
RUN mkdir /home/docker/drone-rescue/sim/.vscode && touch /home/docker/drone-rescue/sim/.vscode/launch.json &&\
    echo "{\"version\": \"0.2.0\",\"configurations\": [{\"name\": \"(gdb) Attach\", \"type\": \"cppdbg\", \"request\": \"attach\", \"program\" : \"/usr/local/bin/argos3\", \"processId\": \"\${command:pickProcess}\", \"MIMode\": \"gdb\", \"setupCommands\": [ {\"description\": \"Enable pretty-printing for gdb\", \"text\": \"-enable-pretty-printing\",\"ignoreFailures\": true}]}]}" \
    > /home/docker/drone-rescue/sim/.vscode/launch.json

# Compile C++ controller
RUN cd /home/docker/drone-rescue/sim/controller &&\
    mkdir build &&\
    cd build/ &&\
    cmake ../src &&\
    make &&\
    make install

# Compile buzz script
RUN cd /home/docker/drone-rescue/sim/argos_simulation &&\
    bzzc search_example.bzz &&\
    bzzc target_drift_example.bzz

# Put your own git credentials to commit edited code
RUN git config --global user.email "ulrich.dah-achinanon@polymtl.ca" &&\
    git config --global user.name "Ulrich Dah-Achinanon"

# Entry point to start the simulation
WORKDIR /home/docker/drone-rescue/sim/argos_simulation
# ENTRYPOINT argos3 -c robust_distributed_slam.argos
