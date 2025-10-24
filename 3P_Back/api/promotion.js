const express = require('express');
const router  = express.Router();
const {sendPromotion} = require('../controller/promotionController');

router.post('/send', sendPromotion);

module.exports = router;