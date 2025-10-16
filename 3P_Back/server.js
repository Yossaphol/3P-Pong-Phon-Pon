const express = require('express');
const app = express();
const cors = require('cors');
const corsOption = require('./config/corsOptions');
require('dotenv').config()
const mongoose = require('mongoose');
const connectDB = require('./config/db');


//connnect database
connectDB();

//routes
const promotionRoute = require('./routes/promotion');
const customerRoute = require('./routes/customer');

//env data
const port = process.env.PORT;

app.get("/", (req, res)=>{
	res.send("3P มาแว้ว")
})

app.use(cors(corsOption)); //cors

app.use('/api/promotion', promotionRoute); //ส่งโปรโมชั่น
app.use('/api/customer', customerRoute); //รับข้อมูลลูกค้าจาก Portal บันทึกลง db, ให้ข้อมูลกับ manager


app.use((req, res)=>{
	res.status(404);
})

mongoose.connection.once('open', ()=>{
	console.log('Connected to MongoDB');
	app.listen(port, ()=>{	console.log(`Server is running on port ${port}`);})
})

