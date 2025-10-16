const mongoose = require('mongoose');
const Schema = mongoose.Schema;

const userSchema = new mongoose.Schema({}, { collection: 'customers' });


module.exports = mongoose.model('User', userSchema)