from gym.envs.registration import register


# If you want to add game supported by amle, place the name here
for game in ['Arkanoid','BuggyBoy']:

    name_sna = game
    
    # For the snapshot file, favor the format '<gamenamed>.sna' or specify the name like this  
    if(game == 'Arkanoid'):
        name_sna = 'arka'
    if(game == 'BuggyBoy'):
        name_sna = 'buggy'


    game_path = "snap/"+name_sna+'.sna'
    # Register Arkanoid game
    register(
        id='{}-v0'.format(game),
        entry_point='gym_cap32.envs:Cap32Env',
        kwargs={'game': game, 'game_path':game_path.encode()},
    )