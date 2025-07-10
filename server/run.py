from dotenv import load_dotenv
load_dotenv()

from app import create_app
from app.mqtt.client import start_mqtt
from app.data_base import db

app = create_app()

with app.app_context():
    db.create_all()


if __name__ == "__main__":
    print("Flask starting...")
    start_mqtt(app)  
    app.run(host="0.0.0.0", port=6000, debug=True)
