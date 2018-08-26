const {writeFile} = require('fs');
const {join} = require('path');
const package = require('../package.json');
const bsconfig = require('../bsconfig.json');

bsconfig.version = package.version;

writeFile(
  join(__dirname, '../bsconfig.json'),
  JSON.stringify(bsconfig, null, 2),
  (err) => {
    if (err) throw err;
  }
);
