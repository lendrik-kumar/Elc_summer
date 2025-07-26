# PlantVillage IoT & AI Platform

A full-stack smart agriculture platform combining IoT sensor monitoring, real-time dashboard, and AI-powered pest/disease detection.

---

## Features

- **IoT Sensors:** ESP32-based sensors stream temperature, humidity, soil moisture, and water depth via MQTT.
- **AI Detection:** Upload or capture plant images for pest/disease detection using deep learning models.
- **Real-Time Dashboard:** Mobile app displays live sensor data and AI predictions.
- **Socket.IO:** Real-time communication between backend and frontend.
- **REST API:** Endpoints for disease and pest prediction.

---

## Project Structure

```
elc_summer/
├── server/           # Flask backend, MQTT, AI models, database
├── my-expo-app/      # Expo React Native frontend app
├── mqtt/             # ESP32 Arduino sketch for sensors
```

---

## Backend (`server/`)

- **Tech:** Flask, Flask-SocketIO, SQLAlchemy, Paho-MQTT, PyTorch, Timm
- **Main entry:** [`server/run.py`](server/run.py)
- **Database:** PostgreSQL

### Setup

1. Install dependencies:
   ```sh
   cd server
   pip install -r requirements.txt
   ```
2. Configure environment variables in [`server/.env`](server/.env):
   ```
   DB_STRING = "postgresql://postgres:yourpassword@localhost:5432/Elc_summer"
   MQTT_USERNAME = "your_mqtt_user"
   MQTT_PASSWORD = "your_mqtt_pass"
   MQTT_HOST = "your_mqtt_host"
   MQTT_PORT = "8883"
   ```
3. Start PostgreSQL and create the database.
4. Run the server:
   ```sh
   python run.py
   ```

---

## Frontend (`my-expo-app/`)

- **Tech:** React Native (Expo), NativeWind, Socket.IO client, Zustand
- **Main entry:** [`my-expo-app/App.tsx`](my-expo-app/App.tsx)
- **Pages:** Home (dashboard), Manual (image upload/capture for AI detection)

### Setup

1. Install dependencies:
   ```sh
   cd my-expo-app
   npm install
   ```
2. Set API base URL in [`my-expo-app/.env`](my-expo-app/.env):
   ```
   API_BASE_URL = "http://<your-server-ip>:6000"
   ```
3. Start the app:
   ```sh
   npx expo start
   ```

---

## ESP32 IoT (`mqtt/`)

- Flash the sketch in [`mqtt/sketch_jul09b/sketch_jul09b.ino`](mqtt/sketch_jul09b/sketch_jul09b.ino) to your ESP32.
- Update WiFi and MQTT credentials as needed.

---

## API Endpoints

- `POST /predict/disease` — Upload image for disease detection.
- `POST /predict/pest` — Upload image for pest detection.

---

## Useful Files

- [`server/run.py`](server/run.py): Main backend entry point.
- [`server/app/ml/predictor.py`](server/app/ml/predictor.py): AI model inference.
- [`server/app/routes/routes.py`](server/app/routes/routes.py): API routes.
- [`my-expo-app/src/pages/home.tsx`](my-expo-app/src/pages/home.tsx): Dashboard UI.
- [`my-expo-app/src/pages/manual.tsx`](my-expo-app/src/pages/manual.tsx): Manual detection UI.

---

## License

MIT License

---

For questions or contributions, open an
