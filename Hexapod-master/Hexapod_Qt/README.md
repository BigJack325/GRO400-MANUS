
Installation on Raspberry-Pi:

<b>Step 1.</b> Activate Raspberry-Pi VNC and  legacy camera using <b>sudo raspy-config</b>  in the interface options selection;
<br/>
<b>Step 2.</b> Install VSCode using <b>sudo apt-get code</b>
<br/>
<b>Step 3.</b> Execute the <b>./install_venv_pi.sh</b> in terminal in Hexapod-master/Hexapod_Qt path WARNING: PyQt5 installation takes up to 1-2h to compile metadata
<pre>
cd ./Hexapod-Master/Hexapod_Qt
./install_venv_pi.sh
</pre> 
<br/>
<b>Step 4.</b> Enter the virtual environment using
<pre>
source Qt_venv/bin/activate
</pre> 
NOTE: Interpreter in VSCode should automatically change to the virtual environnement
<br/>
