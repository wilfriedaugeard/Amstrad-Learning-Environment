import gym
import random
import numpy as np 


env = gym.make('gym_cap32:Arkanoid-v0')

episode_count = 100
reward = 0
done = False
# Random player
for i in range(episode_count):
    env.reset()
    while True:
        action = env.action_space.sample() # Place your model here
        ob, reward, done, _ = env.step(action)
        if done:
            break  