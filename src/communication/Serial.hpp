#pragma once

#include <libserial/SerialPort.h>
#include <string>
#include <vector>

class Serial {
 public:
  Serial(const std::string& port_name, LibSerial::BaudRate baudrate) {
    port.Open(port_name);
    port.SetBaudRate(baudrate);
    port.SetCharacterSize(LibSerial::CharacterSize::CHAR_SIZE_8);
    port.SetParity(LibSerial::Parity::PARITY_NONE);
    port.SetStopBits(LibSerial::StopBits::STOP_BITS_1);
    port.SetFlowControl(LibSerial::FlowControl::FLOW_CONTROL_NONE);
  }

  void write(const std::vector<uint8_t>& data) {
    port.Write(data);
  }

 private:
  LibSerial::SerialPort port;
};