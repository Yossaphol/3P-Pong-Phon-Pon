import express from "express";
import fetch from "node-fetch";
import bodyParser from "body-parser";
import cors from "cors";
import mqtt from "mqtt";
import path from "path";
import { fileURLToPath } from "url";

const __filename = fileURLToPath(import.meta.url);
const __dirname = path.dirname(__filename);

const app = express();
app.use(cors());
app.use(bodyParser.json());
app.use(bodyParser.urlencoded({ extended: true }));
app.use(express.static(__dirname));

const CHANNEL_ID = "2008301054";
const CHANNEL_SECRET = "7266b61735169d4c24e3d52dafc4475b";
const REDIRECT_URI = "https://bilgier-unabstractively-justine.ngrok-free.dev/callback";
const ADD_FRIEND_URL = "https://line.me/R/ti/p/@268wgdln";

const mqttTopic = "offlearn/notify_login";
const mqttClient = mqtt.connect("mqtt://broker.hivemq.com");

mqttClient.on("connect", () => {
  console.log("MQTT connected, clearing retained...");
  mqttClient.publish(mqttTopic, "", { retain: true });
});

mqttClient.on("error", (err) => {
  console.error("MQTT connection error:", err.message);
});

mqttClient.on("close", () => {
  console.warn("MQTT connection closed, will retry...");
});

app.get("/", (req, res) => {
  res.send("OffLearn LINE backend is running");
});

app.post("/liff_notify", async (req, res) => {
  const { userId, name, timestamp } = req.body;
  if (!userId || !name) {
    console.warn("Missing user info from LIFF notify");
    return res.status(400).send("Missing user info");
  }

  console.log("LIFF notify received:", { userId, name, timestamp });
  const payload = { userId, name, timestamp };
  mqttClient.publish(mqttTopic, JSON.stringify(payload), { qos: 1 });

  try {
    const backendRes = await fetch("https://mirier-vagrantly-regena.ngrok-free.dev/api/customer/add", {
      method: "POST",
      headers: { "Content-Type": "application/json" },
      body: JSON.stringify(payload),
    });

    const backendText = await backendRes.text();

    if (backendRes.ok) {
      console.log(`(LIFF_NOTIFY) ส่งข้อมูลไป Backend สำเร็จ (status: ${backendRes.status})`);
    } else {
      console.warn(`(LIFF_NOTIFY) ส่งข้อมูลไป Backend ไม่สำเร็จ (status: ${backendRes.status})`);
    }

    console.log("(LIFF_NOTIFY) Backend response:", backendText);
  } catch (backendErr) {
    console.error("(LIFF_NOTIFY) Error ส่งข้อมูลไป Backend:", backendErr.message);
  }

  res.send("LIFF notification received successfully");
});

app.get("/login", (req, res) => {
  const rawState = req.query.state || "";
  const lineUrl =
    `https://access.line.me/oauth2/v2.1/authorize?response_type=code` +
    `&client_id=${CHANNEL_ID}` +
    `&redirect_uri=${encodeURIComponent(REDIRECT_URI)}` +
    `&state=${encodeURIComponent(rawState)}` +
    `&scope=openid%20profile` +
    `&prompt=consent` +
    `&bot_prompt=aggressive`;

  console.log("Redirecting user to LINE:", lineUrl);
  res.redirect(lineUrl);
});

app.get("/callback", async (req, res) => {
  console.log("LINE Callback triggered");

  const { code } = req.query;
  if (!code) return res.status(400).send("Missing authorization code");

  try {
    const tokenRes = await fetch("https://api.line.me/oauth2/v2.1/token", {
      method: "POST",
      headers: { "Content-Type": "application/x-www-form-urlencoded" },
      body: new URLSearchParams({
        grant_type: "authorization_code",
        code,
        redirect_uri: REDIRECT_URI,
        client_id: CHANNEL_ID,
        client_secret: CHANNEL_SECRET,
      }),
    });

    const rawText = await tokenRes.text();
    console.log("Token response raw:", rawText);

    const tokenData = JSON.parse(rawText);
    if (!tokenData.access_token) {
      console.error("No access_token in response:", tokenData);
      return res.status(400).send("LINE Token Error");
    }

    const profileRes = await fetch("https://api.line.me/v2/profile", {
      headers: { Authorization: `Bearer ${tokenData.access_token}` },
    });

    const profile = await profileRes.json();
    console.log("👤 LINE Profile:", profile);

    const payload = {
      userId: profile.userId,
      name: profile.displayName,
      pictureUrl: profile.pictureUrl || "",
      timestamp: new Date().toISOString(),
    };

    mqttClient.publish(mqttTopic, JSON.stringify(payload), { qos: 1, retain: false });
    console.log("MQTT sent:", JSON.stringify(payload, null, 2));

    try {
      const backendRes = await fetch("https://mirier-vagrantly-regena.ngrok-free.dev/BACKEND_URL/api/customer/add", {
        method: "POST",
        headers: { "Content-Type": "application/json" },
        body: JSON.stringify(payload),
      });

      const backendText = await backendRes.text();

      if (backendRes.ok) {
        console.log(`ส่งข้อมูลไป Backend สำเร็จ (status: ${backendRes.status})`);
      } else {
        console.warn(`ส่งข้อมูลไป Backend ไม่สำเร็จ (status: ${backendRes.status})`);
      }

      console.log("Backend response:", backendText);
    } catch (backendErr) {
      console.error("Error ส่งข้อมูลไป Backend:", backendErr.message);
    }

    res.send(`
      <html>
        <head>
          <meta http-equiv="refresh" content="2;url=${ADD_FRIEND_URL}">
          <title>Login Success</title>
          <style>
            body { font-family: sans-serif; text-align: center; margin-top: 50px; }
            h2 { color: #06c755; }
            .loader {
              margin: 20px auto;
              border: 6px solid #f3f3f3;
              border-top: 6px solid #06c755;
              border-radius: 50%;
              width: 50px; height: 50px;
              animation: spin 1s linear infinite;
            }
            @keyframes spin { 100% { transform: rotate(360deg); } }
          </style>
        </head>
        <body>
          <h2>Login สำเร็จ</h2>
          <div class="loader"></div>
          <p>กำลังพาคุณไปเพิ่มเพื่อนใน LINE Official Account...</p>
          <p>ถ้าไม่ถูกพาไปอัตโนมัติ <a href="${ADD_FRIEND_URL}">คลิกที่นี่</a></p>
        </body>
      </html>
    `);

  } catch (err) {
    console.error("Callback Error:", err);
    res.status(500).send("Internal Server Error: " + err.message);
  }
});

process.on("uncaughtException", (err) => {
  console.error("Uncaught Exception:", err);
});

process.on("unhandledRejection", (err) => {
  console.error("Unhandled Rejection:", err);
});

const PORT = 3000;
app.listen(PORT, () => console.log(`Backend running on port ${PORT}`));
