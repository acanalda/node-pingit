# pingit
Just a basic nodeJS module for executing the Ping command
 
Installation
------------

``` bash
$ npm install pingit
```

Async usage (recommended)
---------

```js
var pingit = require('pingit')

pingit.pingAsync('www.google.com', function(err, miliseconds){
  console.log('Async ping: '+miliseconds); 
});
```

Sync usage
---------

```js
var pingit = require('pingit')

var miliseconds = pingit.ping('www.google.com');
console.log('Sync ping: '+miliseconds); 
```

Tests
-----

``` bash
$ npm test
```