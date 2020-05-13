//////////////////////////////////////////////////////////////////////

void Post_blynk_io_stat(const struct Msg_data blynk_data)
{
  m_hw.Println("Posting to Blynk", debug::ALL);
  switch(blynk_data.address)
  {
    case 1 :
    {
      float battery = blynk_data.payload[2] * 8 * 0.003157; // 0.0032258 @ 3.3V
      float temp = blynk_data.payload[1];
      uint8_t light = map(blynk_data.payload[0], 0, 255, 0, 100);

      Blynk.virtualWrite(V0, battery);
      Blynk.virtualWrite(V1, temp);
      Blynk.virtualWrite(V2, light);
      break;
    }
    case 2 :
    {
      float battery = blynk_data.payload[2] * 8 * 0.003157; // 0.0032258 @ 3.3V
      float temp = blynk_data.payload[1];
      uint8_t light = map(blynk_data.payload[0], 0, 255, 0, 100);

      Blynk.virtualWrite(V5, battery);
      Blynk.virtualWrite(V6, temp);
      Blynk.virtualWrite(V7, light);
      break;
    }
    case 3 :
    {
      float battery = blynk_data.payload[2] * 8 * 0.003157; // 0.0032258 @ 3.3V
      float temp = blynk_data.payload[1];
      uint8_t light = map(blynk_data.payload[0], 0, 255, 0, 100);

      Blynk.virtualWrite(V10, battery);
      Blynk.virtualWrite(V11, temp);
      Blynk.virtualWrite(V12, light);
      break;
    }
    case 4 :
      break;

    case 5 :
      break;

    case 6 :
      break;
  }


  WidgetMap myMap1(V13);
  int index1 = 1;
  float lat1 = 44.484306;
  float lon1 = -73.209325;
  myMap1.location(index1, lat1, lon1, "matt, parilla");

  int index2 = 2;
  float lat2 = 44.487366;
  float lon2 = -73.211969;
  myMap1.location(index2, lat2, lon2, "em");
  //Blynk.setProperty(V13, "color", "#D3435C");


  WidgetTable table(V14);
  static bool add = true;
  if(add)
  {
    table.addRow(1, "Xbee1", millis() / 1000);
    table.addRow(2, "Xbee2", millis() / 1000);
    table.addRow(3, "Xbee3", millis() / 1000);
    add = false;
  }
  else
  {
    table.updateRow(1, "Xbee1 new", "UpdatedValue1");
    table.updateRow(2, "Xbee2 new", "UpdatedValue2");
    table.updateRow(3, "Xbee3 new", "UpdatedValue3");
  }

  table.pickRow(2);
}

//////////////////////////////////////////////////////////////////////

void Post_blynk_location(const struct Msg_data blynk_data)
{
  m_hw.Println("Posting blynk location", debug::ALL);
};
