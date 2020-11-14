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
    apt-get install -y liblua5.3-dev lua5.3 libboost-filesystem-dev cmake

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
    
# Setup deployment key
RUN mkdir /root/.ssh/
ADD argos-key /root/.ssh/argos_id_rsa
ADD controller-key /root/.ssh/controller_id_rsa
RUN touch /root/.ssh/known_hosts && \ 
  ssh-keyscan github.com >> /root/.ssh/known_hosts
RUN echo "# ~/.ssh/config \nHost github-argos \nHostName github.com \nUser git \nIdentityFile ~/.ssh/argos_id_rsa \n\n Host github-controller \nHostName github.com \nUser git \nIdentityFile ~/.ssh/controller_id_rsa" >> ~/.ssh/config

# Add dummy argument to force rebuild starting from that point
ARG CODE_UPDATE=unknown

# Clone simulation
RUN cd /home/docker/ &&\
    git clone github-argos:lajoiepy/drone-rescue-simulation.git simulation &&\
    cd simulation &&\
    git submodule update --init --recursive

# Setup debug config for visual studio code
RUN mkdir /home/docker/simulation/.vscode && touch /home/docker/simulation/.vscode/launch.json &&\
    echo "{\"version\": \"0.2.0\",\"configurations\": [{\"name\": \"(gdb) Attach\", \"type\": \"cppdbg\", \"request\": \"attach\", \"program\" : \"/usr/bin/argos3\", \"processId\": \"\${command:pickProcess}\", \"MIMode\": \"gdb\", \"setupCommands\": [ {\"description\": \"Enable pretty-printing for gdb\", \"text\": \"-enable-pretty-printing\",\"ignoreFailures\": true}]}]}" \
    > /home/docker/simulation/.vscode/launch.json

# Compile C++ controller
RUN cd /home/docker/simulation/controller &&\
    mkdir build &&\
    cd build/ &&\
    cmake ../src &&\
    make &&\
    make install

# Compile buzz script
RUN cd /home/docker/simulation/argos_simulation &&\
    bzzc search_example.bzz

# Put your own git credentials to commit edited code
RUN git config --global user.email "lajoie.py@gmail.com" &&\
    git config --global user.name "Pierre-Yves Lajoie"

# Entry point to start the simulation
WORKDIR /home/docker/simulation/argos_simulation
# ENTRYPOINT argos3 -c robust_distributed_slam.argos