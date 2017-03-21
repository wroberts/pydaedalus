=================
 Packaging Notes
=================

python setup.py test
python setup.py sdist
python setup.py bdist_wheel
python3.4 setup.py bdist_wheel
python3.6 setup.py bdist_wheel
twine upload dist/*
