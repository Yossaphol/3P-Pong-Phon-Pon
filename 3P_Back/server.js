// const express = require('express');
// const app = express();
// const cors = require('cors');
// const corsOption = require('./config/corsOptions');
// require('dotenv').config()
// const mongoose = require('mongoose');
// const connectDB = require('./config/db');
// const logger = require('./logger');

// //json body parsing
// app.use(express.json());
// app.use(express.urlencoded({ extended: true }));


// //connnect database
// connectDB();

// //api
// const promotionRoute = require('./api/promotion');
// const customerRoute = require('./api/customer');

// //env data
// const port = process.env.PORT;

// app.get("/", (req, res)=>{
// 	res.send("3P มาแว้ว")
// })

// app.use(cors(corsOption)); //cors

// app.use('/api/promotion', promotionRoute); //ส่งโปรโมชั่น
// app.use('/api/customer', customerRoute); //รับข้อมูลลูกค้าจาก Portal บันทึกลง db, ให้ข้อมูลกับ manager


// // app.use((req, res)=>{
// // 	res.status(404).send("404 not found");
// // })

// mongoose.connection.once('open', ()=>{
// 	logger('Connected to MongoDB');
// 	app.listen(port, ()=>{	logger(`Server is running on port ${port}`);})
// })


const express = require('express');
const app = express();
const cors = require('cors');
const corsOption = require('./config/corsOptions');
require('dotenv').config();
const mongoose =require('mongoose');
const connectDB = require('./config/db');
const logger = require('./logger'); // <-- Logger is imported

logger.info('Server process starting...'); // <-- Good to log when it starts

//json body parsing
app.use(express.json());
app.use(express.urlencoded({ extended: true }));

//connnect database
connectDB();

//api
const promotionRoute = require('./api/promotion');
const customerRoute = require('./api/customer');

//env data
const port = process.env.PORT;

app.get("/", (req, res)=>{
    res.send("3P มาแว้ว");
})

app.use(cors(corsOption)); //cors

app.use('/api/promotion', promotionRoute); //ส่งโปรโมชั่น
app.use('/api/customer', customerRoute); //รับข้อมูลลูกค้าจาก Portal บันทึกลง db, ให้ข้อมูลกับ manager

// // 404 Handler - This should be after your routes
// app.use((req, res)=>{
//     // Log 404 attempts so you can see if something is broken
//     logger.warn(`404 Not Found - ${req.method} ${req.originalUrl} from ${req.ip}`);
//     res.status(404).send("404 not found");
// });

// Optional: A basic error handler
// This will catch errors from your routes
app.use((err, req, res, next) => {
    // Log the full error
    logger.error(err.message, { stack: err.stack, url: req.originalUrl });
    res.status(500).send('Something broke!');
});

mongoose.connection.once('open', ()=>{
    // ✅ Correct: Use logger.info()
    logger.info('Connected to MongoDB');
    
    app.listen(port, ()=>{
        // ✅ Correct: Use logger.info()
        logger.info(`Server is running on port ${port}`);
    });
});

// Also a good idea to log connection errors!
mongoose.connection.on('error', (err) => {
    // ✅ Correct: Use logger.error() for errors
    logger.error('MongoDB connection error:', err);
});
