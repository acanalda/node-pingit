var pingit = require('bindings')('pingit.node');

module.exports = {
  /**
   * Pings a server syncronously (max wait time = 5s). Returns time in miliseconds
   *
   * @param  {String} url
   * @return {Number}
   */
  ping: function(domain) {
    return pingit.ping(domain);
  },

  /**
   * Unescape special characters in the given string of html.
   *
   * @param  {String} html
   * @return {String}
   */
  pingAsync: function(domain, callback) {
    pingit.pingAsync(domain, function(err, miliseconds){
      if(miliseconds === 0) return callback(new Error('Timeout +5s'));
      if(miliseconds === -1) return callback(new Error('Socket error'));
      if(miliseconds === -2) return callback(new Error('Malloc error'));
      if(miliseconds === -3) return callback(new Error('Unknown host'));

      callback(null, miliseconds);
    });
  }
};