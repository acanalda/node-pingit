# node-pingit
Just a minimalistic nodeJS module for executing the Ping command

Requirements
------------
nodeJS = v0.12 
 
Installation
------------

``` bash
$ npm install node-pingit
```

Async usage (recommended)
---------

```js
var pingit = require('node-pingit')

pingit.pingAsync('www.google.com', function(err, miliseconds){
  console.log('Async ping: '+miliseconds); 
});
```

Sync usage
---------

```js
var pingit = require('node-pingit')

var miliseconds = pingit.ping('www.google.com');
console.log('Sync ping: '+miliseconds); 
```

Tests
-----

``` bash
$ npm test
```
