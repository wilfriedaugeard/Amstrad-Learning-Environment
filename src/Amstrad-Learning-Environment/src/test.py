import sys
import numpy as np
import random

sys.path.insert(0, "amle_py/")

import amle_python_interface as api 

amle = api.AmLEInterface()
games = amle.getSupportedGames()
for i in games:
    print(i)
amle.toggleSound(False)
amle.loadSnapshot("Arkanoid", b"snap/arka.sna")

nbLegalActions = amle.getNbLegalActions()
legalActions = np.empty(nbLegalActions, dtype=np.int32)
amle.getLegalActions(legalActions)
legalActions = legalActions.tolist()

# width = amle.getScreenWidth()
# height = amle.getScreenHeight()

# for i in range(1000000):
#     amle.step()
    
# screen = np.empty(width * height, dtype=np.uint32)
# amle.getScreen(screen)
# screen = screen.tolist()

# for i in range(height):
#     for j in range(width):
#         pass
#         print(screen[i * width + j], end=" ")
#     print("")

for i in range(100000):
    # print("Number of lives = " + str(amle.getNbLives()))
    amle.act(random.choice(legalActions))
    amle.step()

amle.resetGame()

amle.pokeMemory(0xb7d4, 6)
amle.setEmulatorSpeed(32)

while(True):
    amle.act(random.choice(legalActions))
    amle.step()