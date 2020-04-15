# Amstrad Learning Environment
![openai](https://github.com/wilfriedaugeard/Amstrad-Learning-Environment/blob/master/assets/openai.png)
<p align="center">
<img alt="Version : 1.0.0" src="https://img.shields.io/badge/version-1.0.0-green" target="_blank" />
<a href="https://github.com/wilfriedaugeard/Amstrad-Learning-Environment/blob/master/src/Amstrad-Learning-Environment/src/License.txt">
    <img alt="License: GNU" src="https://img.shields.io/badge/license-GNU-yellow.svg" />
</a>
</p>

Subject proposed by the [University of Bordeaux](https://u-bordeaux.fr).


### Goal
Add an Amstrad CPC emulator to the OpenAI Gym library to allow it to do reinforcement learning on games developed on this platform.



## Structure of the project

```sh
  src/
    Amstrad-Learning-Environment/
    OpenAIGymEnvs/
```

The *Amstrad-Learning-Environment* folder contains the C++ interface  and a wrapper in order to use the interface in python. 

 The *OpenAIGymEnvs* folder contains the gym environment using Amle (Amstrad Learning Environment).




## Installation
### Clone
Clone this repository to your local machine if you want to access the code.

```sh
git clone https://github.com/wilfriedaugeard/Amstrad-Learning-Environment/
```

### Install packages

Use pip/pip3 to install the different packages of the project. 

```sh
# Install Amstrad Learning Environment interface
pip install amle-py
```

```sh
# Install Gym environment
pip install gym-cap32
```

For details: [AMLE](https://github.com/wilfriedaugeard/Amstrad-Learning-Environment/blob/master/src/Amstrad-Learning-Environment/src/README.md)



## Authors

👤 **Wilfried Augeard**
- Github: [@Exyos](https://github.com/wilfriedaugeard)
- Website: [waugeard.com](https://waugeard.com)

👤 **Theo De Castro Pinto**
- Github : [@Leopounet](https://github.com/Leopounet)

## License

- [Gnu license](https://github.com/wilfriedaugeard/Amstrad-Learning-Environment/blob/master/src/Amstrad-Learning-Environment/src/License.txt)<br/>
