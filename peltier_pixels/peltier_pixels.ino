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

String COMMAND_PREFIX("ch");
String SUBCOMMAND_PREFIX_CONST("con");
String SUBCOMMAND_PREFIX_SINE("sin");
String SUBCOMMAND_PREFIX_AMPLITUDE("amp");
String SUBCOMMAND_PREFIX_MODULATION("mod");

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
        /* code */
      }
      // Sine output value.
      if (SUBCOMMAND_PREFIX_SINE == subcommandPrefix)
      {
        /* code */
      }
      // Set output amplitude.
      if (SUBCOMMAND_PREFIX_AMPLITUDE == subcommandPrefix)
      {
        /* code */
      }
      // Set output sine wave modulation.
      if (SUBCOMMAND_PREFIX_MODULATION == subcommandPrefix)
      {
        /* code */
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
