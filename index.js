var pingit = require('bindings')('pingit.node')

var domain = 'www.google.com';

pingit.pingAsync(domain, function(err, res){
  console.log('Async:' + res);
});

console.log(pingit.ping(domain));