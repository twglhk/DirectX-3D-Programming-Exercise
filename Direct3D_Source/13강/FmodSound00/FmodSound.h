#pragma once
#include <fmod.h>
#include <string>
using namespace std;

class CFmodSound
{
private:
	FMOD_SYSTEM* m_pSystem;
	FMOD_SOUND** m_ppBGSound;
	FMOD_SOUND** m_ppEFFSound;
	FMOD_CHANNEL** m_ppBGChannel;
	int m_nEFSoundCount;
	int m_nBGSoundCount;	
public:
	void CreateEffectSound( int nCount, string *SoundFileName );
	void CreateBGSound( int nCount, string *SoundFileName );
	void PlaySoundEffect( int nIndex );
	void PlaySoundBG( int nIndex );
	void StopSoundBG( int nIndex );
	void ReleaseSound();
	void Update();

public:
	CFmodSound(void);
	~CFmodSound(void);
};

