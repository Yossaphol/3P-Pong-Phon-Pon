

exports.customerDetail = async (req, res)=>{ //ให้ข้อมูลลูกค้า
	res.status(200);
}

exports.getCustomer = async (req, res) => { //รับลูกค้าจากหน้าบ้าน และบันทึกลง db

	res.json({message: 'customer receieved'});
}
