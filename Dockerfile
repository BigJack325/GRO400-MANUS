FROM ubuntu

RUN apt-get update
RUN DEBIAN_FRONTEND=noninteractive TZ=Etc/UTC apt-get -y install tzdata
RUN DEBIAN_FRONTEND=noninteractive apt-get -y install keyboard-configuration


RUN apt-get install python3 -y
RUN apt-get install python3-pip -y
RUN apt-get install python3-venv -y
RUN apt-get install libqt5serialport5 -y
RUN apt install qtcreator

RUN mkdir -p /home/Qt
COPY Hexapod-master/Hexapod_Qt /home/Qt/Hexapod-master/Hexapod_Qt
WORKDIR /home/Qt/Hexapod-master/Hexapod_Qt

# ENV VIRTUAL_ENV=/opt/venv
# RUN python3 -m venv $VIRTUAL_ENV
# ENV PATH="$VIRTUAL_ENV/bin:$PATH"
RUN pip3 install --upgrade PyQt5
RUN export QT_DEBUG_PLUGINS=1
RUN qtcreator


# Install dependencies:
RUN pip3 install -r "pip_requirements.txt"


# Run the application:
RUN python3 MANUS_QT.py
