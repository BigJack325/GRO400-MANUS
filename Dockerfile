FROM ubuntu

RUN apt-get update
RUN DEBIAN_FRONTEND=noninteractive TZ=Etc/UTC apt-get -y install tzdata
RUN DEBIAN_FRONTEND=noninteractive apt-get -y install keyboard-configuration

RUN apt-get install -y --no-install-recommends apt-utils

RUN apt-get install ffmpeg libsm6 libxext6  -y

RUN apt install -y \
        libxcb-xinerama0 \
        libxcb-image0 \
        libxcb-icccm4 \
        libxcb-keysyms1 \
        libxcb-render-util0 \
        libxcb-xkb1 \
        libfontconfig1 \
        libxcb-shape0 \
        libdbus-1-3 \
        libxkbcommon-x11-0

    
ENV QT_DEBUG_PLUGINS=1

RUN apt-get install python3 -y
RUN apt-get install python3-pip -y
RUN apt-get install python3-venv -y
RUN apt-get install libqt5serialport5 -y

RUN mkdir -p /home/Qt
COPY Hexapod-master/Hexapod_Qt /home/Qt/Hexapod-master/Hexapod_Qt
WORKDIR /home/Qt/Hexapod-master/Hexapod_Qt

RUN pip3 install --upgrade PyQt5

# Install dependencies:
RUN pip3 install -r "pip_requirements.txt"


# Run the application:
RUN python3 MANUS_QT.py
