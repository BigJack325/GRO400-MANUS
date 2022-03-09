python3.8 -m venv Qt_venv
source "Qt_venv/bin/activate"
pip3 install -r "requirements.txt"
python3 -m ipykernel install --user --name=Qt_venv
