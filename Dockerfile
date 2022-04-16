FROM ubuntu:18.04

RUN apt-get update
RUN apt-get install python3 -y
RUN apt-get install python3-pip -y

RUN mkdir -p /home/Qt
COPY Hexapod_Qt /home/Qt/Hexapod_Qt
WORKDIR /home/Qt/Hexapod_Qt
RUN ./install_venv_pi.sh
