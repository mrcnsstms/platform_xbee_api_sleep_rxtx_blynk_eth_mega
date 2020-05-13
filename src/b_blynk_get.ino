
//////////////////////////////////////////////////////////////////////

BLYNK_WRITE(V6)
{
  m_hw.Println("XBEE_LIB: V6 command RX'd", debug::MOST);
  struct Msg_data blynk_data;
  blynk_data.address = 0;
  blynk_data.valid = true;
  blynk_data.payload_id = CMD_ID::IO_OUT;
  blynk_data.payload_len = 3;
  blynk_data.payload[0] = 6; // pin 6
  int pinValue = param.asInt(); // am_hwigning incoming value from pin V1 to a variable

  if(pinValue)
  {
    blynk_data.payload[1] = 1; // state
    blynk_data.payload[2] = 1; // duration (0 is permanent)
  }

  else
  {
    blynk_data.payload[1] = 0; // state
    blynk_data.payload[2] = 0; // duration (0 is permanent)
  }
  update_tx_msg(blynk_data);
}

//////////////////////////////////////////////////////////////////////

BLYNK_WRITE(V36) //V36
{
  m_hw.Println("XBEE_LIB: V36 command RX'd", debug::MOST);
  struct Msg_data blynk_data;
  blynk_data.address = 3;
  blynk_data.valid = true;
  blynk_data.payload_id = CMD_ID::IO_OUT;
  blynk_data.payload_len = 3;
  blynk_data.payload[0] = 6; // pin 6
  int pinValue = param.asInt(); // am_hwigning incoming value from pin V1 to a variable

  if(pinValue)
  {
    blynk_data.payload[1] = 1; // state
    blynk_data.payload[2] = 0; // duration (0 is permanent)
  }

  else
  {
    blynk_data.payload[1] = 0; // state
    blynk_data.payload[2] = 0; // duration (0 is permanent)
  }
  update_tx_msg(blynk_data);
  m_xbee.Clear_msg(blynk_data);
}

//////////////////////////////////////////////////////////////////////

BLYNK_WRITE(V14)
{
  String cmd = param[0].asStr();
  if (cmd == "select")
  {
    //row in table was selected.
    int rowId = param[1].asInt();
    m_hw.Print("Row ID: ", rowId, debug::MOST);
    m_hw.Println(" selected", debug::MOST);
  }

  if (cmd == "deselect")
  {
    //row in table was selected.
    int rowId = param[1].asInt();
    //m_hw.Print("Row ID: ", rowId, MOST);
    //m_hw.Println(" de-selected", MOST);
  }
  if (cmd == "order")
  {
    //rows in table where reodered
    int oldRowIndex = param[1].asInt();
    int newRowIndex = param[2].asInt();
  }
}
