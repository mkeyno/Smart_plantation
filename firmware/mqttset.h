#ifndef _MQTTSET_H
#define _MQTTSET_H
 
AsyncMqttClient mqttClient;
int  _MQTT_port=1883;

IPAddress str2ip(String );

void onMqttConnect(bool sessionPresent) {
  PRINTLN(F("** Connected to the broker **"));
  PRINT(F("Session present: "));  PRINTLN(sessionPresent);
                                                              uint16_t packetIdSub = mqttClient.subscribe("test/lol", 2);
  PRINT(F("Subscribing at QoS 2, packetId: "));  PRINTLN(packetIdSub);
                                mqttClient.publish("test/lol", 0, true, "test 1"); PRINTLN(F("Publishing at QoS 0")); 
  uint16_t       packetIdPub1 = mqttClient.publish("test/lol", 1, true, "test 2"); PRINT(F("Publishing at QoS 1, packetId: "));
  PRINTLN(packetIdPub1);
  uint16_t       packetIdPub2 = mqttClient.publish("test/lol", 2, true, "test 3");PRINT(F("Publishing at QoS 2, packetId: "));    
  PRINTLN(packetIdPub2);
}
void onMqttDisconnect(AsyncMqttClientDisconnectReason reason) {
  PRINTLN(F("** Disconnected from the broker **"));
  PRINTLN(F("Reconnecting to MQTT..."));
  mqttClient.connect();
}
void onMqttSubscribe(uint16_t packetId, uint8_t qos) {
  PRINTLN(F("** Subscribe acknowledged **"));
  PRINT(F("  packetId: "));PRINTLN(packetId);PRINT(F("  qos: ")); PRINTLN(qos); 
}
void onMqttUnsubscribe(uint16_t packetId) {
  PRINTLN(F("** Unsubscribe acknowledged **"));
  PRINT(F("  packetId: ")); PRINTLN(packetId); 
}
void onMqttMessage(char* topic, char* payload, AsyncMqttClientMessageProperties properties, size_t len, size_t index, size_t total) {
  PRINTLN(F("** Publish received **"));
  PRINT(F("  topic: "));  PRINTLN(topic);
  PRINT(F("  qos: "));  PRINTLN(properties.qos);
  PRINT(F("  dup: "));  PRINTLN(properties.dup);
  PRINT(F("  retain: "));  PRINTLN(properties.retain);
  PRINT(F("  len: "));  PRINTLN(len);
  PRINT(F("  index: "));  PRINTLN(index);
  PRINT(F("  total: "));  PRINTLN(total);
}
void onMqttPublish(uint16_t packetId) {
  PRINTLN(F("** Publish acknowledged **"));
  PRINT(F("  packetId: "));  PRINTLN(packetId);
}

void Mqtt_setting(void){
	
mqttClient.onConnect(onMqttConnect);
mqttClient.onDisconnect(onMqttDisconnect);
mqttClient.onSubscribe(onMqttSubscribe);
mqttClient.onUnsubscribe(onMqttUnsubscribe);
mqttClient.onMessage(onMqttMessage);
mqttClient.onPublish(onMqttPublish);
mqttClient.setServer( str2ip(Device_info[MQTT_Server_IP]), _MQTT_port);//setCredentials(Device_info[MQTT_user].c_str(),Device_info[MQTT_pass].c_str()).
mqttClient.setKeepAlive(5).setCleanSession(false).setWill(Device_info[MQTT_Topic].c_str(),QOS, Retain, "no").setClientId(Device_info[MQTT_clientid].c_str());
PRINTLN(F("Connecting to MQTT..."));
mqttClient.connect();	
}

#endif
