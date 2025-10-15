const express = require('express');
const app = express();
const cors = require('cors');
const corsOption = require('./config/corsOptions');
require('dotenv').config()
//env data
const port = 3000; //process.env.port

app.get("/", (req, res)=>{
	res.send("3P มาแว้ว")
})

app.use(cors(corsOption));

//app.use('/api/auth', )

app.listen(port, ()=>{
	console.log(`Server is running on port ${port}`);
})

