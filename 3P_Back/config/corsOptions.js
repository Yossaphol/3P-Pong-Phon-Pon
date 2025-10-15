const whiteList = ['http://127.0.0.1:5173/'] //process.env.frontend_url

const corsOption = {
	origin: (origin, callback) =>{
		if (whiteList.includes(origin)) //ใช้เงื่อนไขนี้ถ้าต้องการใช้เครื่องมือเทส API: if (whiteList.includes(origin) || !origin) {...}
		{
			callback(null, true);
		}
		else
		{
			callback(new Error('Not allowed by CORS'));
		}
	},
	optionsSuccessStatus:200
};

module.exports = corsOption;

