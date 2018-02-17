var nbind = require('nbind');
var lib = nbind.init().lib;

dm = lib.GRDevManager();
// console.log(dm.getAvalibleDevices());
available_devs = dm.getAvalibleDevices();
console.log(available_devs);
console.log("Got devices, printing names:");
available_devs.forEach(function(el) {
    console.log(el.get_name());
});
