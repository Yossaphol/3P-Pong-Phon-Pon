const mongoose = require('mongoose');
const Schema = mongoose.Schema;

const userSchema = new Schema({
	userId: {
		type: String,
		required: true,
		unique: true
	},
	name: {
		type: String,
		required: true
	},
	timestamp: {
		type: Date, 
		required: true 
	},
	pictureUrl: {
		type: String,
		required: false 
	}
}, { collection: 'customers', versionKey: false });

module.exports = mongoose.model('User', userSchema);
