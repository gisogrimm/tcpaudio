#ifndef AUDIOBUFFER_H
#define AUDIOBUFFER_H

#include <stdlib.h>
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

struct audiobuffer_t {
  uint32_t frames;    ///< Number of samples per block in each channel
  uint32_t channels;  ///< Number of audio channels
  float samplingrate; ///< Sampling rate in Hz
  float calib;   ///< Calibration value, a value of 2^15 corresponds to this
                 ///< sound pressure in Pa
  int16_t* data; ///< Audio data, interleaved channel order
};

/**
 * Fill members of an audio buffer
 *
 * @param frames Number of samples per block in each channel
 * @param channels Number of audio channels
 * @param samplingrate Sampling rate in Hz
 * @return A audiobuffer_t structure with data allocated for frames*channels
 * values and set to zero
 *
 * The calib value is set to 1.
 */
struct audiobuffer_t audiobuffer_allocate(uint32_t frames, uint32_t channels,
                                          float samplingrate);

/**
 * Free the data of an audio buffer.
 *
 * The data pointer is set to NULL pointer.
 */
void audiobuffer_free(struct audiobuffer_t* audio);

/**
 * Return number of Bytes needed to pack this audio buffer
 */
size_t audiobuffer_get_size(struct audiobuffer_t);

/**
 * Serialize audio buffer into memory.
 *
 * @param audio Audio buffer
 * @param dest Start of memory array where the data should be stored
 * @param space Number of bytes available in the destination buffer
 * @return Number of bytes written, or zero in case of an error.
 */
size_t audiobuffer_write(struct audiobuffer_t audio, char* dest, size_t space);

/**
 * Fill audio data of an audio buffer from a serialized version.
 *
 * @param audio An allocated audio buffer
 * @param src Start of memory area
 * @param len Number of bytes of the serialized audio buffer
 * @return Zero in case of success, or non-zero in case of an error. Later
 * versions will specify an error code.
 */
int audiobuffer_read(struct audiobuffer_t* audio, char* src, size_t len);

#ifdef __cplusplus
}
#endif

#endif
