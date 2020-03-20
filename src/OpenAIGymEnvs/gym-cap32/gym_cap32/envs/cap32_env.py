import gym
from gym import error, spaces, utils
from gym.utils import seeding


try:
    import amle_py
except ImportError as e:
    raise error.DependencyNotInstalled("{}. (HINT: you can install HFO dependencies with 'pip3 install gym[cap32].)'".format(e))

from amle_py import AmLEInterface


class Cap32Env(gym.Env):
  metadata = {'render.modes': ['human']}

  def __init__(self):
    return None
  def step(self, action):
    return "test step"
  def reset(self):
    return 0
  def render(self, mode='human'):
    return 0
  def close(self):
    return 0