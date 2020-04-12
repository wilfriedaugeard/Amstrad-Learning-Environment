# Amstrad Learning Environment

The Amstrad Learning Environment is a Python interface that is meant to be used with AIs and OpenAi Gym. The Python library is
available on Linux and Windows.

## Installation

### Unix

First you'll need to install Python3 (version >= 3.6). To install Python : 

```bash
sudo apt-get install python3
```

To check the version :

```bash
python3 --version
```

Then, you'll need to install pip, so you can install the amle-py package :

```bash
curl https://bootstrap.pypa.io/get-pip.py | python
```

With pip, you'll need to install two dependencies :

```bash
pip install --upgrade setuptools
pip install --upgrade numpy
```

Finally you can install amle-py :

```bash
pip install --upgrade amle-py
```

### Windows 

Install Python by going on their [website](https://www.python.org/downloads/). Make sure to add Python to your path and install pip during the installation.

## How to use

### Compile and use the sources (Linux only)

To compile the sources you will need to install a few dependencies :

- SDL 1.2
- libpng
- libzip
- cppunit

Once you have all these dependencies, you can compile the project. You may also want to generate the documentation of the project, this is done in the doc/ folder. Make sure to have doxygen installed :

```bash
sudo apt-get install doxygen
```

And then : 

```bash
doxygen doxygen.confuig
```

### Use the Python library

First you will need to create a new Python file in which you import the library :

```python
import amle_py
```

Then you will need to create a new amle object :

```python
amle = amle_py.AmLEInterface()
```

And you will need to load a game :

```python
amle.loadSnapshot("Arkanoid", "snap/arka.sna")
```

Note that the first string has to a name the AmLE can understand. If you have a doubt you can get the list of all possible strings with : 

```python
games = amle.getSupportedGames()
```

Moreover, the second argument is a path to YOUR .sna file. You have to import one from the internet and generate one yourself with an emulator. Also, it doesn't have to be in a snap/ folder, this is just cleaner.

Then you may want to run the game : 

```python
while not(amle.gameOver()):
    amle.step()
```

This doesn't do anything interesting though, you can also interact with the game. To do so, before the loop you can do : 

```python
nbLegalActions = amle.getNbLegalActions()
legalActions = np.empty(nbLegalActions, dtype=np.int32)
amle.getLegalActions(legalActions)
legalActions = legalActions.tolist()
```

And then in the loop :

```python
import amle_py
import random
...
...
while not(amle.gameOver()):
    amle.act(random.choice(legalActions))
    amle.step()
```

Finally, you may want to generate the documentation for the library. To do so go in the amle_py folder and run :

```bash
pydoc -w amle_python_interface.py
```