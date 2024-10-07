# Plant Assistant for Arduino

This program connects to wifi and mqtt, reads a capacitative soil moisture sensor and publishes the sensor readings to a topic. 

This integrates with home assistant.

## MQTT Topics

### Outbound
| Topic                                                  | JSON                 |
|---------------                                         |------------------    |
| plant_assistant/indoor/<unique_ID>/soil_moisture       | "{"Vol%": %d}"     |
| plant_assistant/indoor/<unique_ID>/battery_status      | "{"%": %d}"     |
| plant_assistant/outdoor/<unique_ID>/<sensor(1-4)>/soil_moisture | "{"Vol%": %d}"   |
| plant_assistant/outdoor/<unique_ID>/battery_status     |"{"%": %d}"      |





### Inbout
plant_assistant/indoor/<unique_ID>/reboot
plant_assistant/indoor/<unique_ID>/switch_mode





