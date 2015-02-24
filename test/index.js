var assert = require('assert'),
  pingit = require('../index');

describe('#pingit', function() {

  it('ping sync', function(done) {
    var miliseconds = pingit.ping('www.google.com');

    assert.equal(miliseconds>0, true);

    done();
  });

  it('ping async', function(done) {
    pingit.pingAsync('www.google.com', function(err, miliseconds){
      assert.equal(miliseconds>0, true);

      done();
    });
  });
});
