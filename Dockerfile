FROM ubuntu

RUN apt-get update
RUN apt-get install python3 -y
RUN apt-get install python3-pip -y
RUN apt-get install python3-venv -y

RUN mkdir -p /home/Qt
COPY Hexapod-master/Hexapod_Qt /home/Qt/Hexapod-master/Hexapod_Qt
WORKDIR /home/Qt/Hexapod-master/Hexapod_Qt
RUN chmod u+x install_venv.sh
RUN ./install_venv.sh

ENV VIRTUAL_ENV=/Qt_venv
ENV PATH=”$VIRTUAL_ENV/bin:$PATH”

# Run the application:
COPY MANUS_QT.py .
CMD [“python3”, “myapp.py”]
