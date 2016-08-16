#ifndef _SOUND_HPP_
#define _SOUND_HPP_
#include "fmod/inc/fmod.hpp"
#include "fmod/inc/fmod_errors.h"
#if defined(_WIN64)
#pragma comment(lib, "fmod/lib/fmodex64_vc.lib")
#elif defined(_WIN32)
#pragma comment(lib, "fmod/lib/fmodexL_vc.lib")
#endif
#include <map>
#include <string>
using namespace FMOD;
#ifdef _UNICODE
#define _TEXT_STR(str) L##str
#else
#define _TEXT_STR(str) str
#endif
#define FMOD_BGM    true
#define FMOD_EFFECT false
class SoundDevice{
private:
System* _system; 
FMOD_RESULT _result;
float _volume;
#ifdef _UNICODE
std::map<std::wstring, Sound*> _sound;
std::map<std::wstring, Channel*> _channel;
#else
std::map<std::string, Sound*> _sound;
std::map<std::string, Channel*> _channel;
#endif
private:
bool _errcheck(FMOD_RESULT result){
if(result!=FMOD_OK){
#ifdef _WINDOW
TCHAR str[256];
wsprintf(str, _TEXT_STR("FMOD error! (%d) %d\n"), result, FMOD_ErrorString(result));
MessageBox(0, str, _TEXT_STR("FMOD Error"), MB_OK);
#endif
return false;}
return true;}
void _volumeset(){
#ifdef _UNICODE
std::map<std::wstring,Channel*>::iterator citer=_channel.begin();
#else
std::map<std::string,Channel*>::iterator citer=_channel.begin();
#endif
bool _isplay;
for(;citer!=_channel.end();++citer){
_errcheck(citer->second->isPlaying(&_isplay));
if(!_isplay){
_errcheck(citer->second->setPaused(true));
_errcheck(citer->second->setVolume(_volume));
_errcheck(citer->second->setPaused(false));
}else _errcheck(citer->second->setVolume(_volume));}}
public:
void SetVolume(float __volume = 1.0f /* 0.0 ~ 1.0 */){
if(__volume<(float)0)_volume=0;
else if(__volume>(float)1)_volume=1;
else _volume=__volume;
_volumeset();}
void SetVolume(int __volume = 100 /* 0 ~ 100 */){
if(__volume<0)_volume=0;
else if(__volume>100)_volume=1.0f;
else _volume=(float)__volume/100.f;
_volumeset();}
bool Init(const int& _channelcnt /* Number of channel*/){
_result=System_Create(&_system);
if(!_errcheck(_result))return false;
_result=_system->init(_channelcnt,FMOD_INIT_NORMAL,0);
if(!_errcheck(_result))return false;
return true;}
#ifdef _UNICODE
bool LoadSound(std::wstring _key, std::wstring _path, bool IsBgm = FMOD_EFFECT){
#else
bool LoadSound(std::string _key, std::string _path, bool IsBgm = FMOD_EFFECT){
#endif
Sound* pSound;
if(IsBgm)_result=_system->createStream((const char*)_path.data(),FMOD_2D|FMOD_HARDWARE|FMOD_LOOP_NORMAL|FMOD_UNICODE,0,&pSound);
else _result=_system->createSound((const char*)_path.data(),FMOD_2D|FMOD_HARDWARE|FMOD_LOOP_OFF|FMOD_UNICODE,0,&pSound);
if(!_errcheck(_result))return false;
_sound.insert(std::make_pair(_key,pSound));
if(IsBgm){
Channel* pChannel;
_errcheck(_system->playSound(FMOD_CHANNEL_FREE,pSound,true,&pChannel));
_errcheck(pChannel->setVolume(_volume));
_channel.insert(std::make_pair(_key,pChannel));}
return true;}
#ifdef _UNICODE
void Play(std::wstring _key){
std::map<std::wstring, Sound*>::iterator siter=_sound.find(_key);std::map<std::wstring, Channel*>::iterator citer=_channel.find(_key);
#else
void Play(std::string _key){
std::map<std::string, Sound*>::iterator siter=_sound.find(_key);std::map<std::string, Channel*>::iterator citer=_channel.find(_key);
#endif
if(citer==_channel.end()){
if(siter==_sound.end())return;
else{
Channel* pChannel;
_errcheck(_system->playSound(FMOD_CHANNEL_FREE,siter->second,true,&pChannel));
_errcheck(pChannel->setVolume(_volume));
_errcheck(pChannel->setPaused(false));}}
else{
_errcheck(citer->second->setVolume(_volume));
_errcheck(citer->second->setPaused(false));}}
#ifdef _UNICODE
void Pause(std::wstring _key){
std::map<std::wstring, Channel*>::iterator citer=_channel.find(_key);
#else
void Pause(std::string _key){
std::map<std::string, Channel*>::iterator citer=_channel.find(_key);
#endif
if(citer==_channel.end())return;
_errcheck(citer->second->setPaused(true));}
#ifdef _UNICODE
void Stop(std::wstring _key){
std::map<std::wstring, Channel*>::iterator citer=_channel.find(_key);
#else
void Stop(std::string _key){
std::map<std::string, Channel*>::iterator citer=_channel.find(_key);
#endif
if(citer==_channel.end())return;
_errcheck(citer->second->setPosition(0, FMOD_TIMEUNIT_MS));
_errcheck(citer->second->setPaused(true));}
void Update(){_errcheck(_system->update());}
void Close(){
SoundDevice** _pInstance=_Instance();
if(*_pInstance){delete *_pInstance;*_pInstance=0;}}
private:
static SoundDevice** _Instance(){
static SoundDevice* _pInstance=0;
if(!_pInstance)_pInstance=new SoundDevice;return &_pInstance;}
friend class GameSound;
void _release(){
#ifdef _UNICODE
std::map<std::wstring, Sound*>::iterator siter;
#else
std::map<std::string, Sound*>::iterator siter;
#endif
for(siter=_sound.begin();siter!=_sound.end();++siter){
_errcheck(siter->second->release());}
_sound.clear();_channel.clear();
_errcheck(_system->close());}
SoundDevice():_volume(1.0f){}~SoundDevice(){_release();}};
class GameSound {public:SoundDevice* operator->(){return *SoundDevice::_Instance();}};
#endif