# Check images installed locally
docker images

# Search for available images in Docker Hub
docker search ubuntu

# Download docker ubuntu image
docker pull ubuntu

# Verify user running commands is root
docker run ubuntu whoami

# Open shell session in container
docker run -it ubuntu /bin/bash

# Show containers running a command right now
docker ps

# Show all containers (even container whose command has already run)
docker ps -a

# After exiting the terminal session the container is REMOVED so when running
# `docker ps -a` the container doesn't appear
docker run -it --rm ubuntu /bin/bash

# Delete all docker containers
docker ps -a | awk 'NR > 1 { print $1 }' | xargs docker rm
docker rm $(docker ps -aq)

# Mounting current working directory
docker run -it --rm -v `pwd`:/docker ubuntu /bin/bash

# Use this option to login on container with a different user
# -u, --user string     Username or UID (format: <name|uid>[:<group|gid>])

# Create image from Dockerfile
docker build -t 'cdev:v1' .

# Delete docker images <none>:<none>
docker rmi $(docker images -q -f "dangling=true")

# Compile on Ubuntu with gcc
docker run --name ubuntu_cdev -ti --rm -e CC=gcc -v $(pwd):/docker cdev:v1 make server

# Creates a file in a running docker container
docker exec -d ubuntu_bash touch /tmp/execWorks
