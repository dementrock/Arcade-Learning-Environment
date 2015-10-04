#ifndef __ALE_C_WRAPPER_H__
#define __ALE_C_WRAPPER_H__

#include <ale_interface.hpp>

extern "C" {
  // Declares int rgb_palette[256]
  #include "atari_ntsc_rgb_palette.h"
  ALEInterface *ALE_new() {return new ALEInterface();}
  void ALE_del(ALEInterface *ale){delete ale;}
  const char *getString(ALEInterface *ale, const char *key){return ale->getString(key).c_str();}
  int getInt(ALEInterface *ale,const char *key) {return ale->getInt(key);}
  bool getBool(ALEInterface *ale,const char *key){return ale->getBool(key);}
  float getFloat(ALEInterface *ale,const char *key){return ale->getFloat(key);}
  void setString(ALEInterface *ale,const char *key,const char *value){ale->setString(key,value);}
  void setInt(ALEInterface *ale,const char *key,int value){ale->setInt(key,value);}
  void setBool(ALEInterface *ale,const char *key,bool value){ale->setBool(key,value);}
  void setFloat(ALEInterface *ale,const char *key,float value){ale->setFloat(key,value);}
  void loadROM(ALEInterface *ale,const char *rom_file){ale->loadROM(rom_file);}
  int act(ALEInterface *ale,int action){return ale->act((Action)action);}
  bool game_over(ALEInterface *ale){return ale->game_over();}
  void reset_game(ALEInterface *ale){ale->reset_game();}
  void getLegalActionSet(ALEInterface *ale,int *actions){
    ActionVect action_vect = ale->getLegalActionSet();
    for(unsigned int i = 0;i < ale->getLegalActionSet().size();i++){
      actions[i] = action_vect[i];
    }
  }
  int getLegalActionSize(ALEInterface *ale){return ale->getLegalActionSet().size();}
  void getMinimalActionSet(ALEInterface *ale,int *actions){
    ActionVect action_vect = ale->getMinimalActionSet();
    for(unsigned int i = 0;i < ale->getMinimalActionSet().size();i++){
      actions[i] = action_vect[i];
    }
  }
  int getMinimalActionSize(ALEInterface *ale){return ale->getMinimalActionSet().size();}
  int getFrameNumber(ALEInterface *ale){return ale->getFrameNumber();}
  int lives(ALEInterface *ale){return ale->lives();}
  int getEpisodeFrameNumber(ALEInterface *ale){return ale->getEpisodeFrameNumber();}
  void getScreen(ALEInterface *ale,unsigned char *screen_data){
    int w = ale->getScreen().width();
    int h = ale->getScreen().height();
    pixel_t *ale_screen_data = (pixel_t *)ale->getScreen().getArray();
    memcpy(screen_data,ale_screen_data,w*h*sizeof(pixel_t));
  }
  void getRAM(ALEInterface *ale,unsigned char *ram){
    unsigned char *ale_ram = ale->getRAM().array();
    int size = ale->getRAM().size();
    memcpy(ram,ale_ram,size*sizeof(unsigned char));
  }

  int getRAMSize(ALEInterface *ale){return ale->getRAM().size();}
  int getScreenWidth(ALEInterface *ale){return ale->getScreen().width();}
  int getScreenHeight(ALEInterface *ale){return ale->getScreen().height();}

  void getScreenRGB(ALEInterface *ale, int *output_buffer){
    size_t w = ale->getScreen().width();
    size_t h = ale->getScreen().height();
    size_t screen_size = w*h;
    pixel_t *ale_screen_data = ale->getScreen().getArray();

    for(int i = 0;i < w*h;i++){
        output_buffer[i] = rgb_palette[ale_screen_data[i]];
    }

  }

  void getScreenGrayscale(ALEInterface *ale, unsigned char *output_buffer){
    size_t w = ale->getScreen().width();
    size_t h = ale->getScreen().height();
    size_t screen_size = w*h;
    pixel_t *ale_screen_data = ale->getScreen().getArray();

    ale->theOSystem->colourPalette().applyPaletteGrayscale(output_buffer, ale_screen_data, screen_size);
  }

  void saveState(ALEInterface *ale){ale->saveState();}
  void loadState(ALEInterface *ale){ale->loadState();}
  void saveScreenPNG(ALEInterface *ale,const char *filename){ale->saveScreenPNG(filename);}

  ALEState* cloneState(ALEInterface *ale) {
    return new ALEState(ale->cloneState());
  }

  void ALEState_del(ALEState* state) {
    delete state;
  }

  void restoreState(ALEInterface *ale, ALEState* state) {
    ale->restoreState(*state);
  }

  int ALEState_getFrameNumber(ALEState* state) {
    return state->getFrameNumber();
  }

  int ALEState_getEpisodeFrameNumber(ALEState* state) {
    return state->getEpisodeFrameNumber();
  }

  int ALEState_getLeftPaddle(ALEState* state) {
    return state->getLeftPaddle();
  }

  int ALEState_getRightPaddle(ALEState* state) {
    return state->getRightPaddle();
  }

  struct ser_string {
    unsigned int size;
    char* data;
  };

  void delSerString(char* data) {
    delete [] data;
  }

  ser_string ALEState_getSerializedState(ALEState* state) {
    std::string ser = state->getSerializedState();
    char* cpy = new char[ser.length()];
    memcpy(cpy, ser.c_str(), ser.length());
    ser_string ret;
    ret.data = cpy;
    ret.size = ser.length();
    return ret;
  }

  bool ALEState_equals(ALEState* a, ALEState *b) {
    return a->equals(*b);
  }

  void setLoggerLevelInfo() {
    ale::Logger::setMode(ale::Logger::Info);
  }

  void setLoggerLevelWarning() {
    ale::Logger::setMode(ale::Logger::Warning);
  }

  void setLoggerLevelError() {
    ale::Logger::setMode(ale::Logger::Error);
  }

  void loadSerialized(ALEInterface *ale,
      int left_paddle, int right_paddle, int frame_number,
      int episode_frame_number, int n_ser_bytes, char* ser_bytes) {

    std::string ser_string(ser_bytes, n_ser_bytes);
    
    ALEState state(
        left_paddle, right_paddle, frame_number, episode_frame_number,
        ser_string);
    ale->restoreState(state);
  }


}

#endif
