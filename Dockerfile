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
        
RUN apt-get install -y '^libxcb.*-dev' libx11-xcb-dev libglu1-mesa-dev libxrender-dev libxi-dev libxkbcommon-dev libxkbcommon-x11-dev 

        
# RUN apt install -y libgl1-mesa-glx 
# RUN apt install -y libgl1
    
ENV QT_DEBUG_PLUGINS=1
# ENV QT_QPA_PLATFORM=xcb
# ENV QT_QPA_PLATFORM_PLUGIN_PATH=/opt/Qt/${QT_VERSION}/gcc_64/plugins
# ENV QT_PLUGIN_PATH=/opt/Qt/${QT_VERSION}/gcc_64/plugins
# ENV DISPLAY=:1

RUN apt-get install python3 -y
RUN apt-get install python3-pip -y
RUN apt-get install python3-venv -y
RUN apt-get install libqt5serialport5 -y
# RUN apt-get install qt5-default -y
# RUN apt-get install build-essential -y
# RUN apt-get install mesa-common-dev -y

RUN mkdir -p /home/Qt
COPY Hexapod-master/Hexapod_Qt /home/Qt/Hexapod-master/Hexapod_Qt
WORKDIR /home/Qt/Hexapod-master/Hexapod_Qt

# ENV VIRTUAL_ENV=/opt/venv
# RUN python3 -m venv $VIRTUAL_ENV
# ENV PATH="$VIRTUAL_ENV/bin:$PATH"
RUN pip3 install --upgrade PyQt5

# Install dependencies:
RUN pip3 install -r "pip_requirements.txt"


# Run the application:
RUN python3 MANUS_QT.py
