const User = require('../model/User');

exports.customerDetail = async (req, res)=>{ //ให้ข้อมูลลูกค้า
	try{
		const users = await User.find();
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

		if (!data.username || !data.userid)
		{
			return res.status(400).json({"message":"Please validate your data and try again"});
		}
		const newCustomer = new User({
			userId : data.userid,
			username : data.username
		})
		await newCustomer.save();
		res.json({message: 'customer receieved'});
	}
	catch (err)
	{
		res.status(500).json({message: err.message});
	}
}
