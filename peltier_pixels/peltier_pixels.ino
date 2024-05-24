/*
 * This sketch operates multiple peltier elements using a multi-channel MOSFET.
 * The user can configure each channel through serial commands, either by 
 * direct USB or via a Bluetooth connection.
 *
 * MOSFET channel to Arduino pin mapping (using proto shield extension):
 * - CH0 -> 8
 * - CH1 -> 9
 * - CH2 -> 10
 * - CH3 -> 11
 *
 * Supported commands:
 * - ch<X>con - Set channel <X> to constant value
 * - ch<X>sin<hz> - Set channel <X> to sine wave with <hz> frequency (can be float)
 * - ch<X>amp<value> - Set channel <X> output amplitude to <value>
 * - ch<X>mod<value> - Set channel <X> output modulation (time shift) to <value> (can be float)
 *
 * <X> can be 0, 1, 2 or 3. If set to 9 than all channels are modified.
 * 
 * Author: Mordechai Botrashvily
 * Date: 20/05/2024
 */

const String COMMAND_PREFIX("ch");
const String SUBCOMMAND_PREFIX_CONST("con");
const String SUBCOMMAND_PREFIX_SINE("sin");
const String SUBCOMMAND_PREFIX_AMPLITUDE("amp");
const String SUBCOMMAND_PREFIX_MODULATION("mod");

// Arrays for storing the configuration for each channel.
// By default mode of operation for each channel is constant. A value of true 
// means sine wave output.
bool channelOperationMode[4] = {0};
// By default sine wave frequency for each channel is 0.2 Hz (5 seconds).
float channelSineFrequency[4] = {0.2f};
// By default output amplitude for each channel is 0 (lowest amplitude).
uint8_t channelAmplitude[4] = {0};
// By default sine wave modulation for each channel is 0 seconds (no delay).
float channelSineModulation[4] = {0.0f};

void setup() {
  // Configure serial channel to baudrate of 115,200 bps.
  Serial.begin(115200);
}

void loop() {
  // Process input commands, should always start with ch and a digit.
  char commandPrefix[2];
  char subcommandPrefix[4] = {0};
  int channel = -1;
  if (Serial.available() > 2) {
    Serial.readBytes(commandPrefix, 2);

    // If input corresponds to a valid command, parse channel number.
    if (COMMAND_PREFIX == commandPrefix) {
      channel = Serial.parseInt();
      Serial.print("Received command for channel: ");
      Serial.println(channel);

      // Parse specific command.
      Serial.readBytes(subcommandPrefix, 3);
      Serial.print("Subcommand: ");
      Serial.println(subcommandPrefix);

      // Handle subcommands.
      // Constant output value.
      if (SUBCOMMAND_PREFIX_CONST == subcommandPrefix)
      {
        // Set mod of operation to constant.
      }
      // Sine output value.
      if (SUBCOMMAND_PREFIX_SINE == subcommandPrefix)
      {
        // Parse desired frequency.
        float freq = Serial.parseFloat();
        Serial.print("Freq: ");
        Serial.println(freq);
        // Set mod of operation to sine wave.
      }
      // Set output amplitude.
      if (SUBCOMMAND_PREFIX_AMPLITUDE == subcommandPrefix)
      {
        // Parse desired amplitude.
        uint8_t amplitude = Serial.parseInt();
        Serial.print("Amp: ");
        Serial.println(amplitude);
      }
      // Set output sine wave modulation.
      if (SUBCOMMAND_PREFIX_MODULATION == subcommandPrefix)
      {
        // Parse desired modulation.
        float modulation = Serial.parseFloat();
        Serial.print("Mod: ");
        Serial.println(modulation);
      }
    }
    else
    {
      Serial.print("Unrecognized command: ");
      Serial.print(commandPrefix);
      Serial.println(Serial.readStringUntil('\n'));
    }
  }
}
