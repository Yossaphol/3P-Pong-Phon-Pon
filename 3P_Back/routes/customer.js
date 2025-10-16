const express = require('express');
const router = express.Router();
const {customerDetail, getCustomer} = require('../controller/customerController');

router.get('/details', customerDetail);
router.post('/add', getCustomer);

module.exports = router;
