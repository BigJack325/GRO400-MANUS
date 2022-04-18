FROM ubuntu

RUN apt-get update
RUN DEBIAN_FRONTEND=noninteractive TZ=Etc/UTC apt-get -y install tzdata
RUN DEBIAN_FRONTEND=noninteractive apt-get -y install keyboard-configuration

# RUN apt-get install -y xserver-xorg xinit
# RUN apt-get install -y \
#     mesa-utils \
#     libegl1-mesa \
#     libegl1-mesa-dev \
#     libgbm-dev \
#     libgbm1 \
#     libgl1-mesa-dev \
#     libgl1-mesa-dri \
#     libgl1-mesa-glx \
#     libglu1-mesa \
#     libglu1-mesa-dev
RUN apt-get install -y --no-install-recommends apt-utils

RUN apt install -y \
        libxcb-xinerama0 \
        libxcb-image0 \
        libxcb-icccm4 \
        libxcb-keysyms1 \
        libxcb-render-util0 \
        libxcb-xkb1 \
        libxkbcommon-x11-0
        
# RUN apt-get install ffmpeg libsm6 libxext6  -y
    
# RUN Xvfb :1 -screen 0 1024x768x16 &
    
ENV QT_DEBUG_PLUGINS=1
ENV QT_QPA_PLATFORM=xcb
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
