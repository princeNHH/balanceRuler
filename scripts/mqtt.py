import sys
import json
import logging
import threading
import time
import paho.mqtt.client as mqtt

import logging
import threading
import json
import paho.mqtt.client as mqtt

class MQTTTable:
    MQTT_DEFAULT_PORT = 1883

    def __init__(self, mqtt_broker, mqtt_topic, logging_level=logging.INFO):
        self.mqtt_broker = mqtt_broker
        self.mqtt_topic = mqtt_topic
        self.stop_sig = threading.Event()
        self.stop_sig.clear()

        self.data_table = {}
        self.data_table_lock = threading.Lock()

        self.logger = logging.getLogger(self.__class__.__name__)
        self.logger.setLevel(logging_level)
        
        if not self.logger.handlers:
            ch = logging.StreamHandler()
            ch.setLevel(logging.DEBUG)
            ch.setFormatter(logging.Formatter("%(asctime)s %(levelname)s [%(name)s]: %(message)s", datefmt="%Y-%m-%d %H:%M:%S"))
            self.logger.addHandler(ch)

    def stop(self):
        self.stop_sig.set()
        self.logger.info("Stopped.")

    def start(self):
        self.logger.debug("Starting...")
        self.connect_mqtt()
        self.client.loop_start()

    def connect_mqtt(self):
        self.client = mqtt.Client()
        self.client.on_connect = self.on_connect
        self.client.on_message = self.on_message
        self.client.connect(self.mqtt_broker, self.MQTT_DEFAULT_PORT, 60)

    def on_connect(self, client, userdata, flags, rc):
        if rc == 0:
            self.logger.info("Connected to MQTT broker.")
            client.subscribe(self.mqtt_topic)
        else:
            self.logger.error(f"Failed to connect to MQTT broker with result code {rc}")

    def on_message(self, client, userdata, msg):
        try:
            data = json.loads(msg.payload.decode())
            
            key = data.get("key")
            if key:
                with self.data_table_lock:
                    self.data_table[key] = data.get("value")
                    # self.logger.info(f"Received data: {self.data_table}")
        except json.decoder.JSONDecodeError:
            self.logger.warning("Packet format error.")

    def get(self, key):
        with self.data_table_lock:
            return self.data_table.get(key)

    def cleanup(self):
        self.client.disconnect()
        self.client.loop_stop()
        self.logger.info("Cleaned up.")

if __name__ == "__main__":
    try:
        mqtt_table = MQTTTable("172.20.10.5", "system/hiep", logging_level=logging.DEBUG)
        
        mqtt_table.start()

        while not mqtt_table.stop_sig.is_set():
            print(mqtt_table.get("/joint/1"))
            
            time.sleep(0.01)
    except KeyboardInterrupt:
        mqtt_table.stop()