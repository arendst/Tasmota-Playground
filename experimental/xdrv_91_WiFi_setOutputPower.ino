/*

  EXPERIMENTAL DRIVER - PROVIDED FOR TINKERERS BUT NO SUPPORT PROVIDED

  How to use: Simply copy file into sonoff folder and compile - The driver will then be included automatically
              Please note that if you use more than one experimental driver you will need to rename the 91
              value to a different value e.g. 92-99
  
  xdrv_91_WiFi_setOutputPower.ino - Allow use of WiFi.setOutputPower(New_dBm); through command:

  WifiPower New_dBm

  Where New_dBm is an inclusive value of 0 through 20.5

  Copyright (C) 2019  Andre Thomas and Theo Arends

  This program is free software: you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation, either version 3 of the License, or
  (at your option) any later version.

  This program is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.

  You should have received a copy of the GNU General Public License
  along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#define XDRV_91                     91

const char kWifiPowerCommands[] PROGMEM = "|"  // No prefix
  "WifiPower" ;

void (* const WifiPowerCommand[])(void) PROGMEM = {
  &CmndWifiPower };

void CmndWifiPower(void)
{
  bool serviced = true;
  uint8_t paramcount = 0;
  if (XdrvMailbox.data_len > 0) {
    paramcount=1;
  } else {
    serviced = false;
    return;
  }
  char sub_string[XdrvMailbox.data_len+1];
  char tmp_string[5];
  sprintf(sub_string,"%s",XdrvMailbox.data); // Move the parameter to a char array
  float New_dBm = atof(sub_string);
  if (New_dBm < 0) { New_dBm = 0; }
  if (New_dBm > 20.5) { New_dBm = 20.5; }
  dtostrf(New_dBm, 2, 1, tmp_string);
  WiFi.setOutputPower(New_dBm);
  char tmp_response[32];
  sprintf(tmp_response,"WifiPower = %s", tmp_string);
  ResponseCmndChar(tmp_response);
}

bool Xdrv91(byte function)
{
  bool result = false;
  switch (function) {
    case FUNC_COMMAND:
      result = DecodeCommand(kWifiPowerCommands, WifiPowerCommand);
      break;
  }
  return result;
}
