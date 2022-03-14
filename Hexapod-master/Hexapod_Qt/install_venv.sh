python3 -m venv Qt_venv
source "Qt_venv/bin/activate"
pip install --upgrade pip
pip install -r "requirements.txt"
python3 -m ipykernel install --user --name=Qt_venv
