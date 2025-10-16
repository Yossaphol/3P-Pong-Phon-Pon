const whiteList = [process.env.FRONTEND_URL]

const corsOption = {
	origin: (origin, callback) =>{
		if (whiteList.includes(origin) || !origin) 
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

