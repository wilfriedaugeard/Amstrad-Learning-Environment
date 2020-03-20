from gym.envs.registration import register


# List of available games 
for game in ['arkanoid', 'buggy']:

    for obs_type in ['image', 'ram']:
         # space_invaders should yield SpaceInvaders-v0 and SpaceInvaders-ram-v0 
        name = ''.join([g.capitalize() for g in game.split('_')])
        
        if obs_type == 'ram':
            name = '{}-ram'.format(name)

        register(
            id='{}-v0'.format(name),
            entry_point='gym_cap32.envs:Cap32Env',
        )
       