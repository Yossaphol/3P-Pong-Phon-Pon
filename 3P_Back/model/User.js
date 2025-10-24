const mongoose = require('mongoose');
const Schema = mongoose.Schema;

const userSchema = new mongoose.Schema({
	userId : {
		type: String,
		required: true,
		unique : true
	},
	username : {
		type : String,
		required : true
	}
}, { collection: 'customers', versionKey: false });
//userId, username

module.exports = mongoose.model('User', userSchema)