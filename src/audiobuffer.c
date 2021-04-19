#include "audiobuffer.h"
#include <string.h>
#include <strings.h>

struct audiobuffer_t audiobuffer_allocate(uint32_t frames, uint32_t channels,
                                          float samplingrate)
{
  struct audiobuffer_t ret;
  bzero(&ret, sizeof(ret));
  ret.frames = frames;
  ret.channels = channels;
  ret.samplingrate = samplingrate;
  ret.calib = 1.0f;
  size_t size = ret.frames * ret.channels;
  if(!size)
    size = 1;
  ret.data = malloc(size * sizeof(int16_t));
  return ret;
}

void audiobuffer_free(struct audiobuffer_t* audio)
{
  free(audio->data);
  bzero(audio, sizeof(struct audiobuffer_t));
}

size_t audiobuffer_get_size(struct audiobuffer_t audio)
{
  return sizeof(audio.frames) + sizeof(audio.channels) +
         sizeof(audio.samplingrate) + sizeof(audio.calib) +
         audio.frames * audio.channels * sizeof(int16_t);
}

size_t audiobuffer_write(struct audiobuffer_t audio, char* dest, size_t space)
{
  size_t ret = audiobuffer_get_size(audio);
  if(space < ret)
    return 0;
  *((uint32_t*)dest) = audio.frames;
  dest += sizeof(audio.frames);
  *((uint32_t*)dest) = audio.channels;
  dest += sizeof(audio.channels);
  *((float*)dest) = audio.samplingrate;
  dest += sizeof(audio.samplingrate);
  *((float*)dest) = audio.calib;
  dest += sizeof(audio.calib);
  memcpy(dest, audio.data, sizeof(int16_t) * audio.frames * audio.channels);
  return ret;
}

int audiobuffer_read(struct audiobuffer_t* audio, char* src, size_t len)
{
  if(len != audiobuffer_get_size(*audio))
    return 1;
  uint32_t frames = *((uint32_t*)src);
  src += sizeof(frames);
  if(frames != audio->frames)
    return 2;
  uint32_t channels = *((uint32_t*)src);
  src += sizeof(channels);
  if(channels != audio->channels)
    return 3;
  float samplingrate = *((float*)src);
  src += sizeof(samplingrate);
  if(samplingrate != audio->samplingrate)
    return 4;
  audio->calib = *((float*)src);
  src += sizeof(audio->calib);
  memcpy(audio->data, src, sizeof(int16_t) * audio->frames * audio->channels);
  return 0;
}
