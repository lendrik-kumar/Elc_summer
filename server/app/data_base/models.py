from app.data_base import db
from datetime import datetime
from sqlalchemy.dialects.postgresql import JSON

class SensorData(db.Model):
    __tablename__ = 'sensor_data'

    id = db.Column(db.Integer, primary_key=True)
    temperature = db.Column(db.Float)
    humidity = db.Column(db.Float)
    soil_moisture = db.Column(db.Float)
    full_payload = db.Column(db.JSON)
    timestamp = db.Column(db.DateTime, default=datetime.utcnow)