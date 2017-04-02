# node-pingit
A minimalistic Node.js module for checking and monitoring server availability.

Installation
------------
Latest version of node-pingit is installable through npm.
``` bash
$ npm install node-pingit
```

Async usage (recommended)
---------
Launch a concurrent process to avoid bottlenecks. This enhances the overall responsiveness of your application.
```js
var pingit = require('node-pingit');

pingit.pingAsync('www.google.com', function(err, miliseconds){
  console.log('Async ping: '+miliseconds); 
});
```

Sync usage
---------
Your program will wait for ping command to finish before moving on to another task.
```js
var pingit = require('node-pingit');

var miliseconds = pingit.ping('www.google.com');
console.log('Sync ping: '+miliseconds); 
```

Tests
-----
You can run the test file in the library.
``` bash
$ npm run test
```
