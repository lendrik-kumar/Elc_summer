import os

class Config:
    SQLALCHEMY_DATABASE_URI = os.getenv("DB_STRING")
    SQLALCHEMY_TRACK_MODIFICATIONS = False

    MQTT_HOST = os.getenv("MQTT_HOST")
    MQTT_PORT = int(os.getenv("MQTT_PORT", 8883))
    MQTT_USERNAME = os.getenv("MQTT_USERNAME")
    MQTT_PASSWORD = os.getenv("MQTT_PASSWORD")