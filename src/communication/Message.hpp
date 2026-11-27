#pragma once

#include <cstdint>
#include <cstring>
#include <vector>

#include "CRC.hpp"
#include "../util/Geometry.hpp"

namespace communication {

    /**
        Abstract base class for all messages following the DJI serial protocol.
        Defines the common interface and basic functionality for messages.

        Structure of a Serial Message:
        +-----------------+------------------------------------------------------------+
        | Byte Number     | Byte Description                                           |
        +=================+============================================================+
        | Frame Header                                                                 |
        +-----------------+------------------------------------------------------------+
        | 0               | Frame Head Byte (0xA5)                                     |
        +-----------------+------------------------------------------------------------+
        | 1               | Frame Data Length, LSB                                     |
        +-----------------+------------------------------------------------------------+
        | 2               | Frame Data Length, MSB                                     |
        +-----------------+------------------------------------------------------------+
        | 3               | Frame Sequence Number                                      |
        +-----------------+------------------------------------------------------------+
        | 4               | CRC8 of the frame, (bytes 0 - 3)                           |
        +-----------------+------------------------------------------------------------+
        | 5               | Message Type, LSB                                          |
        +-----------------+------------------------------------------------------------+
        | 6               | Message Type, MSB                                          |
        +-----------------+------------------------------------------------------------+
        | Body - Data Length bytes                                                     |
        +-----------------+------------------------------------------------------------+
        | Message CRC                                                                  |
        +-----------------+------------------------------------------------------------+
        | 7 + Data Length | CRC16 of header and frame, LSB (bytes 0 - 6 + Data Length) |
        +-----------------+------------------------------------------------------------+
        | 8 + Data Length | CRC16 of header and frame, MSB                             |
        +-----------------+------------------------------------------------------------+
    **/


    class DJIMessage {
        public:
            static constexpr uint8_t HEAD_BYTE = 0xA5;
            virtual uint16_t getID() const = 0;
            virtual std::vector<uint8_t> getPayload() const = 0;

            std::vector<uint8_t> createMessage(uint8_t sequence_num = 0) const {
                const std::vector<uint8_t> payload = getPayload();
                const uint16_t payload_len = static_cast<uint16_t>(payload.size());

                std::vector<uint8_t> msg;
                msg.reserve(5 + 2 + payload_len + 2);

                msg.push_back(HEAD_BYTE);
                appendLittleEndian(msg, payload_len);
                msg.push_back(sequence_num);
                msg.push_back(calculateCRC8(msg));

                appendLittleEndian(msg, getID());
                msg.insert(msg.end(), payload.begin(), payload.end());

                appendLittleEndian(msg, calculateCRC16(msg));

                return msg;
            }

        protected:
            template <typename T>
            static void appendLittleEndian(std::vector<uint8_t>& packet, T value) {
                uint8_t bytes[sizeof(T)];
                std::memcpy(bytes, &value, sizeof(T));
                packet.insert(packet.end(), bytes, bytes + sizeof(T));
            }
    };

    class RobotPositionMessage : public DJIMessage {
        public:
            RobotPositionMessage(const util::Point3D& position, uint8_t colour_id)
                : position(position), colour_id(colour_id) {}
            
            uint16_t getID() const override { return 0x0001; }

            std::vector<uint8_t> getPayload() const override {
                std::vector<uint8_t> payload;
                payload.reserve(13); // 3 floats (4 bytes each) + 1 byte for colour
                appendLittleEndian(payload, position.x);
                appendLittleEndian(payload, position.y);
                appendLittleEndian(payload, position.z);
                payload.push_back(colour_id);
                return payload;
            }

        private:
            util::Point3D position;
            uint8_t colour_id;
    };



} // namespace communication