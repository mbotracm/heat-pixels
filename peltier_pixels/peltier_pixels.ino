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
 * - ch<X>const - Set channel <X> to constant value
 * - ch<X>sine<hz> - Set channel <X> to sine wave with <hz> frequency (can be float)
 * - ch<X>amp<value> - Set channel <X> output amplitude to <value>
 * - ch<X>mod<value> - Set channel <X> output modulation (time shift) to <value> (can be float)
 *
 * <X> can be 0, 1, 2 or 3. If set to 9 than all channels are modified.
 * 
 * Author: Mordechai Botrashvily
 * Date: 20/05/2024
 */

void setup() {
  // put your setup code here, to run once:

}

void loop() {
  // put your main code here, to run repeatedly:

}
