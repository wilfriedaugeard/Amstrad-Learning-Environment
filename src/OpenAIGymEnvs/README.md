# OpenAI Gym Envs

[Link to doc](https://github.com/openai/gym/blob/master/docs/creating-environments.md)


## Structure

```sh
  gym-cap32/
    README.md
    test.py
    setup.py
    gym_cap32/
      __init__.py
      envs/
        __init__.py
        cap32_env.py
        cap32_env_extrahard_env.py
  ```


## Installation of Gym
```sh
pip3 install gym
```

## Test

```sh
  # In the root directory
  pip3 install -e .
  python3 test.py
```


