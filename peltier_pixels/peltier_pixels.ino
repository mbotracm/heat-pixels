/*
 * This sketch operates multiple peltier elements using a multi-channel MOSFET.
 * The user can configure each channel through serial commands, either by 
 * direct USB or via a Bluetooth connection.
 *
 * MOSFET channel to Arduino pin mapping (using proto shield extension):
 * - CH0 -> 5
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

const int CHANNEL_PIN_MAPPING[4] = {5, 9, 10, 11};

const String COMMAND_PREFIX("ch");
const String SUBCOMMAND_PREFIX_CONST("con");
const String SUBCOMMAND_PREFIX_SINE("sin");
const String SUBCOMMAND_PREFIX_AMPLITUDE("amp");
const String SUBCOMMAND_PREFIX_MODULATION("mod");
const String SUBCOMMAND_PREFIX_GET("get");

// Arrays for storing the configuration for each channel.
// By default mode of operation for each channel is constant. A value of true 
// means sine wave output.
bool channelOperationMode[4] = {0, 0, 0, 0};
// By default sine wave frequency for each channel is 0.2 Hz (5 seconds).
float channelSineFrequency[4] = {0.2f, 0.2f, 0.2f, 0.2f};
// By default output amplitude for each channel is 0 (lowest amplitude).
int channelAmplitude[4] = {0, 0, 0, 0};
// By default sine wave modulation for each channel is 0 seconds (no delay).
float channelSineModulation[4] = {0.0f, 0.0f, 0.0f, 0.0f};

// Indicator variable to update a channel after serial command.
int updateChannel = -1;

void setup()
{
  // Configure serial channel to baudrate of 115,200 bps.
  Serial.begin(115200);

  // Configure output pins.
  for (uint8_t i = 0; i < 4; i++)
  {
    pinMode(CHANNEL_PIN_MAPPING[i], OUTPUT);
  }
}

void parseCommands()
{
  // Process input commands, should always start with ch and a digit.
  char commandPrefix[3] = {0};
  char subcommandPrefix[4] = {0};
  int channel = -1;
  if (Serial.available() > 2)
  {
    Serial.readBytes(commandPrefix, 2);

    // If input corresponds to a valid command, parse channel number.
    if (COMMAND_PREFIX == commandPrefix)
    {
      channel = Serial.parseInt();
      Serial.print("Received command for channel: ");
      Serial.println(channel);

      // Validate channel number.
      if (channel < 0 || (channel > 3 && channel != 9))
      {
        Serial.println("Invalid channel");
        // Read remaining serial buffer to clear line.
        Serial.readStringUntil('\n');
        return;
      }

      // Channel IDs in general, unless a specific channel was requested.
      uint8_t minChannel = 0;
      uint8_t maxChannel = 4;

      if (channel != 9)
      {
        // Handle single channel.
        minChannel = channel;
        maxChannel = channel + 1;
      }

      // Parse specific command.
      Serial.readBytes(subcommandPrefix, 3);
      Serial.print("Subcommand: ");
      Serial.println(subcommandPrefix);

      // Handle subcommands.
      // Constant output value.
      if (SUBCOMMAND_PREFIX_CONST == subcommandPrefix)
      {
        // Set mod of operation to constant.
        // Handle relevant channels.
        for (uint8_t i = minChannel; i < maxChannel; i++)
        {
          channelOperationMode[i] = false;
        }

        updateChannel = channel;
      }
      // Sine output value.
      if (SUBCOMMAND_PREFIX_SINE == subcommandPrefix)
      {
        // Parse desired frequency.
        float freq = Serial.parseFloat();
        Serial.print("Freq: ");
        Serial.println(freq);

        // Set mod of operation to sine wave.
        // Update frequency.
        // Handle relevant channels.
        for (uint8_t i = minChannel; i < maxChannel; i++)
        {
          channelOperationMode[i] = true;
          channelSineFrequency[i] = freq;
        }

        updateChannel = channel;
      }
      // Set output amplitude.
      if (SUBCOMMAND_PREFIX_AMPLITUDE == subcommandPrefix)
      {
        // Parse desired amplitude.
        int amplitude = Serial.parseInt();
        Serial.print("Amp: ");
        Serial.println(amplitude);

        // Update amplitude.
        // Handle relevant channels.
        for (uint8_t i = minChannel; i < maxChannel; i++)
        {
          channelAmplitude[i] = amplitude;
        }

        updateChannel = channel;
      }
      // Set output sine wave modulation.
      if (SUBCOMMAND_PREFIX_MODULATION == subcommandPrefix)
      {
        // Parse desired modulation.
        float modulation = Serial.parseFloat();
        Serial.print("Mod: ");
        Serial.println(modulation);

        // Update modulation.
        // Handle relevant channels.
        for (uint8_t i = minChannel; i < maxChannel; i++)
        {
          channelSineModulation[i] = modulation;
        }

        updateChannel = channel;
      }
      // Print channel configuration.
      if (SUBCOMMAND_PREFIX_GET == subcommandPrefix)
      {
        // Update modulation.
        // Handle relevant channels.
        Serial.println("--- CONFIG ---");
        for (uint8_t i = minChannel; i < maxChannel; i++)
        {
          Serial.print("Channel: ");
          Serial.println(i);
          Serial.print("Mod: ");
          Serial.println(channelOperationMode[i]);
          Serial.print("Sine Freq: ");
          Serial.println(channelSineFrequency[i]);
          Serial.print("Amplitude: ");
          Serial.println(channelAmplitude[i]);
          Serial.print("Sine Mod: ");
          Serial.println(channelSineModulation[i]);
        }
      }
    }
    else
    {
      Serial.print("Unrecognized command: ");
      Serial.println(commandPrefix);
      Serial.println(Serial.readStringUntil('\n'));
    }
  }
}

void configureChannels()
{
  // Don't send channel commands if not required.
  if (updateChannel == -1)
  {
    return;
  }

  // Channel IDs in general, unless a specific channel was requested.
  uint8_t minChannel = 0;
  uint8_t maxChannel = 4;

  if (updateChannel != 9)
  {
    // Handle single channel.
    minChannel = updateChannel;
    maxChannel = updateChannel + 1;
  }

  Serial.print("Updating channel: ");
  Serial.println(updateChannel);

  // Loop over all channel and output corresponding configuration.
  for (uint8_t i = minChannel; i < maxChannel; i++)
  {
    int output = 0;

    // Constant output.
    if (channelOperationMode[i] == false)
    {
      output = channelAmplitude[i];
    }
    else
    {
      // Sine wave output.
      // Angular frequency = 2pi*f.
      float w = 2 * PI * channelSineFrequency[i];
      // Time since beginning.
      double t = millis() / 1000.0;
      // Compute sine wave with modulation.
      output = channelAmplitude[i] * sin(w * (t - channelSineModulation[i]));
    }

    // Output desired value.
    // TODO this method of using PWM actually specifies the duty cycle of the
    // signal. So it supports a constant mode of operation.
    // To fully support a sine wave, we'll have to modify the duty cycle in 
    // more granular way.
    analogWrite(CHANNEL_PIN_MAPPING[i], output);
  }

  // After update is complete, clear status.
  updateChannel = -1;
}

void loop()
{
  // Handle input commands if any.
  parseCommands();

  // Output current configuration for each channel.
  configureChannels();
}
