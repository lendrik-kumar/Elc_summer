import time
import json
import paho.mqtt.client as paho
from paho import mqtt
from app.data_base import db
from app.data_base.models import SensorData
import os

def start_mqtt(app):
    def on_connect(client, userdata, flags, rc, properties=None):
        print("✅ MQTT Connected with result code:", rc)
        client.subscribe("esp32/sensors", qos=1)

    def on_publish(client, userdata, mid, properties=None):
        print("📤 Message published. mid:", mid)

    def on_subscribe(client, userdata, mid, granted_qos, properties=None):
        print("🔗 Subscribed:", mid, granted_qos)

    def on_message(client, userdata, msg):
        try:
            payload = json.loads(msg.payload.decode())
            print("📥 Received:", payload)

            with app.app_context():
                data = SensorData(
                    temperature=payload.get("temperature"),
                    humidity=payload.get("humidity"),
                    soil_moisture=payload.get("soil_moisture"),
                    full_payload=payload,
                )
                db.session.add(data)
                db.session.commit()
                print("✅ Data saved")

        except Exception as e:
            print("❌ MQTT error:", e)

    client = paho.Client(client_id="", userdata=None, protocol=paho.MQTTv5)
    client.tls_set(tls_version=mqtt.client.ssl.PROTOCOL_TLS)
    client.username_pw_set(
        username=os.getenv("MQTT_USERNAME"),
        password=os.getenv("MQTT_PASSWORD")
    )
    client.on_connect = on_connect
    client.on_message = on_message
    client.on_subscribe = on_subscribe
    client.on_publish = on_publish

    client.connect(os.getenv("MQTT_HOST"), int(os.getenv("MQTT_PORT", 8883)))
    client.loop_start()