#pragma once

#include <iostream>
#include <fmod.hpp>
#include <fmod_errors.h>

FMOD::System* m_fmodSystem;
FMOD::Sound* m_music;
FMOD::Channel* m_musicChannel;

void ERRCHECK(FMOD_RESULT result)
{
	if (result != FMOD_OK)
	{
		printf("FMOD error! (%d) %s\n", result, FMOD_ErrorString(result));
	}
}

void initFmod()
{
    FMOD_RESULT result;
    unsigned int version;

    result = FMOD::System_Create(&m_fmodSystem);
    ERRCHECK(result);

    result = m_fmodSystem->getVersion(&version);
    ERRCHECK(result);

    if (version < FMOD_VERSION)
        printf("FMOD Error! You are using an old version of FMOD.", version, FMOD_VERSION);

    // initialize fmod system
    result = m_fmodSystem->init(32, FMOD_INIT_NORMAL, 0);
    ERRCHECK(result);

    // load and set up music
    result = m_fmodSystem->createStream("../media/1812 Overture.mp3", FMOD_SOFTWARE, 0, &m_music);
    ERRCHECK(result);

    // play the loaded mp3 music
    result = m_fmodSystem->playSound(FMOD_CHANNEL_FREE, m_music, false, &m_musicChannel);
    ERRCHECK(result);

    // set sound channel loop count
    //FMOD_Channel_SetLoopCount(m_musicChannel, 999);
    m_musicChannel->setLoopCount(999);

    // set sound volume
    m_musicChannel->setVolume(0.75f);
}