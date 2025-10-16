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
	//const data = req.body;

	// if (!data.name)
	// {
	// 	return res.status(404).json({"message":"Please validate your data and try again"});
	// }

	res.json({message: 'customer receieved'});
}
