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
    && rm -rf /var/lib/apt/lists/*
    
COPY argos3_simulator-3.0.0-x86_64-beta56.deb .

RUN apt-get update && \ 
    apt-get install -y liblua5.3-dev lua5.3 cmake

RUN apt-get update && \
    apt-get -y install sudo
RUN adduser --disabled-password --gecos '' docker
RUN adduser docker sudo
RUN echo '%sudo ALL=(ALL) NOPASSWD:ALL' >> /etc/sudoers
USER docker
RUN sudo dpkg -i argos3_simulator-3.0.0-x86_64-beta56.deb
RUN sudo chmod -R ugo+rw /home/docker
USER root

# Install Buzz
RUN git clone https://github.com/MISTLab/Buzz.git /home/docker/buzz
RUN cd /home/docker/buzz &&\
    mkdir build &&\
    cd build &&\
    cmake -DCMAKE_BUILD_TYPE=Debug ../src &&\
    make &&\
    make install &&\
    ldconfig

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

# Setup debug config for visual studio code
RUN mkdir /home/docker/simulation && \
    mkdir /home/docker/simulation/.vscode && touch /home/docker/simulation/.vscode/launch.json &&\
    echo "{\"version\": \"0.2.0\",\"configurations\": [{\"name\": \"(gdb) Attach\", \"type\": \"cppdbg\", \"request\": \"attach\", \"program\" : \"/usr/bin/argos3\", \"processId\": \"\${command:pickProcess}\", \"MIMode\": \"gdb\", \"setupCommands\": [ {\"description\": \"Enable pretty-printing for gdb\", \"text\": \"-enable-pretty-printing\",\"ignoreFailures\": true}]}]}" \
    > /home/docker/simulation/.vscode/launch.json
    
# Setup deployment key
RUN mkdir /root/.ssh/
ADD repo-key /root/.ssh/id_rsa
RUN touch /root/.ssh/known_hosts && \ 
  ssh-keyscan github.com >> /root/.ssh/known_hosts

# Add dummy argument to force rebuild starting from that point
ARG CODE_UPDATE=unknown

# Clone simulation
RUN cd /home/docker/simulation &&\
    git init &&\
    git remote add origin git@github.com:lajoiepy/drone-rescue-simulation.git &&\
    git pull origin master &&\
    git submodule update --init --recursive

# Compile buzz script
RUN cd /home/docker/simulation/argos_simulation 
    #&&\
    #bzzc random_walk_mapping.bzz &&\
    #bzzc dataset_mapping.bzz

# Put your own git credentials to commit edited code
RUN git config --global user.email "lajoie.py@gmail.com" &&\
    git config --global user.name "Pierre-Yves Lajoie"

# Entry point to start the simulation
WORKDIR /home/docker/simulation/argos_simulation
# ENTRYPOINT argos3 -c robust_distributed_slam.argos