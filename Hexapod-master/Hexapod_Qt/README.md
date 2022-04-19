
## Installation on Raspberry-Pi:

<b>Step 1.</b> Activate Raspberry-Pi VNC and  legacy camera using <b>sudo raspy-config</b>  in the interface options selection
<br/>
<b>Step 2.</b> Install VSCode using
<pre>
sudo apt-get code
</pre>
<b>Step 3.</b> Execute the <b>./install_venv_pi.sh</b> in terminal in <b>Hexapod-master/Hexapod_Qt path</b>
<pre>
cd ./Hexapod-Master/Hexapod_Qt
./install_venv_pi.sh
</pre> 
<b>WARNING: PyQt5 installation takes up to 1-2h to compile metadata</b><br>
<br>
<b>Step 4.</b> Enter the virtual environment using
<pre>
source Qt_venv/bin/activate
</pre> 
NOTE: Interpreter in VSCode should automatically change to the virtual environnement

## Installation for TensorFlow(NVIDIA GPU strongly advised):

<b>Step 1.</b> Download [Anaconda](https://www.anaconda.com/products/distribution) and the [Repo](https://github.com/EDP325/GRO400-MANUS.git) on your computer, <b> not Raspberry Pi </b>, then open a terminal. You should see a virtual environnement (base) in the terminal.<br>
![Screen Shot 2022-04-17 at 11 45 03 PM](https://user-images.githubusercontent.com/72100463/163751224-2b0fb61f-1e87-4eee-8caf-9cedb23b2161.png)<br>
When this environnement is active, you can activate the jupyter notebook using
<pre>
jupiter notebook
</pre>
To deactivate the base environnement, use
<pre>
deactivate
</pre>
<b>Step 2.</b> Execute the <b>./install_venv.sh</b> in terminal in <b>Hexapod-master/Hexapod_Qt path</b>
<pre>
cd ./Hexapod-Master/Hexapod_Qt
./install_venv.sh
</pre>
<b>WARNING: BE SHURE TO ALWAYS BE IN VIRTUAL ENVIRONNEMENT USING</b>
<pre>
source Qt_venv/bin/activate
</pre>
<b>INSIDE THE Hexapod-master/Hexapod_Qt PATH </b>
<br>
<br>
Your terminal should look like this before proceeding<br>
![Screen Shot 2022-04-17 at 11 53 23 PM](https://user-images.githubusercontent.com/72100463/163751914-dbc2be7d-5153-4293-a687-d18ea624f408.png)

<b>Step 3.</b> Follow this [Github Repo](https://github.com/nicknochnack/TFODCourse) or this [Youtube Video](https://www.youtube.com/watch?v=yqkISICHH-U&t=11796s).<br>
NOTE: Do not use mac computer as they do not have NVIDIA GPU.CPU runtime to teach the TensorFlow can be very long and often do not give good results. When using the 2. Training and Detection, there is code to test if TensorFlow is dectecting NVIDIA GPU. If the code returns False, try updating the GPU to nvidia-470 and look up your GPU driver to see if installed.
