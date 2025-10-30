const User = require('../model/User');
const logger = require('../logger');

exports.customerDetail = async (req, res)=>{ //ให้ข้อมูลลูกค้า
	try{
		const users = await User.find();
		logger.info("GET customer");
		res.json(users);
	}catch (err)
	{
		console.log(err);
		res.status(500);
	}
}

exports.getCustomer = async (req, res) => { //รับลูกค้าจากหน้าบ้าน และบันทึกลง db
	try
	{
		const data = req.body;

		if (!data.name || !data.userId)
		{
			logger.warn("Warning from POST /api/customer/add");
			return res.status(400).json({"message":"Please validate your data and try again"});
		}
		const newCustomer = new User({
			userId: data.userId,
			name: data.name,
			pictureUrl: data.pictureUrl,
			timestamp: new Date(data.timestamp)
		})
		await newCustomer.save();
		logger.info(`Received data: ${JSON.stringify(data, null, 2)}`);
		res.json({message: 'customer receieved'});
	}
	catch (err)
	{
		res.status(500).json({message: err.message});
	}
}

