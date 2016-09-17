FROM ubuntu

RUN apt-get update && apt-get install -y build-essential

RUN apt-get install -y guile-2.0 guile-2.0-dev

WORKDIR /docker
