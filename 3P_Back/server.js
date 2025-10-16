const express = require('express');
const app = express();
const cors = require('cors');
const corsOption = require('./config/corsOptions');
require('dotenv').config()

//routes
const promotionRoute = require('./routes/promotion');

//env data
const port = process.env.PORT;

app.get("/", (req, res)=>{
	res.send("3P มาแว้ว")
})

app.use(cors(corsOption)); //cors

app.use('/api/promotion', promotionRoute);

app.listen(port, ()=>{
	console.log(`Server is running on port ${port}`);
})

