const winston = require('winston');
const path = require('path');

// Ensure the 'logs' directory exists
const fs = require('fs');
const logDir = path.join(__dirname, 'logs');

if (!fs.existsSync(logDir)) {
  fs.mkdirSync(logDir);
}

// Create the logger
const logger = winston.createLogger({
  level: 'info', // Default level
  format: winston.format.combine(
    winston.format.timestamp({
      format: 'YYYY-MM-DD HH:mm:ss'
    }),
    winston.format.errors({ stack: true }), // Log stack traces
    winston.format.splat(),
    winston.format.json() // Log in JSON format
  ),
  defaultMeta: { service: '3P-api' }, // Add service name to all logs
  transports: [
    // - Write all logs with level 'error' and below to `logs/error.log`
    new winston.transports.File({
      filename: path.join(logDir, 'error.log'),
      level: 'error'
    }),
    // - Write all logs with level 'info' and below to `logs/combined.log`
    new winston.transports.File({
      filename: path.join(logDir, 'combined.log')
    })
  ]
});

//
// If we're not in production, also log to the `console`
// with a simpler, colorized format.
//
if (process.env.NODE_ENV !== 'production') {
  logger.add(new winston.transports.Console({
    format: winston.format.combine(
      winston.format.colorize(),
      winston.format.simple()
    ),
    level: 'debug' // Log 'debug' and above to the console in dev
  }));
}

module.exports = logger;

