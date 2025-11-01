const express = require('express');
const app = express();
const cors = require('cors');
const corsOption = require('./config/corsOptions');
require('dotenv').config();
const mongoose =require('mongoose');
const connectDB = require('./config/db');
const axios = require('axios');
const User = require('./model/User');



//json body parsing
app.use(express.json());
app.use(express.urlencoded({ extended: true }));

//connnect database
connectDB();

//api
// app.post("/send-coupon", async (req, res) => {
//     try {
//         const { userId, coupon } = req.body;

//         console.log("📩 Received coupon:", req.body);

//         // ✅ ส่งข้อความเข้า LINE Messaging API
//         await axios.post(
//             "https://api.line.me/v2/bot/message/push",
//             {
//                 to: userId,
//                 messages: [
//                     {
//                         type: "text",
//                         text: `🎉 คุณได้รับคูปอง: ${coupon}`
//                     }
//                 ]
//             },
//             {
//                 headers: {
//                     "Content-Type": "application/json",
//                     Authorization: `Bearer ${process.env.LINE_CHANNEL_ACCESS_TOKEN}`
//                 }
//             }
//         );

//         res.json({ message: "✅ Coupon sent to LINE user!" });
        
//     } catch (err) {
//         console.error("❌ LINE API error:", err.response?.data || err.message);
//         res.status(500).json({ error: "Failed to send coupon via LINE" });
//     }
// });
app.post("/send-coupon", async (req, res) => {
    try {
        const { name, details, startDate, endDate, customers } = req.body;

        console.log("📩 Received coupon data:", req.body);

        for (const customer of customers) {
            await axios.post(
                "https://api.line.me/v2/bot/message/push",
                {
                    to: customer.userId,
                    messages: [
                        {
                            type: "text",
                            text: `🎁 คุณได้รับคูปองใหม่!\n\n${name}\n${details}\n📅 ใช้ได้: ${startDate} - ${endDate}`
                        }
                    ]
                },
                {
                    headers: {
                        "Content-Type": "application/json",
                        Authorization: `Bearer ${process.env.LINE_CHANNEL_ACCESS_TOKEN}`
                    }
                }
            );

            console.log(`✅ Sent to ${customer.userId}`);
        }

        res.json({ message: "✅ ส่งคูปองให้ทุกคนสำเร็จ!" });

    } catch (err) {
        console.error("❌ LINE API error:", err.response?.data || err.message);
        res.status(500).json({ error: "ส่งคูปองไม่สำเร็จ" });
    }
});



//env data
const port = process.env.PORT;

app.get("/", (req, res)=>{
    res.send("3P มาแว้ว");
})

app.use(cors());
// app.use(cors(corsOption)); //cors


app.get('/details', async (req, res)=>{
    try{
        const users = await User.find();
        res.json(users);
    }catch (err)
    {
        console.log(err);
        res.status(500);
    }
}
    
); //รับข้อมูลลูกค้าจาก Portal บันทึกลง db, ให้ข้อมูลกับ manager
app.post('/add', async (req, res)=>{
    try
        {
            const data = req.body;
    
            if (!data.name || !data.userId)
            {
                return res.status(400).json({"message":"Please validate your data and try again"});
            }
            const newCustomer = new User({
                userId: data.userId,
                name: data.name,
                pictureUrl: data.pictureUrl,
                timestamp: new Date(data.timestamp)
            })
            await newCustomer.save();
            res.json({message: 'customer receieved'});
        }
        catch (err)
        {
            res.status(500).json({message: err.message});
        }
    }
);

// // 404 Handler - This should be after your routes
// app.use((req, res)=>{
//     // Log 404 attempts so you can see if something is broken
//     res.status(404).send("404 not found");
// });

// Optional: A basic error handler
// This will catch errors from your routes

app.use((err, req, res, next) => {
    // Log the full error
   
    res.status(500).send('Something broke!');
});

mongoose.connection.once('open', ()=>{

    
    app.listen(port, ()=>{

        console.log(`Server is running on port ${port}`);
    });
});

// Also a good idea to log connection errors!
mongoose.connection.on('error', (err) => {
    console.error('MongoDB connection error:', err);
});
