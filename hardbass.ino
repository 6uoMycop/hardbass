/**
 * \brief Hardbass vibrator
 * \copyright No copyright
 * \autor Arkadii Petushkov <Arkady1234@yandex.ru>
 * \version 1.0
 */

/** Define if serial output is needed */
#define DEBUG

/** Input analog port for Troyka audio input module output channel */
#define PORT_IN A0
/** Output PWM port (to transistor) */
#define PORT_OUT 9
/** Number of measurements for local_max calculation */
#define N 70
/** Idle zero value. Power-related */
#define ZERO_VAL 96

#ifdef DEBUG
/** Serial port baudrate (debug) */
#define BAUDRATE 115200
#endif

/** Buffer variable for incoming data. Note: signed */
int16_t v;
/**
 * Maximum value among N measurements used as output. 
 * Must be casted to uint8_t and be in valid range
 */
uint16_t local_max;

void setup()
{
#ifdef DEBUG
  /** If needed for debug*/
  Serial.begin(BAUDRATE);
#endif
}

void loop()
{
  /** Obtain local maximum value */
  local_max = 0;
  for(uint8_t i = 0; i < N; i++)
  {
    /**
     * Read input, fit in 8 bits and erase less significant bit for noise reduction.
     * Also set down to zero
     */
    v = ((analogRead(PORT_IN) >> 3) << 1) - ZERO_VAL;
    /** Invert negative part of wave */
    v = (v < 0) ? (-v) : (v);
    /** Remember if max */
    local_max = (v > local_max) ? v : local_max;
  }

  /** Multiply by 8 */
  local_max <<= 2;
  /** Check overrange */
  if(local_max > 0xFF)
  {
    local_max = 0xFF;
  }

  /** Write to PWM port */
  analogWrite(PORT_OUT, (uint8_t)local_max);

A#ifdef DEBUG
  /** Print value to serial */
  Serial.print((uint8_t)local_max);
  Serial.print("\n");
#endif
}
