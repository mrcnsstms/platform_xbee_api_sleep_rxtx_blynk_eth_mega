#include "millisDelay.h"
#include "version.h"
#include "Xbee_lib.h"
#include "Xbee_lib_defs.h"
#include <Print_lib.h>
#include <Message_lib_defs.h>
#include <SPI.h>
#include <Ethernet.h>
#include <Sensor_lib.h>
#define BLYNK_PRINT Serial // must be before Blynk include
#include <BlynkSimpleEthernet.h>

#define W5100_CS   10
#define SDCARD_CS   4
#define LED_PIN    13

const char auth[] = "v1KVwnFGjFTRMiuZUeZyL9XAs7CGB43r";

Print_lib m_hw(&Serial, debug::MOST);
Xbee_lib m_xbee(&m_hw);

millisDelay m_remote_status_timer;
millisDelay m_local_timer;

struct Msg_data m_rx_blynk_msg;
struct Msg_data m_tx_msg[6];
struct Msg_data m_rx_msg[6];

uint8_t m_tx_count[6] = {}; // number of xbees in network

//////////////////////////////////////////////////////////////////////

void setup()
{
  // allow time to switch to xbee mode on pcb
  delay(2000);
  Serial1.begin(19200);
  m_hw.Begin(19200);
  m_hw.Println("xbee_api_sleep_txrx_blynk_eth_mega : ",
               version,
               debug::SOME);

  // pin definitions
  pinMode(LED_PIN, OUTPUT);
  pinMode(SDCARD_CS, OUTPUT);
  digitalWrite(SDCARD_CS, HIGH); // Deselect SD card
  pinMode(W5100_CS, OUTPUT);
  digitalWrite(W5100_CS, HIGH);

  // delay for reporting status
  m_remote_status_timer.start(1000);
  // delay for reporting status
  m_local_timer.start(3000);

  // callback for when valid remote data received
  m_xbee.Set_callback(Message_received);

  //setup blynk parameters
  Blynk.begin(auth);   // must be last call
}

//////////////////////////////////////////////////////////////////////

void loop()
{
  //Run blynk
  Blynk.run();

  //Run_local();

  handle_wireless();

  if(m_remote_status_timer.justFinished())
  {
    m_remote_status_timer.repeat();
    handle_remote_status();
  }

  if(m_local_timer.justFinished())
  {
    m_local_timer.repeat();
    //Run_local();
  }
}

//////////////////////////////////////////////////////////////////////

void Run_local()
{
  // build message, insert payloads
  struct Msg_data tx_msg;
  tx_msg.frame_type = 0x10;
  tx_msg.address = ID::XBEE_1;
  tx_msg.payload_len = 1;
  tx_msg.payload_cnt = m_tx_count;
  tx_msg.payload_id = STAT_ID::IO_IN;
  tx_msg.payload[0] = 1;  // length
  tx_msg.payload[1] = '!';

  char char_array[2] = {99,'!'};
  //m_hw.Println("char 0: ", static_cast<uint8_t>(char_array[0]), debug::MOST);
  //m_hw.Println("char 1: ", char_array[1], debug::MOST);
  // light sensor
  // Sensor_lib sensor(3.3);
  // m_hw.Println("Payload length 0: ", tx_msg.payload[0], debug::SOME);
  // m_hw.Println("sizeof() 0: ", sizeof(tx_msg.payload), debug::SOME);
  // tx_msg.payload_len = sensor.Get_analog(A0,
  //                                        io_type::LIGHT,
  //                                        tx_msg.payload);
  m_hw.Print_msg("Building tx_msg: ",tx_msg, DEC, debug::SOME);
  // m_hw.Println("Payload length 1: ", tx_msg.payload[0], debug::SOME);
  // m_hw.Println("sizeof() 1: ", sizeof(tx_msg.payload), debug::SOME);
  // tx_msg.payload_len = sensor.Get_analog(6,
  //                                        io_type::DALLAS_TEMP,
  //                                        tx_msg.payload);
  // m_hw.Println("Payload length 2: ", tx_msg.payload[0], debug::SOME);
  // m_hw.Println("sizeof() 2: ", sizeof(tx_msg.payload), debug::SOME);
  // tx_msg.payload_len = sensor.Get_analog(A7,
  //                                        io_type::BATTERY,
  //                                        tx_msg.payload);
  // m_hw.Println("Payload length 3: ", tx_msg.payload[0], debug::SOME);
  // m_hw.Println("sizeof() 3: ", sizeof(tx_msg.payload), debug::SOME);
  // light_sensor
  // analogRead(A0);
  // int sum = 0;
  // for(int i = 0; i < 5; i++)
  // {
  //   sum += analogRead(A0);
  // }
  // int dac = sum/5;
  // uint8_t light = map(dac, 0, 1023, 0, 100);
  // tx_msg.payload[0] = light;

  // temp sensor
  // tx_msg.payload[1] = getDallasTemp();

  // battery
  // analogRead(A7); // throw away
  // uint16_t battery = analogRead(A7);
  // tx_msg.payload[2] = battery/4;
}

//////////////////////////////////////////////////////////////////////

void Transmit_frame(const uint8_t* frame, const uint8_t length)
{
  // doesn't work properly, tx's huge frame
  //m_hw.Print_array("TX frame: ", frame, length, debug::ALL);
  m_hw.Print("TX frame: ", debug::ALL);
  m_hw.Print_array(frame, length, HEX, debug::ALL);
  delay(10);
  Serial1.write(frame, length);
  delay(10);
}

//////////////////////////////////////////////////////////////////////

uint8_t Convert_payload(uint8_t payload_id[], uint8_t length)
{
  switch(payload_id[0])
  {
    case 'i' :
    break;
  }
}

//////////////////////////////////////////////////////////////////////

void handle_remote_status()
{
  m_hw.Println("Handle remote status", debug::ALL);
  for(int i = 0; i < 5; i++)
  {
    if(m_rx_msg[i].valid)
    {
      m_hw.Println("Remote address: ", m_rx_msg[i].address, debug::ALL);
      uint8_t light = map(m_rx_msg[i].payload[0], 0, 255, 0, 100);
      m_hw.Println("Light[%]: ", light, debug::ALL);
      m_hw.Println("Temperature[F]: ", m_rx_msg[i].payload[1], debug::ALL);
      float battery = m_rx_msg[i].payload[2] * 8 * 0.003157; // 0.0032258 @ 3.3V
      m_hw.Println("Battery[V]: ", battery, debug::ALL);

      Post_blynk_io_stat(m_rx_msg[i]);
    }
  }
}

//////////////////////////////////////////////////////////////////////

void update_tx_msg(const struct Msg_data blynk_data)
{
  m_hw.Println("Updating TX msg", debug::MOST);
  m_hw.Print_msg(blynk_data, DEC, debug::MOST);
  m_tx_msg[blynk_data.address] = blynk_data;
}

//////////////////////////////////////////////////////////////////////

void handle_wireless()
{
  while(Serial1.available())
  {
    m_xbee.Process_byte(Serial1.read());
  }
}

//////////////////////////////////////////////////////////////////////

void Blynk_received(const struct Msg_data blynk_data)
{

}

//////////////////////////////////////////////////////////////////////

void Message_received(const struct Msg_data rx_data)
{
  m_hw.Print_msg("RX msg: ", rx_data, HEX, debug::ALL);
  switch(rx_data.payload_id)
  {
    case CMD_ID::NO_ACK :
      m_hw.Println("RX'd No Ack", debug::MOST);
      // do nothing, no response
      break;

    case CMD_ID::ACK :
      m_hw.Println("RX'd Ack", debug::MOST);
      // build response
      break;

    case CMD_ID::IO_IN :
      m_hw.Print("RX'd IO_IN, ", debug::MOST);
      m_hw.Println("Address: ", rx_data.address, debug::MOST);

      // update rx data container
      m_rx_msg[rx_data.address] = rx_data;
      break;

    default :
      m_hw.Print("Unknown CMD::ID: ", debug::MOST);
      m_hw.Println(rx_data.payload_id, debug::MOST);
  }

  // build message, insert payloads
  struct Msg_data tx_msg;
  tx_msg.frame_type = 0x10;
  tx_msg.address = rx_data.address;
  tx_msg.payload_len = m_tx_msg[tx_msg.address].payload_len;
  tx_msg.payload_cnt = m_tx_count[rx_data.address];
  tx_msg.payload_id = m_tx_msg[tx_msg.address].payload_id;;

  // payload
  tx_msg.payload[0] = m_tx_msg[tx_msg.address].payload[0];
  tx_msg.payload[1] = m_tx_msg[tx_msg.address].payload[1];
  tx_msg.payload[2] = m_tx_msg[tx_msg.address].payload[2];

  uint8_t tx_array[sizeof(tx_msg.payload) + 20];
  bool tx_ok = m_xbee.Build_frame(tx_msg, tx_array);
  if(tx_ok)
  {
    Transmit_frame(tx_array, sizeof(tx_array));
    m_tx_count[rx_data.address]++;
  }

  // if only meant to send this once
  if(true)
  {
    m_xbee.Clear_msg(m_tx_msg[tx_msg.address]);
  }
}
